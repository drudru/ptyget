#include "byte.h"
#include "stralloc.h"

int stralloc_copy(sato,safrom)
stralloc *sato;
stralloc *safrom;
{
 int i;
 i = safrom->len;
 if (!stralloc_ready(sato,i)) return 0;
 byte_copy(sato->s,i,safrom->s);
 sato->len = i;
 return 1;
}
