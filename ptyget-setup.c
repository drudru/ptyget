#include <sys/types.h>
#include "subfd.h"
#include "readwrite.h"
#include "substdio.h"
#include "stralloc.h"
#include "open.h"
#include "exit.h"
#include "error.h"
#include "conf-home.h"
#include "conf-secure.h"
#include "auto-gids.h"

void die(s,fn,t,err) char *s; char *fn; char *t; char *err;
{
  substdio_puts(subfderr,"ptyget-setup: ");
  substdio_puts(subfderr,s);
  substdio_puts(subfderr,fn);
  substdio_puts(subfderr,t);
  substdio_puts(subfderr,err);
  substdio_puts(subfderr,"\n");
  substdio_flush(subfderr);
  _exit(1);
}
void die_nomem() { die("fatal: out of memory","","",""); }
void die_write(s) char *s; { die("fatal: unable to write ",s,": ",error_str(errno)); }
void die_read(s) char *s; { die("fatal: unable to read ", s,": ",error_str(errno)); }
void die_chmod(s) char *s; { die("fatal: unable to chmod ", s,": ",error_str(errno)); }
void die_chown(s) char *s; { die("fatal: unable to chown ", s,": ",error_str(errno)); }
void die_mkdir(s) char *s; { die("fatal: unable to mkdir ", s,": ",error_str(errno)); }

stralloc homefn = {0};

void makehomefn(fn)
char *fn;
{
  if (!stralloc_copys(&homefn,CONF_HOME)) die_nomem();
  if (!stralloc_cats(&homefn,"/")) die_nomem();
  if (!stralloc_cats(&homefn,fn)) die_nomem();
  if (!stralloc_0(&homefn)) die_nomem();
}

void protect(fn,mode)
char *fn;
int mode;
{
#ifdef PTYGET_SECURE
  if (chown(fn,0,GID_TTY) == -1) die_chown(fn);
#endif
  if (chmod(fn,mode) == -1) die_chmod(fn);
}

char inbuf[SUBSTDIO_INSIZE];
char outbuf[SUBSTDIO_OUTSIZE];

void copy(fnfrom,fn,mode)
char *fnfrom;
char *fn;
int mode;
{
  int fd;
  int fdfrom;
  substdio ssin;
  substdio ssout;
 
  makehomefn(fn);
  fdfrom = open_read(fnfrom);
  if (fdfrom == -1) die_read(fnfrom);
  fd = open_trunc(homefn.s);
  if (fd == -1) die_write(fn);
 
  substdio_fdbuf(&ssout,write,fd,outbuf,sizeof(outbuf));
  substdio_fdbuf(&ssin,read,fdfrom,inbuf,sizeof(inbuf));
 
  switch(substdio_copy(&ssout,&ssin))
   {
    case -2: die_read(fnfrom);
    case -3: die_write(fn);
   }
 
  if (substdio_flush(&ssout) == -1) die_write(fn);
 
  close(fdfrom);
  close(fd);
  protect(homefn.s,mode);
}

void creatdir(fn,mode)
char *fn;
int mode;
{
  makehomefn(fn);
  if (mkdir(homefn.s,mode) == -1)
    if (errno != error_exist)
      die_mkdir(homefn.s);
  protect(homefn.s,mode);
}

void main()
{
  creatdir("bin",0755);
#ifdef PTYGET_SECURE
  copy("ptyget","bin/ptyget",06755);
#else
  copy("ptyget","bin/ptyget",00755);
#endif
  copy("ptyrun","bin/ptyrun",0755);
  copy("ptybandage","bin/ptybandage",0755);
  copy("nobuf","bin/nobuf",0755);
  copy("background","bin/background",0755);
  copy("ttydetach","bin/ttydetach",0755);
  copy("ptyspawn","bin/ptyspawn",0755);
  copy("ptyio","bin/ptyio",0755);
  copy("exclon","bin/exclon",0755);
  copy("excloff","bin/excloff",0755);

  creatdir("man",0755);
  creatdir("man/cat1",0755);
  creatdir("man/cat8",0755);

  copy("ptyget.0","man/cat8/ptyget.0",0644);
  copy("ptybandage.0","man/cat1/ptybandage.0",0644);
  copy("ptyrun.0","man/cat1/ptyrun.0",0644);
  copy("nobuf.0","man/cat1/nobuf.0",0644);
  copy("background.0","man/cat1/background.0",0644);
  copy("ttydetach.0","man/cat1/ttydetach.0",0644);
  copy("ptyspawn.0","man/cat8/ptyspawn.0",0644);
  copy("ptyio.0","man/cat8/ptyio.0",0644);
  copy("exclon.0","man/cat1/exclon.0",0644);
  copy("excloff.0","man/cat1/excloff.0",0644);

  _exit(0);
}
