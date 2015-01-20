#include "byte.h"
#include "str.h"
#include "stralloc.h"

int stralloc_copys(sa,s)
stralloc *sa;
char *s;
{
 int i;
 i = str_len(s);
 if (!stralloc_ready(sa,i + 1)) return 0;
 byte_copy(sa->s,i + 1,s);
 sa->len = i;
 sa->s[i] = 'Z'; /* ``offensive programming'' */
 return 1;
}
