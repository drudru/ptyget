dependon trysgprm.c compile load
( ./compile trysgprm.c && ./load trysgprm ) >/dev/null 2>&1 \
&& echo \#define HASSIGPROCMASK 1
rm -f trysgprm.o trysgprm
formake '( ( ./compile trysgprm.c && ./load trysgprm ) >/dev/null 2>&1 \'
formake '&& echo \#define HASSIGPROCMASK 1 || exit 0 ) > hassgprm.h'
formake 'rm -f trysgprm.o trysgprm'
