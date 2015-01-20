case "$1" in
  shar)
    dependon FILES `cat FILES`
    formake 'shar -m `cat FILES` > shar'
    formake 'chmod 400 shar'
    shar -m `cat FILES`
    chmod 400 $3
    ;;
  makelib|loaddns|load|compile)
    dependon make-commands systype
    formake './make-commands "`cat ./systype`"' $1 '>' $1
    formake 'chmod 755' $1
    ./make-commands "`cat ./systype`" $1
    chmod 755 $3
    ;;
  make-commands)
    dependon make-cmds.sh conf-cc.sh warn-auto.sh
    formake 'cat warn-auto.sh conf-cc.sh make-cmds.sh > make-commands'
    formake 'chmod 755 make-commands'
    cat warn-auto.sh conf-cc.sh make-cmds.sh
    chmod 755 $3
    ;;
  find-systype)
    dependon find-systype.sh conf-cc.sh warn-auto.sh
    formake 'cat warn-auto.sh conf-cc.sh find-systype.sh > find-systype'
    formake 'chmod 755 find-systype'
    cat warn-auto.sh conf-cc.sh find-systype.sh
    chmod 755 $3
    ;;
  systype)
    dependon find-systype trycpp.c
    formake './find-systype > systype'
    ./find-systype
    ;;
  *)
    nosuchtarget
    ;;
esac
