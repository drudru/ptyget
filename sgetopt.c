/* sgetopt.c, sgetopt.h: (yet another) improved getopt clone, outer layer
Daniel J. Bernstein, djb@silverton.berkeley.edu.
Depends on subgetopt.h, substdio.h, subfd.h.
No system requirements.
931201: Baseline.
No known patent problems.

Documentation in sgetopt.3.
*/

#include "substdio.h"
#include "subfd.h"
#define SGETOPTNOSHORT
#include "sgetopt.h"
#define SUBGETOPTNOSHORT
#include "subgetopt.h"

#define getopt sgetoptmine
#define optind subgetoptind
#define opterr sgetopterr
#define optproblem subgetoptproblem
#define optprogname sgetoptprogname

int opterr = 1;
char *optprogname = 0;

int getopt(argc,argv,opts)
int argc;
char **argv;
char *opts;
{
 int c;
 char *s;

 if (!optprogname)
  {
   optprogname = *argv;
   if (!optprogname) /* oh boy */
     optprogname = "";
   for (s = optprogname;*s;++s)
     if (*s == '/')
       optprogname = s + 1;
  }
 c = subgetopt(argc,argv,opts);
 if (opterr)
   if (c == '?')
    {
     char chp[2]; chp[0] = optproblem; chp[1] = '\n';
     substdio_putsflush(subfderr,optprogname);
     if (argv[optind] && (optind < argc))
       substdio_putsflush(subfderr,": illegal option -- ");
     else
       substdio_putsflush(subfderr,": option requires an argument -- ");
     substdio_putflush(subfderr,chp,2);
    }
 return c;
}
