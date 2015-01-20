#ifndef TTYMODES_H
#define TTYMODES_H

#include <sys/ioctl.h>
#include <termios.h>

struct ttymodes {
  struct termios ti;
  struct winsize ws;
} ;

extern int ttymodes_gett();
extern int ttymodes_getw();
extern int ttymodes_get();
extern int ttymodes_sett();
extern int ttymodes_setw();
extern int ttymodes_set();

extern void ttymodes_makeraw();
extern void ttymodes_sane();

#endif
