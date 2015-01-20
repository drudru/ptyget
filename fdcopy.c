#include <fcntl.h>
#include "fdcopy.h"

int fdcopy(to,from)
int to;
int from;
{
 if (to == from) return 0;
 if (fcntl(from,F_GETFL,0) == -1) return -1;
 close(to);
 if (fcntl(from,F_DUPFD,to) == -1) return -1;
 return 0;
}

int fdmove(to,from)
int to;
int from;
{
 if (to == from) return 0;
 if (fdcopy(to,from) == -1) return -1;
 close(from);
 return 0;
}
