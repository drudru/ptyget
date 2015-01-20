#include "strerr.h"
#include "error.h"

void main(argc,argv)
int argc;
char **argv;
{
  int pi[2]; /* synchronization, to eliminate zombies */
  char ch;

  if (pipe(pi) == -1)
    strerr_die2sys(111,"background: fatal: ","unable to create internal pipe: ");
  switch(fork()) {
    case -1:
      strerr_die2sys(111,"background: fatal: ","unable to fork: ");
    case 0:
      close(pi[1]);
      while (read(pi[0],&ch,1) == -1)
	if (errno != error_intr)
	  break; /* oh, well */
      close(pi[0]);
      execvp(argv[1],argv + 1);
      strerr_die4sys(111,"ttydetach: fatal: ","unable to run ",argv[1],": ");
  }
  _exit(0);
}
