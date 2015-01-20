dependon trywaitp.c compile load
( ./compile trywaitp.c && ./load trywaitp ) >/dev/null 2>&1 \
&& echo \#define HASWAITPID 1
rm -f trywaitp.o trywaitp
formake '( ( ./compile trywaitp.c && ./load trywaitp ) >/dev/null 2>&1 \'
formake '&& echo \#define HASWAITPID 1 || exit 0 ) > haswaitp.h'
formake 'rm -f trywaitp.o trywaitp'
