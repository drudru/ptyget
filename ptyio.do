objs="ttyctrl.o ttymodes.o fdcopy.o libwait.a libsig.a libenv.a libgetopt.a libsubstdio.a liberror.a libstr.a"
dependon load $1.o $objs
directtarget
./load $1 $objs
formake ./load $1 $objs
