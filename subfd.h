#ifndef SUBFD_H
#define SUBFD_H

extern struct substdio *subfdin;
extern struct substdio *subfdout;
extern struct substdio *subfderr;

extern int subfd_read();

#endif
