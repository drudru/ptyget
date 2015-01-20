objs=""
dependon load $1.o $objs
directtarget
./load $1 $objs
formake ./load $1 $objs
