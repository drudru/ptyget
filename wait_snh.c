#include <sys/types.h>
#include <sys/wait.h>
#include "haswaitp.h"

int wait_stopnohang(wstat) int *wstat;
{
#ifdef HASWAITPID
 return waitpid(-1,wstat,WNOHANG | WUNTRACED);
#else
 return wait3(wstat,WNOHANG | WUNTRACED,(struct rusage *) 0);
#endif
}
