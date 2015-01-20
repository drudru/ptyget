dependon tryptc.c compile load
( ./compile tryptc.c && ./load tryptc && ./tryptc ) >/dev/null 2>&1 \
&& echo \#define HASPTC 1
rm -f tryptc.o tryptc
formake '( ( ./compile tryptc.c && ./load tryptc && ./tryptc ) >/dev/null 2>&1 \'
formake '&& echo \#define HASPTC 1 || exit 0 ) > hasptc.h'
formake 'rm -f tryptc.o tryptc'
