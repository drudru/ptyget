#include <sys/types.h>
#include <sys/wait.h>
#include "haswaitp.h"
#include "error.h"

int wait_stop(wstat) int *wstat;
{
 int r;

 do
#ifdef HASWAITPID
   r = waitpid(-1,wstat,WUNTRACED);
#else
   r = wait3(wstat,WUNTRACED,(struct rusage *) 0);
#endif
 while ((r == -1) && (errno == error_intr));

 return r;
}
