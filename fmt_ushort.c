#include "fmt.h"

unsigned int fmt_ushort(s,u) register char *s; register unsigned short u;
{
 register unsigned long l; l = u; return fmt_ulong(s,l);
}
