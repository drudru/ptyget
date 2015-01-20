#include "substdio.h"
#include "readwrite.h"
#include "exit.h"
#include "conf-home.h"

char buf[16];
substdio out = SUBSTDIO_FDBUF(write,1,buf,sizeof(buf));

void main()
{
 substdio_puts(&out,CONF_HOME);
 substdio_putsflush(&out,"\n");
 _exit(0);
}
