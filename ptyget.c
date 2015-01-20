#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include "env.h"
#include "fmt.h"
#include "str.h"
#include "strerr.h"
#include <termios.h>

#include "hasptmx.h"
#include "hasptc.h"
#include "conf-secure.h"


#ifdef HASPTMX
#ifndef PTYGET_OK
#define PTYGET_OK

#include <sys/sysmacros.h>
#include <sys/stropts.h>
#include <sys/stream.h>
#include <sys/ptms.h>

char fnslave[10 + FMT_ULONG]; /* 10 for /dev/pts/ */

char *pty45()
{
  struct strioctl si;
  struct stat st;
  int fd;
  char *s;

  fd = open("/dev/ptmx",O_RDWR | O_NDELAY);
  if (fd == -1) return 0;
  if (fdmove(4,fd) == -1) return 0;

  if (fstat(4,&st) == -1) return 0;
  s = fnslave;
  s += fmt_str(s,"/dev/pts/");
  s += fmt_ulong(s,(unsigned long) minor(st.st_rdev));
  *s = 0;

#ifdef PTYGET_SECURE
  if (chown(fnslave,getuid(),getegid()) == -1)
    return 0;
  if (chmod(fnslave,0600) == -1)
    return 0;
#endif

  si.ic_cmd = UNLKPT;
  si.ic_timout = 0;
  si.ic_len = 0;
  si.ic_dp = 0;
  if (ioctl(4,I_STR,&si) == -1) return 0;

  fd = open(fnslave,O_RDWR | O_NOCTTY);
  if (fd == -1) return 0;
  if (fdmove(5,fd) == -1) return 0;

  if (ioctl(5,I_PUSH,"ptem") == -1) return 0;
  if (ioctl(5,I_PUSH,"ldterm") == -1) return 0;
  if (tcflush(5,TCIOFLUSH) == -1) return 0;

  return fnslave;
}

#endif
#endif


#ifdef HASPTC
#ifndef PTYGET_OK
#define PTYGET_OK

#ifdef PTYGET_SECURE

  ERROR! ptc-based systems have no pty security.

#endif

#include <sys/sysmacros.h>

char fnslave[10 + FMT_ULONG]; /* 10 for /dev/pts/ */

char *pty45()
{
  struct stat st;
  int fd;

  fd = open("/dev/ptc",O_RDWR | O_NDELAY);
  if (fd == -1) return 0;
  if (fdmove(4,fd) == -1) return 0;

  if (fstat(4,&st) == -1) return 0;
  s = fnslave;
  s += fmt_str(s,"/dev/pts/");
  s += fmt_ulong(s,(unsigned long) minor(st.st_rdev));
  *s = 0;

  fd = open(fnslave,O_RDWR | O_NOCTTY);
  if (fd == -1) return 0;
  if (fdmove(5,fd) == -1) return 0;

  return fnslave;
}

#endif
#endif


#ifndef PTYGET_OK

char fnmaster[] = "/dev/pty??";
char fnslave[] = "/dev/tty??";

/* Do not change the sizes of pty1 and pty2! */
char pty1[16] = "pqrstuvwxyzPQRST";
char pty2[16] = "0123456789abcdef";

char *pty45()
{
  int fd;
  struct stat st;
  int pos;
  int increment;
  int loop;

  pos = 255 & getpid();
  increment = 1 + (254 & time((long *) 0));

  for (loop = 0;loop < 256;++loop) {
    pos = 255 & (pos + increment);

    fnslave[8] = fnmaster[8] = pty1[15 & pos];
    fnslave[9] = fnmaster[9] = pty2[15 & (pos >> 4)];

    if (fnmaster[8] == '-') continue;
    if (stat(fnmaster,&st) == -1) { pty1[15 & pos] = '-'; continue; }

    fd = open(fnmaster,O_RDWR | O_NDELAY);
    if (fd != -1) {
      if (fdmove(4,fd) == -1) return 0;

#ifdef PTYGET_SECURE
      if (chown(fnslave,getuid(),getegid()) == -1)
        return 0;
      if (chmod(fnslave,0600) == -1)
        return 0;
      revoke(fnslave);
#endif

      fd = open(fnslave,O_RDWR | O_NOCTTY);
      if (fd != -1) {
	if (fdmove(5,fd) == -1) return 0;
	return fnslave;
      }
      close(4);
    }
  }
  return 0;
}

#endif


void main(argc,argv)
int argc;
char **argv;
{
  char *tty;

  tty = pty45();

  if (setgid(getgid()) == -1) _exit(1);
  if (setuid(getuid()) == -1) _exit(1);

  if (!tty)
    strerr_die2sys(111,"ptyget: fatal: ","unable to allocate pty: ");

#ifndef PTYGET_SECURE
  strerr_warn1("ptyget: warning: unable to allocate secure pty",0);
#endif

  if (!env_put2("TTY",tty))
    strerr_die2x(111,"ptyget: fatal: ","out of memory");

  if (!argv[1])
    strerr_die1x(1,"ptyget: usage: ptyget subprogram [ args ... ]");

  execvp(argv[1],argv + 1);
  strerr_die4sys(111,"ptyget: fatal: ","unable to run ",argv[1],": ");
}
