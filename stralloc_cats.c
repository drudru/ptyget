#include "byte.h"
#include "str.h"
#include "stralloc.h"

int stralloc_cats(sa,s)
stralloc *sa;
char *s;
{
 int i;
 if (!sa->s) return stralloc_copys(sa,s);
 i = str_len(s);
 if (!stralloc_readyplus(sa,i)) return 0;
 byte_copy(sa->s + sa->len,i,s);
 sa->len += i;
 return 1;
}
