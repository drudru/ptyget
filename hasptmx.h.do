dependon tryptmx.c compile load
( ./compile tryptmx.c && ./load tryptmx && ./tryptmx ) >/dev/null 2>&1 \
&& echo \#define HASPTMX 1
rm -f tryptmx.o tryptmx
formake '( ( ./compile tryptmx.c && ./load tryptmx && ./tryptmx ) >/dev/null 2>&1 \'
formake '&& echo \#define HASPTMX 1 || exit 0 ) > hasptmx.h'
formake 'rm -f tryptmx.o tryptmx'
