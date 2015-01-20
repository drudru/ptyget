/* getopt.c, getopt.h: (yet another) improved getopt clone, outer layer
Daniel J. Bernstein, djb@silverton.berkeley.edu.
Depends on subgetopt.h.
Requires stdio.
931129: Rewrote in terms of subgetopt.
No known patent problems.

Documentation in getopt.3.
*/

#include <stdio.h>
#define GETOPTNOSHORT
#include "getopt.h"
#define SUBGETOPTNOSHORT
#include "subgetopt.h"

#define getopt getoptmine
#define optind subgetoptind
#define opterr getopterr
#define optproblem subgetoptproblem
#define optprogname getoptprogname

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
     optprogname = ""; /*XXX*/
   for (s = optprogname;*s;++s)
     if (*s == '/')
       optprogname = s + 1;
  }
 c = subgetopt(argc,argv,opts);
 if (opterr)
   if (c == '?')
     if (argv[optind] && (optind < argc))
       fprintf(stderr,"%s: illegal option -- %c\n",optprogname,optproblem);
     else
       fprintf(stderr,"%s: option requires an argument -- %c\n"
         ,optprogname,optproblem);
 return c;
}
