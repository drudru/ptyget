dependon compile trysysel.c select.h1 select.h2
./compile trysysel.c >/dev/null 2>&1 && cat select.h2 || cat select.h1
rm -f trysysel.o
formake '( ./compile trysysel.c >/dev/null 2>&1 \'
formake '&& cat select.h2 || cat select.h1 ) > select.h'
formake 'rm -f trysysel.o trysysel'
