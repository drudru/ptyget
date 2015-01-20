#include "substdio.h"
#include "byte.h"
#include "error.h"

static int oneread(op,fd,buf,len)
register int (*op)();
register int fd;
register char *buf;
register int len;
{
 register int r;

 for (;;)
  {
   r = op(fd,buf,len);
   if (r == -1)
     if (errno == error_intr)
       continue;
   break;
  }
 return r;
}

static int fillit(s)
substdio *s; /* empty right-justified buffer */
{
 char *y;
 int r;
 int q;

 q = s->n;
 y = s->x;

 r = oneread(s->op,s->fd,y,q);
 if (r <= 0)
   return r;

 s->p = r;
 q -= r;
 s->n = q;
 if (q > 0) /* damn, gotta shift */
   byte_copyr(y + q,r,y);

 return r;
}

static int getthis(s,buf,len)
substdio *s;
char *buf;
int len;
{
 char *y;
 int r;
 int q;

 y = s->x + s->n;
 r = s->p;
 q = r - len;
 if (q > 0) { r = len; s->p = q; } else s->p = 0;
 byte_copy(buf,r,y);
 s->n += r;
 return r;
}

int substdio_feed(s)
substdio *s;
{
 int r;
 if (s->p) return 1;
 r = fillit(s);
 if (r <= 0) return r;
 return 1;
}

int substdio_bget(s,buf,len)
substdio *s;
char *buf;
int len;
{
 int r;

 if (s->p > 0) return getthis(s,buf,len);

 r = s->n; if (r <= len) return oneread(s->op,s->fd,buf,r);

 r = fillit(s); if (r <= 0) return r;
 return getthis(s,buf,len);
}

int substdio_get(s,buf,len)
substdio *s;
char *buf;
int len;
{
 int r;

 if (s->p > 0) return getthis(s,buf,len);

 if (s->n <= len) return oneread(s->op,s->fd,buf,len);

 r = fillit(s); if (r <= 0) return r;
 return getthis(s,buf,len);
}

char *substdio_peek(s,len)
substdio *s;
int *len;
{
 *len = s->p;
 return s->x + s->n;
}

void substdio_seek(s,len)
substdio *s;
int len;
{
 s->n += len;
 s->p -= len;
}
