#include <sys/types.h>
#include <grp.h>
#include "subfd.h"
#include "substdio.h"
#include "fmt.h"
#include "exit.h"
#include "conf-group.h"

void fatal(type,name) char *type; char *name;
{
  substdio_puts(subfderr,"fatal: can't find ");
  substdio_puts(subfderr,type);
  substdio_puts(subfderr," ");
  substdio_puts(subfderr,name);
  substdio_putsflush(subfderr,"\n");
  _exit(1);
}

void define(s,d) char *s; int d;
{
  char strd[FMT_ULONG];
  strd[fmt_ulong(strd,(unsigned long) d)] = 0;
  substdio_puts(subfdout,"#define ");
  substdio_puts(subfdout,s);
  substdio_puts(subfdout," ");
  substdio_puts(subfdout,strd);
  substdio_puts(subfdout,"\n");
}

void group(s,g) char *s; char *g;
{
  struct group *gr;
  gr = getgrnam(g);
  if (!gr) fatal("group",g);
  define(s,(int) gr->gr_gid);
}

void main()
{
  substdio_puts(subfdout,"#ifndef AUTO_GIDS_H\n#define AUTO_GIDS_H\n");
  group("GID_TTY",GROUP_TTY);
  substdio_putsflush(subfdout,"#endif\n");
  _exit(0);
}
