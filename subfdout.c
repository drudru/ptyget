#include "readwrite.h"
#include "substdio.h"
#include "subfd.h"

char subfd_outbuf[SUBSTDIO_OUTSIZE];
static struct substdio out =
  SUBSTDIO_FDBUF(write,1,subfd_outbuf,SUBSTDIO_OUTSIZE);
struct substdio *subfdout = &out;
