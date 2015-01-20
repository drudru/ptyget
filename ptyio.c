#include "wait.h"
#include "fork.h"
#include "select.h"
#include "sig.h"
#include <signal.h>
#include "error.h"
#include "readwrite.h"
#include "exit.h"
#include "ttyctrl.h"
#include "ttymodes.h"
#include "sgetopt.h"
#include "substdio.h"
#include "subfd.h"

char bufin[1024];
int bufinpos;
int bufinsize;
char bufout[1024];
int bufoutsize;
int bufoutpos;

struct ttymodes tminit;

int fdtty;
struct ttymodes tmraw;
int flagcont = 0;
struct ttymodes tmcont;
void sigcont()
{
  if (ttymodes_get(&tmcont,fdtty) != -1) flagcont = 1;
}

int pi9[2];
int flagptyrok = 1;
int flagptywok = 1;
int flagremote = 0;
int flagreading = 1;
int flagwriting = 1;
int flageof9 = 0;
int flagtty = 0;

void sigwinch()
{
  if (flagtty) {
    ttymodes_getw(&tminit,fdtty);
    ttymodes_setw(&tminit,5);
  }
}

void main(argc,argv)
int argc;
char **argv;
{
  int r;
  int w;
  fd_set rfds;
  fd_set wfds;
  int wstat;
  int opt;
  int child;

  while ((opt = getopt(argc,argv,"rtT")) != opteof)
    switch(opt) {
      case 'r': flagremote = 1; break;
      case 'T': flagtty = 0; break;
      case 't': flagtty = 1; break;
      case '?':
      default:
	_exit(1);
    }
  argc -= optind;
  argv += optind;
  if (!*argv) {
    substdio_putsflush(subfderr,"ptyio: usage: ptyio subprogram [ args ... ]\n");
    _exit(1);
  }

  sig_pipeignore();
  sig_block(SIGWINCH);

  if (pipe(pi9) == -1) {
    substdio_putsflush(subfderr,"ptyio: fatal: unable to create internal pipe\n");
    _exit(111);
  }

  if (!flagtty)
    ttymodes_sane(&tminit);
  else {
    fdtty = ttyctrl();
    if (fdtty == -1) {
      substdio_putsflush(subfderr,"ptyio: fatal: unable to find attached tty\n");
      _exit(111);
    }

    sig_catch(SIGTTOU,SIG_DFL);
    sig_unblock(SIGTTOU);
    sig_contcatch(sigcont);
    sig_contunblock();
  
    if (ttymodes_get(&tminit,fdtty) == -1) {
      substdio_putsflush(subfderr,"ptyio: fatal: unable to get modes of attached tty\n");
      _exit(111);
    }
    tmraw = tminit;
    ttymodes_makeraw(&tmraw);
    ttymodes_set(&tmraw,fdtty);
  
    sig_contblock();
    while (flagcont) {
      flagcont = 0;
      tminit = tmcont;
      tmraw = tminit;
      ttymodes_makeraw(&tmraw);
      sig_contunblock();
      ttymodes_set(&tmraw,fdtty);
      sig_contblock();
    }
  
    sig_contdefault();
    sig_contunblock();
  }

  if (flagremote) {
    int on;
    on = 1;
    if (ioctl(4,TIOCREMOTE,&on) == -1) {
      substdio_putsflush(subfderr,"ptyio: fatal: unable to set remote mode\n");
      _exit(111);
    }
    ttymodes_makeraw(&tminit);
  }

  sig_block(SIGTTOU);
  ttymodes_set(&tminit,5);

  switch(child = vfork()) {
    case -1:
      if (flagtty)
        ttymodes_set(&tminit,fdtty);
      substdio_putsflush(subfderr,"ptyio: fatal: unable to fork\n");
      _exit(111);
    case 0:
      close(pi9[0]);
      if (flagtty)
        close(fdtty);
      if (fdmove(9,pi9[1]) == -1) {
        substdio_putsflush(subfderr,"ptyio: fatal: unable to copy child fd\n");
        _exit(111);
      }
      sig_unblock(SIGTTOU);
      sig_unblock(SIGWINCH);
      execvp(*argv,argv);
      substdio_putsflush(subfderr,"ptyio: fatal: unable to run subprogram\n");
      _exit(111);
  }

  close(pi9[1]);
  if (fdmove(2,pi9[0]) == -1) {
    substdio_putsflush(subfderr,"ptyio: fatal: unable to copy parent fd\n");
    _exit(111);
  }

  sig_catch(SIGWINCH,sigwinch);

  bufinpos = bufinsize = 0;
  bufoutpos = bufoutsize = 0;

  while ((flagptyrok && flagwriting) || !flageof9) {
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);

    if (flagreading && flagptywok)
      if (bufinpos == bufinsize)
        FD_SET(0,&rfds);
    if (flagptywok)
      if ((bufinpos < bufinsize) || (flagremote && !flagreading))
        FD_SET(4,&wfds);
    if (flagptyrok && flagwriting)
      if (bufoutpos == bufoutsize)
        FD_SET(4,&rfds);
    if (flagwriting)
      if (bufoutpos < bufoutsize)
        FD_SET(1,&wfds);
    if (!flageof9)
      FD_SET(2,&rfds);

    sig_unblock(SIGWINCH);
    if (select(6,&rfds,&wfds,(fd_set *) 0,(struct timeval *) 0) == -1)
      continue;
    sig_block(SIGWINCH);
    
    if (FD_ISSET(0,&rfds)) if (flagreading && flagptywok) {
      r = read(0,bufin,sizeof(bufin));
      if (r <= 0)
	flagreading = 0;
      else {
	bufinpos = 0;
	bufinsize = r;
      }
    }

    if (FD_ISSET(4,&wfds)) if (flagptywok) {
      if (bufinpos < bufinsize) {
        w = write(4,bufin + bufinpos,bufinsize - bufinpos);
        if (w <= 0)
	  flagptywok = 0;
        else
	  bufinpos += w;
      }
      else if (flagremote && !flagreading) {
	w = write(4,bufin,0);
	if (w < 0)
	  flagptywok = 0;
      }
    }

    if (FD_ISSET(4,&rfds)) if (flagptyrok && flagwriting) {
      r = read(4,bufout,sizeof(bufout));
      if (r <= 0)
	flagptyrok = 0;
      else {
	bufoutpos = 0;
	bufoutsize = r;
      }
    }

    if (FD_ISSET(1,&wfds)) if (flagwriting) {
      w = write(1,bufout + bufoutpos,bufoutsize - bufoutpos);
      if (w <= 0) {
	flagwriting = 0;
	kill(child,SIGPIPE);
      }
      else
	bufoutpos += w;
    }

    if (FD_ISSET(2,&rfds)) if (!flageof9) {
      unsigned char ch;

      r = read(2,&ch,1);
      if (r <= 0) {
	flageof9 = 1;
	close(5);
      }
      else {
        if (flagtty) {
          ttymodes_set(&tminit,fdtty);
          sig_dfl((unsigned int) ch);
          sig_unblock(SIGTTOU);
          ttymodes_sett(&tmraw,fdtty);
          sig_block(SIGTTOU);
	  ttymodes_getw(&tminit,fdtty);
	  ttymodes_setw(&tminit,5);
        }
        kill(child,SIGCONT);
      }
    }
  }

  close(4);
  r = wait_pid(&wstat,child);
  if (flagtty)
    ttymodes_set(&tminit,fdtty);
  if (r == -1) {
    substdio_putsflush(subfderr,"ptyio: fatal: child disappeared!\n");
    _exit(111);
  }
  if (wait_crashed(wstat)) {
    substdio_putsflush(subfderr,"ptyio: fatal: child crashed\n");
    _exit(111);
  }
  _exit(wait_exitcode(wstat));
}
