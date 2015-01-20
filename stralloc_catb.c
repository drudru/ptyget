#include "stralloc.h"
#include "byte.h"

int stralloc_catb(sa,s,n)
stralloc *sa;
char *s;
unsigned int n;
{
 if (!sa->s) return stralloc_copyb(sa,s,n);
 if (!stralloc_readyplus(sa,n)) return 0;
 byte_copy(sa->s + sa->len,n,s);
 sa->len += n;
 return 1;
}
