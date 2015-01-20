#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <fcntl.h>
#include "fdcopy.h"
#include "sgetopt.h"
#include "exit.h"
#include "fork.h"
#include "wait.h"
#include "env.h"
#include "error.h"
#include "readwrite.h"
#include "strerr.h"
#include "sig.h"
#include <signal.h>
#include <termios.h> /* for TIOCEXCL on Sun */

int child;

void sigcont()
{
  /*XXX: use killpg instead? */
  kill(-child,SIGCONT);
}

#define FATAL "ptyspawn: fatal: "
#define WARNING "ptyspawn: warning: "

int flagpreserve2 = 0;
int flagpreserve3 = 0;
int flagexcl = 0;

void main(argc,argv)
int argc;
char **argv;
{
  int r;
  int wstat;
  char *ttyname;
  char ch;
  int opt;

  while ((opt = getopt(argc,argv,"23x")) != opteof)
    switch(opt) {
      case '2': flagpreserve2 = 1; break;
      case '3': flagpreserve3 = 1; break;
      case 'x': flagexcl = 1; break;
      case '?':
      default:
	_exit(1);
    }
  argc -= optind;
  argv += optind;
  if (!*argv)
    strerr_die1x(1,"ptyspawn: usage: ptyspawn subprogram [ args ... ]");

  sig_pipeignore();

  ttyname = env_get("TTY");
  if (!ttyname)
    strerr_die2x(1,FATAL,"TTY not set");

  if (setsid() == -1)
    strerr_warn2(WARNING,"unable to setsid",0);

  close(0);
  if (open(ttyname,O_RDONLY) != 0)
    strerr_die4sys(111,FATAL,"unable to open ",ttyname," for reading: ");
  close(1);
  if (open(ttyname,O_WRONLY) != 1)
    strerr_die4sys(111,FATAL,"unable to open ",ttyname," for writing: ");
  if (!flagpreserve2) {
    close(2);
    if (open(ttyname,O_RDWR) != 2)
      _exit(111);
    /* now errors go to pty */
  }

#ifdef TIOCSCTTY /* otherwise, the fd 0 open automatically did it */
  if (ioctl(5,TIOCSCTTY,(char *) 0) == -1)
    strerr_warn2(WARNING,"unable to sctty",0);
#endif

  if (!flagpreserve3) {
    close(3);
    if (open("/dev/tty",O_RDWR) != 3)
      strerr_die2sys(111,FATAL,"unable to open /dev/tty: ");
  }

  if (flagexcl)
    if (ioctl(5,TIOCEXCL,(char *) 0) == -1)
      strerr_warn2(WARNING,"unable to set exclusive use",0);
  close(5);
  close(4);

  switch(child = vfork()) {
    case -1:
      strerr_die2sys(111,FATAL,"unable to fork: ");
    case 0:
      close(9);
      setpgid(getpid(),getpid());
      sig_block(SIGTTOU);
      tcsetpgrp(1,getpid());
      sig_unblock(SIGTTOU);
      sig_pipedefault();
      execvp(*argv,argv);
      if (error_temp(errno))
        strerr_die4sys(111,FATAL,"unable to run ",*argv,": ");
      strerr_die4sys(1,FATAL,"unable to run ",*argv,": ");
  }

  sig_contcatch(sigcont);

  for (;;) {
    r = wait_stop(&wstat);
    if ((r == -1) && (errno == error_intr)) continue;
    if (r == -1)
      strerr_die2sys(111,FATAL,"impossible wait failure: ");
    if (r != child) continue;
    if (!wait_stopped(wstat)) {
      if (wait_crashed(wstat))
        strerr_die2x(111,FATAL,"child crashed");
      _exit(wait_exitcode(wstat));
    }

    ch = wait_stopsig(wstat);
    do
      r = write(9,&ch,1);
    while ((r == -1) && (errno == error_intr));
  }
}
