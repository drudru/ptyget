#include "byte.h"
#include "stralloc.h"

int stralloc_cat(sato,safrom)
stralloc *sato;
stralloc *safrom;
{
 int i;
 if (!sato->s) return stralloc_copy(sato,safrom);
 i = safrom->len;
 if (!stralloc_readyplus(sato,i)) return 0;
 byte_copy(sato->s + sato->len,i,safrom->s);
 sato->len += i;
 return 1;
}
