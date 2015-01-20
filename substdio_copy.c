#include "substdio.h"

int substdio_copy(ssout,ssin)
substdio *ssout;
substdio *ssin;
{
 int f;
 char *x;
 int xlen;

 for (;;)
  {
   f = substdio_feed(ssin);
   if (f < 0) return -2;
   if (!f) return 0;
   x = substdio_PEEK(ssin,&xlen);
   if (substdio_put(ssout,x,xlen) == -1) return -3;
   substdio_SEEK(ssin,xlen);
  }
}
