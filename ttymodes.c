#include <sys/types.h>
#include <sys/ioctl.h>
#include "error.h"
#include <termios.h>
#include "ttymodes.h"

int ttymodes_gett(tm,fd)
struct ttymodes *tm;
int fd;
{
  int r;
  do
    r = tcgetattr(fd,&tm->ti);
  while ((r == -1) && (errno == error_intr));
  return r;
}

int ttymodes_sett(tm,fd)
struct ttymodes *tm;
int fd;
{
  int r;
  do
    r = tcsetattr(fd,TCSADRAIN,&tm->ti);
  while ((r == -1) && (errno == error_intr));
  return r;
}

int ttymodes_getw(tm,fd)
struct ttymodes *tm;
int fd;
{
  int r;
  do
    r = ioctl(fd,TIOCGWINSZ,&tm->ws);
  while ((r == -1) && (errno == error_intr));
  return r;
}

int ttymodes_setw(tm,fd)
struct ttymodes *tm;
int fd;
{
  int r;
  do
    r = ioctl(fd,TIOCSWINSZ,&tm->ws);
  while ((r == -1) && (errno == error_intr));
  return r;
}

int ttymodes_get(tm,fd)
struct ttymodes *tm;
int fd;
{
  if (ttymodes_gett(tm,fd) == -1) return -1;
  return ttymodes_getw(tm,fd);
}

int ttymodes_set(tm,fd)
struct ttymodes *tm;
int fd;
{
  if (ttymodes_sett(tm,fd) == -1) return -1;
  return ttymodes_setw(tm,fd);
}

void ttymodes_makeraw(tm)
struct ttymodes *tm;
{
  tm->ti.c_iflag &= ~(BRKINT | IGNBRK | IGNPAR | PARMRK);
  tm->ti.c_iflag &= ~(ISTRIP | IXON | IXOFF | ICRNL | INLCR | IGNCR | IMAXBEL);
  tm->ti.c_oflag &= ~OPOST;
  tm->ti.c_lflag &= ~(ECHO | ECHONL);
  tm->ti.c_lflag &= ~(ICANON | ISIG | IEXTEN);
  tm->ti.c_cflag &= ~(CSIZE | PARENB);
  tm->ti.c_cflag |= CS8;
  tm->ti.c_cc[VMIN] = 1;
  tm->ti.c_cc[VTIME] = 0;
}

void ttymodes_sane(tm)
struct ttymodes *tm;
{
  int i;

  for (i = 0;i < sizeof(*tm);++i)
    ((char *) tm)[i] = 0;

  for (i = 0;i < sizeof(tm->ti.c_cc) / sizeof(*tm->ti.c_cc);++i)
    tm->ti.c_cc[i] = _POSIX_VDISABLE;

  tm->ti.c_iflag = BRKINT | IGNBRK | ICRNL | IXON | IMAXBEL;
  tm->ti.c_oflag = OPOST | ONLCR;
  tm->ti.c_lflag = ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOKE | ECHOCTL;
  tm->ti.c_cflag = CS8 | CREAD | HUPCL;

  /* These are critical for Solaris! Otherwise master reads eof upon TCSA. */
  cfsetispeed(&tm->ti,B38400);
  cfsetospeed(&tm->ti,B38400);

  tm->ti.c_cc[VMIN] = 1;
  tm->ti.c_cc[VTIME] = 0;
  tm->ti.c_cc[VINTR] = 3;
  tm->ti.c_cc[VQUIT] = 28;
  tm->ti.c_cc[VERASE] = 127;
  tm->ti.c_cc[VKILL] = 21;
  tm->ti.c_cc[VEOF] = 4;
  tm->ti.c_cc[VSTART] = 17;
  tm->ti.c_cc[VSTOP] = 19;
#ifdef VSUSP
  tm->ti.c_cc[VSUSP] = 26;
#endif
#ifdef VDSUSP
  tm->ti.c_cc[VDSUSP] = 25;
#endif
#ifdef VREPRINT
  tm->ti.c_cc[VREPRINT] = 18;
#endif
#ifdef VDISCARD
  tm->ti.c_cc[VDISCARD] = 15;
#endif
#ifdef VWERASE
  tm->ti.c_cc[VWERASE] = 23;
#endif
#ifdef VLNEXT
  tm->ti.c_cc[VLNEXT] = 22;
#endif
#ifdef VSTATUS
  tm->ti.c_cc[VSTATUS] = 20;
#endif
}
