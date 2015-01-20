#include "stralloc.h"
#include "byte.h"

int stralloc_copyb(sa,s,n)
stralloc *sa;
char *s;
unsigned int n;
{
 if (!stralloc_ready(sa,n)) return 0;
 byte_copy(sa->s,n,s);
 sa->len = n;
 return 1;
}
