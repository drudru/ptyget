default: it

alloc.o: \
compile alloc.c alloc.h alloc.c error.h alloc.c
	./compile alloc.c

alloc_re.o: \
compile alloc_re.c alloc.h alloc_re.c byte.h alloc_re.c
	./compile alloc_re.c

auto-gids.h: \
printgids
	./printgids > temp-gids.h
	mv temp-gids.h auto-gids.h

background: \
load background.o libstrerr.a libsubstdio.a liballoc.a liberror.a \
libstr.a
	./load background libstrerr.a libsubstdio.a liballoc.a \
	liberror.a libstr.a

background.0: \
background.1
	nroff -man background.1 > background.0

background.o: \
compile background.c strerr.h background.c error.h background.c
	./compile background.c

byte_chr.o: \
compile byte_chr.c byte.h byte_chr.c
	./compile byte_chr.c

byte_copy.o: \
compile byte_copy.c byte.h byte_copy.c
	./compile byte_copy.c

byte_cr.o: \
compile byte_cr.c byte.h byte_cr.c
	./compile byte_cr.c

byte_diff.o: \
compile byte_diff.c byte.h byte_diff.c
	./compile byte_diff.c

byte_rchr.o: \
compile byte_rchr.c byte.h byte_rchr.c
	./compile byte_rchr.c

byte_zero.o: \
compile byte_zero.c byte.h byte_zero.c
	./compile byte_zero.c

compile: \
make-commands systype
	./make-commands "`cat ./systype`" compile > compile
	chmod 755 compile

env.o: \
compile env.c str.h env.c alloc.h env.c env.h env.c
	./compile env.c

envread.o: \
compile envread.c env.h envread.c str.h envread.c
	./compile envread.c

error.o: \
compile error.c error.c error.h error.c
	./compile error.c

error_str.o: \
compile error_str.c error_str.c error.h error_str.c
	./compile error_str.c

error_temp.o: \
compile error_temp.c error_temp.c error.h error_temp.c
	./compile error_temp.c

excloff: \
load excloff.o
	./load excloff

excloff.0: \
excloff.1
	nroff -man excloff.1 > excloff.0

excloff.o: \
compile excloff.c excloff.c excloff.c
	./compile excloff.c

exclon: \
load exclon.o
	./load exclon

exclon.0: \
exclon.1
	nroff -man exclon.1 > exclon.0

exclon.o: \
compile exclon.c exclon.c exclon.c
	./compile exclon.c

fdcopy.o: \
compile fdcopy.c fdcopy.c fdcopy.h fdcopy.c
	./compile fdcopy.c

find-systype: \
find-systype.sh conf-cc.sh warn-auto.sh
	cat warn-auto.sh conf-cc.sh find-systype.sh > find-systype
	chmod 755 find-systype

fmt_str.o: \
compile fmt_str.c fmt.h fmt_str.c
	./compile fmt_str.c

fmt_strn.o: \
compile fmt_strn.c fmt.h fmt_strn.c
	./compile fmt_strn.c

fmt_uint.o: \
compile fmt_uint.c fmt.h fmt_uint.c
	./compile fmt_uint.c

fmt_uint0.o: \
compile fmt_uint0.c fmt.h fmt_uint0.c
	./compile fmt_uint0.c

fmt_ulong.o: \
compile fmt_ulong.c fmt.h fmt_ulong.c
	./compile fmt_ulong.c

fmt_ushort.o: \
compile fmt_ushort.c fmt.h fmt_ushort.c
	./compile fmt_ushort.c

fork.h: \
compile load tryvfork.c fork.h1 fork.h2
	( ( ./compile tryvfork.c && ./load tryvfork ) >/dev/null \
	2>&1 \
	&& cat fork.h2 || cat fork.h1 ) > fork.h
	rm -f tryvfork.o tryvfork

getopt.o: \
compile getopt.c getopt.c getopt.h getopt.h subgetopt.h getopt.h \
getopt.c subgetopt.h subgetopt.h getopt.c
	./compile getopt.c

hasptc.h: \
tryptc.c compile load
	( ( ./compile tryptc.c && ./load tryptc && ./tryptc ) \
	>/dev/null 2>&1 \
	&& echo \#define HASPTC 1 || exit 0 ) > hasptc.h
	rm -f tryptc.o tryptc

hasptmx.h: \
tryptmx.c compile load
	( ( ./compile tryptmx.c && ./load tryptmx && ./tryptmx ) \
	>/dev/null 2>&1 \
	&& echo \#define HASPTMX 1 || exit 0 ) > hasptmx.h
	rm -f tryptmx.o tryptmx

hassgact.h: \
trysgact.c compile load
	( ( ./compile trysgact.c && ./load trysgact ) >/dev/null \
	2>&1 \
	&& echo \#define HASSIGACTION 1 || exit 0 ) > hassgact.h
	rm -f trysgact.o trysgact

hassgprm.h: \
trysgprm.c compile load
	( ( ./compile trysgprm.c && ./load trysgprm ) >/dev/null \
	2>&1 \
	&& echo \#define HASSIGPROCMASK 1 || exit 0 ) > hassgprm.h
	rm -f trysgprm.o trysgprm

haswaitp.h: \
trywaitp.c compile load
	( ( ./compile trywaitp.c && ./load trywaitp ) >/dev/null \
	2>&1 \
	&& echo \#define HASWAITPID 1 || exit 0 ) > haswaitp.h
	rm -f trywaitp.o trywaitp

home: \
load home.o libsubstdio.a liberror.a libstr.a
	./load home libsubstdio.a liberror.a libstr.a

home.o: \
compile home.c substdio.h home.c readwrite.h home.c exit.h home.c \
conf-home.h home.c
	./compile home.c

it: \
ptyrun ptyrun.0 ptybandage ptybandage.0 nobuf nobuf.0 ptyget ptyget.0 \
ptyspawn ptyspawn.0 ptyio ptyio.0 ttydetach ttydetach.0 background \
background.0 exclon exclon.0 excloff excloff.0

liballoc.a: \
makelib alloc.o alloc_re.o
	./makelib liballoc.a alloc.o alloc_re.o

libenv.a: \
makelib env.o envread.o
	./makelib libenv.a env.o envread.o

liberror.a: \
makelib error.o error_str.o error_temp.o
	./makelib liberror.a error.o error_str.o error_temp.o

libfs.a: \
makelib fmt_str.o fmt_strn.o fmt_uint.o fmt_uint0.o fmt_ulong.o \
fmt_ushort.o
	./makelib libfs.a fmt_str.o fmt_strn.o fmt_uint.o \
	fmt_uint0.o fmt_ulong.o fmt_ushort.o

libgetopt.a: \
makelib subgetopt.o sgetopt.o getopt.o
	./makelib libgetopt.a subgetopt.o sgetopt.o getopt.o

libopen.a: \
makelib open_append.o open_excl.o open_read.o open_trunc.o \
open_write.o
	./makelib libopen.a open_append.o open_excl.o open_read.o \
	open_trunc.o open_write.o

libsig.a: \
makelib sig_block.o sig_catch.o sig_dfl.o sig_pipe.o sig_cont.o
	./makelib libsig.a sig_block.o sig_catch.o sig_dfl.o \
	sig_pipe.o sig_cont.o

libstr.a: \
makelib str_len.o str_diff.o str_diffn.o str_cpy.o str_chr.o \
str_rchr.o byte_chr.o byte_rchr.o byte_diff.o byte_copy.o byte_cr.o \
byte_zero.o
	./makelib libstr.a str_len.o str_diff.o str_diffn.o \
	str_cpy.o str_chr.o str_rchr.o byte_chr.o byte_rchr.o \
	byte_diff.o byte_copy.o byte_cr.o byte_zero.o

libstralloc.a: \
makelib stralloc_eady.o stralloc_pend.o stralloc_copy.o \
stralloc_opys.o stralloc_opyb.o stralloc_cat.o stralloc_cats.o \
stralloc_catb.o
	./makelib libstralloc.a stralloc_eady.o stralloc_pend.o \
	stralloc_copy.o stralloc_opys.o stralloc_opyb.o \
	stralloc_cat.o stralloc_cats.o stralloc_catb.o

libstrerr.a: \
makelib strerr.o strerr_sys.o strerr_die.o
	./makelib libstrerr.a strerr.o strerr_sys.o strerr_die.o

libsubstdio.a: \
makelib substdio.o substdi.o substdo.o subfderr.o subfdout.o \
subfdin.o substdio_copy.o
	./makelib libsubstdio.a substdio.o substdi.o substdo.o \
	subfderr.o subfdout.o subfdin.o substdio_copy.o

libwait.a: \
makelib wait_pid.o wait_nohang.o wait_stop.o wait_snh.o
	./makelib libwait.a wait_pid.o wait_nohang.o wait_stop.o \
	wait_snh.o

load: \
make-commands systype
	./make-commands "`cat ./systype`" load > load
	chmod 755 load

make-commands: \
make-cmds.sh conf-cc.sh warn-auto.sh
	cat warn-auto.sh conf-cc.sh make-cmds.sh > make-commands
	chmod 755 make-commands

makelib: \
make-commands systype
	./make-commands "`cat ./systype`" makelib > makelib
	chmod 755 makelib

nobuf: \
warn-auto.sh nobuf.sh home
	cat warn-auto.sh nobuf.sh | sed s}HOME}`./home`}g > nobuf
	chmod 755 nobuf

nobuf.0: \
nobuf.1
	nroff -man nobuf.1 > nobuf.0

open_append.o: \
compile open_append.c open_append.c open_append.c open.h \
open_append.c
	./compile open_append.c

open_excl.o: \
compile open_excl.c open_excl.c open_excl.c open.h open_excl.c
	./compile open_excl.c

open_read.o: \
compile open_read.c open_read.c open_read.c open.h open_read.c
	./compile open_read.c

open_trunc.o: \
compile open_trunc.c open_trunc.c open_trunc.c open.h open_trunc.c
	./compile open_trunc.c

open_write.o: \
compile open_write.c open_write.c open_write.c open.h open_write.c
	./compile open_write.c

printgids: \
load printgids.o libsubstdio.a liberror.a libstr.a libfs.a
	./load printgids libsubstdio.a liberror.a libstr.a libfs.a

printgids.o: \
compile printgids.c printgids.c printgids.c subfd.h printgids.c \
substdio.h printgids.c fmt.h printgids.c exit.h printgids.c \
conf-group.h printgids.c
	./compile printgids.c

ptybandage: \
warn-auto.sh ptybandage.sh home
	cat warn-auto.sh ptybandage.sh | sed s}HOME}`./home`}g > \
	ptybandage
	chmod 755 ptybandage

ptybandage.0: \
ptybandage.1
	nroff -man ptybandage.1 > ptybandage.0

ptyget: \
load ptyget.o fdcopy.o libenv.a liballoc.a libstrerr.a libsubstdio.a \
liberror.a libfs.a libstr.a
	./load ptyget fdcopy.o libenv.a liballoc.a libstrerr.a \
	libsubstdio.a liberror.a libfs.a libstr.a

ptyget-setup: \
load ptyget-setup.o libopen.a libstralloc.a liballoc.a libsubstdio.a \
liberror.a libstr.a
	./load ptyget-setup libopen.a libstralloc.a liballoc.a \
	libsubstdio.a liberror.a libstr.a

ptyget-setup.o: \
compile ptyget-setup.c ptyget-setup.c subfd.h ptyget-setup.c \
readwrite.h ptyget-setup.c substdio.h ptyget-setup.c stralloc.h \
gen_alloc.h stralloc.h ptyget-setup.c open.h ptyget-setup.c exit.h \
ptyget-setup.c error.h ptyget-setup.c conf-home.h ptyget-setup.c \
conf-secure.h ptyget-setup.c auto-gids.h ptyget-setup.c
	./compile ptyget-setup.c

ptyget.0: \
ptyget.8
	nroff -man ptyget.8 > ptyget.0

ptyget.o: \
compile ptyget.c ptyget.c ptyget.c ptyget.c ptyget.c ptyget.c env.h \
ptyget.c fmt.h ptyget.c str.h ptyget.c strerr.h ptyget.c ptyget.c \
hasptmx.h ptyget.c hasptc.h ptyget.c conf-secure.h ptyget.c ptyget.c \
ptyget.c
	./compile ptyget.c

ptyio: \
load ptyio.o ttyctrl.o ttymodes.o fdcopy.o libwait.a libsig.a \
libenv.a libgetopt.a libsubstdio.a liberror.a libstr.a
	./load ptyio ttyctrl.o ttymodes.o fdcopy.o libwait.a \
	libsig.a libenv.a libgetopt.a libsubstdio.a liberror.a \
	libstr.a

ptyio.0: \
ptyio.8
	nroff -man ptyio.8 > ptyio.0

ptyio.o: \
compile ptyio.c wait.h ptyio.c fork.h ptyio.c select.h select.h \
select.h select.h ptyio.c sig.h ptyio.c ptyio.c error.h ptyio.c \
readwrite.h ptyio.c exit.h ptyio.c ttyctrl.h ptyio.c ttymodes.h \
ttymodes.h ttymodes.h ptyio.c sgetopt.h subgetopt.h sgetopt.h ptyio.c \
substdio.h ptyio.c subfd.h ptyio.c
	./compile ptyio.c

ptyrun: \
warn-auto.sh ptyrun.sh home
	cat warn-auto.sh ptyrun.sh | sed s}HOME}`./home`}g > ptyrun
	chmod 755 ptyrun

ptyrun.0: \
ptyrun.1
	nroff -man ptyrun.1 > ptyrun.0

ptyspawn: \
load ptyspawn.o libwait.a libsig.a libenv.a libstrerr.a libgetopt.a \
libsubstdio.a liberror.a libstr.a
	./load ptyspawn libwait.a libsig.a libenv.a libstrerr.a \
	libgetopt.a libsubstdio.a liberror.a libstr.a

ptyspawn.0: \
ptyspawn.8
	nroff -man ptyspawn.8 > ptyspawn.0

ptyspawn.o: \
compile ptyspawn.c ptyspawn.c ptyspawn.c ptyspawn.c ptyspawn.c \
fdcopy.h ptyspawn.c sgetopt.h subgetopt.h sgetopt.h ptyspawn.c exit.h \
ptyspawn.c fork.h ptyspawn.c wait.h ptyspawn.c env.h ptyspawn.c \
error.h ptyspawn.c readwrite.h ptyspawn.c strerr.h ptyspawn.c sig.h \
ptyspawn.c ptyspawn.c ptyspawn.c
	./compile ptyspawn.c

select.h: \
compile trysysel.c select.h1 select.h2
	( ./compile trysysel.c >/dev/null 2>&1 \
	&& cat select.h2 || cat select.h1 ) > select.h
	rm -f trysysel.o trysysel

setup: \
it home ptyget-setup
	./ptyget-setup

sgetopt.o: \
compile sgetopt.c substdio.h sgetopt.c subfd.h sgetopt.c sgetopt.h \
sgetopt.h subgetopt.h sgetopt.h sgetopt.c subgetopt.h subgetopt.h \
sgetopt.c
	./compile sgetopt.c

shar: \
FILES BLURB README INSTALL TODO CHANGES FILES SYSDEPS VERSION \
Makefile ptyrun.1 ptyrun.sh ptybandage.1 ptybandage.sh nobuf.1 \
nobuf.sh ptyget.8 ptyget.c tryptmx.c tryptc.c ptyspawn.8 ptyspawn.c \
ptyio.8 ptyio.c ttydetach.1 ttydetach.c background.1 background.c \
exclon.1 exclon.c excloff.1 excloff.c ttymodes.h ttymodes.c ttyctrl.h \
ttyctrl.c home.c printgids.c ptyget-setup.c conf-home.h conf-secure.h \
conf-group.h conf-cc.sh find-systype.sh make-cmds.sh trycpp.c \
warn-auto.sh error.h error.c error_str.c error_temp.c getopt.3 \
getopt.h getopt.c sgetopt.3 sgetopt.h sgetopt.c subgetopt.3 \
subgetopt.h subgetopt.c substdio.h substdio.c substdi.c substdo.c \
substdio_copy.c subfd.h subfderr.c subfdout.c subfdin.c readwrite.h \
exit.h fork.h1 fork.h2 tryvfork.c byte.h byte_chr.c byte_copy.c \
byte_cr.c byte_diff.c byte_rchr.c byte_zero.c str.h str_chr.c \
str_cpy.c str_diff.c str_diffn.c str_len.c str_rchr.c sig.h \
sig_block.c sig_catch.c sig_cont.c sig_dfl.c sig_pipe.c trysgact.c \
trysgprm.c env.h env.c envread.c wait.h wait_pid.c wait_nohang.c \
wait_stop.c wait_snh.c trywaitp.c alloc.h alloc.c alloc_re.c fdcopy.h \
fdcopy.c select.h1 select.h2 trysysel.c strerr.h strerr.c \
strerr_sys.c strerr_die.c fmt.h fmt_str.c fmt_strn.c fmt_uint.c \
fmt_uint0.c fmt_ulong.c fmt_ushort.c gen_alloc.h gen_allocdefs.h \
stralloc.h stralloc_eady.c stralloc_pend.c stralloc_copy.c \
stralloc_opyb.c stralloc_opys.c stralloc_cat.c stralloc_catb.c \
stralloc_cats.c open.h open_append.c open_excl.c open_read.c \
open_trunc.c open_write.c
	shar -m `cat FILES` > shar
	chmod 400 shar

sig_block.o: \
compile sig_block.c sig_block.c sig.h sig_block.c hassgprm.h \
sig_block.c
	./compile sig_block.c

sig_catch.o: \
compile sig_catch.c sig_catch.c sig.h sig_catch.c hassgact.h \
sig_catch.c
	./compile sig_catch.c

sig_cont.o: \
compile sig_cont.c sig_cont.c sig.h sig_cont.c
	./compile sig_cont.c

sig_dfl.o: \
compile sig_dfl.c sig_dfl.c sig.h sig_dfl.c hassgact.h sig_dfl.c \
hassgprm.h sig_dfl.c
	./compile sig_dfl.c

sig_pipe.o: \
compile sig_pipe.c sig_pipe.c sig.h sig_pipe.c
	./compile sig_pipe.c

str_chr.o: \
compile str_chr.c str.h str_chr.c
	./compile str_chr.c

str_cpy.o: \
compile str_cpy.c str.h str_cpy.c
	./compile str_cpy.c

str_diff.o: \
compile str_diff.c str.h str_diff.c
	./compile str_diff.c

str_diffn.o: \
compile str_diffn.c str.h str_diffn.c
	./compile str_diffn.c

str_len.o: \
compile str_len.c str.h str_len.c
	./compile str_len.c

str_rchr.o: \
compile str_rchr.c str.h str_rchr.c
	./compile str_rchr.c

stralloc_cat.o: \
compile stralloc_cat.c byte.h stralloc_cat.c stralloc.h gen_alloc.h \
stralloc.h stralloc_cat.c
	./compile stralloc_cat.c

stralloc_catb.o: \
compile stralloc_catb.c stralloc.h gen_alloc.h stralloc.h \
stralloc_catb.c byte.h stralloc_catb.c
	./compile stralloc_catb.c

stralloc_cats.o: \
compile stralloc_cats.c byte.h stralloc_cats.c str.h stralloc_cats.c \
stralloc.h gen_alloc.h stralloc.h stralloc_cats.c
	./compile stralloc_cats.c

stralloc_copy.o: \
compile stralloc_copy.c byte.h stralloc_copy.c stralloc.h gen_alloc.h \
stralloc.h stralloc_copy.c
	./compile stralloc_copy.c

stralloc_eady.o: \
compile stralloc_eady.c alloc.h stralloc_eady.c stralloc.h \
gen_alloc.h stralloc.h stralloc_eady.c gen_allocdefs.h \
stralloc_eady.c
	./compile stralloc_eady.c

stralloc_opyb.o: \
compile stralloc_opyb.c stralloc.h gen_alloc.h stralloc.h \
stralloc_opyb.c byte.h stralloc_opyb.c
	./compile stralloc_opyb.c

stralloc_opys.o: \
compile stralloc_opys.c byte.h stralloc_opys.c str.h stralloc_opys.c \
stralloc.h gen_alloc.h stralloc.h stralloc_opys.c
	./compile stralloc_opys.c

stralloc_pend.o: \
compile stralloc_pend.c alloc.h stralloc_pend.c stralloc.h \
gen_alloc.h stralloc.h stralloc_pend.c gen_allocdefs.h \
stralloc_pend.c
	./compile stralloc_pend.c

strerr.o: \
compile strerr.c stralloc.h gen_alloc.h stralloc.h strerr.c strerr.h \
strerr.c
	./compile strerr.c

strerr_die.o: \
compile strerr_die.c substdio.h strerr_die.c subfd.h strerr_die.c \
exit.h strerr_die.c strerr.h strerr_die.c
	./compile strerr_die.c

strerr_sys.o: \
compile strerr_sys.c error.h strerr_sys.c strerr.h strerr_sys.c
	./compile strerr_sys.c

subfderr.o: \
compile subfderr.c readwrite.h subfderr.c substdio.h subfderr.c \
subfd.h subfderr.c
	./compile subfderr.c

subfdin.o: \
compile subfdin.c readwrite.h subfdin.c substdio.h subfdin.c subfd.h \
subfdin.c
	./compile subfdin.c

subfdout.o: \
compile subfdout.c readwrite.h subfdout.c substdio.h subfdout.c \
subfd.h subfdout.c
	./compile subfdout.c

subgetopt.o: \
compile subgetopt.c subgetopt.h subgetopt.h subgetopt.c
	./compile subgetopt.c

substdi.o: \
compile substdi.c substdio.h substdi.c byte.h substdi.c error.h \
substdi.c
	./compile substdi.c

substdio.o: \
compile substdio.c substdio.h substdio.c
	./compile substdio.c

substdio_copy.o: \
compile substdio_copy.c substdio.h substdio_copy.c
	./compile substdio_copy.c

substdo.o: \
compile substdo.c substdio.h substdo.c str.h substdo.c byte.h \
substdo.c error.h substdo.c
	./compile substdo.c

systype: \
find-systype trycpp.c
	./find-systype > systype

ttyctrl.o: \
compile ttyctrl.c ttyctrl.c ttyctrl.c ttyctrl.c ttyctrl.h ttyctrl.c
	./compile ttyctrl.c

ttydetach: \
load ttydetach.o ttyctrl.o libstrerr.a libsubstdio.a libenv.a \
liballoc.a liberror.a libstr.a
	./load ttydetach ttyctrl.o libstrerr.a libsubstdio.a \
	libenv.a liballoc.a liberror.a libstr.a

ttydetach.0: \
ttydetach.1
	nroff -man ttydetach.1 > ttydetach.0

ttydetach.o: \
compile ttydetach.c env.h ttydetach.c strerr.h ttydetach.c
	./compile ttydetach.c

ttymodes.o: \
compile ttymodes.c ttymodes.c ttymodes.c error.h ttymodes.c \
ttymodes.c ttymodes.h ttymodes.h ttymodes.h ttymodes.c
	./compile ttymodes.c

wait_nohang.o: \
compile wait_nohang.c wait_nohang.c wait_nohang.c haswaitp.h \
wait_nohang.c
	./compile wait_nohang.c

wait_pid.o: \
compile wait_pid.c wait_pid.c wait_pid.c error.h wait_pid.c
	./compile wait_pid.c

wait_snh.o: \
compile wait_snh.c wait_snh.c wait_snh.c haswaitp.h wait_snh.c
	./compile wait_snh.c

wait_stop.o: \
compile wait_stop.c wait_stop.c wait_stop.c haswaitp.h wait_stop.c \
error.h wait_stop.c
	./compile wait_stop.c
