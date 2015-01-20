objs="fdcopy.o libenv.a liballoc.a libstrerr.a libsubstdio.a liberror.a libfs.a libstr.a"
dependon load $1.o $objs
directtarget
./load $1 $objs
formake ./load $1 $objs
