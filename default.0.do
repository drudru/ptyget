case $2 in
  ptybandage|ptyrun|nobuf|ttydetach|exclon|excloff|background)
    dependon $2.1
    nroff -man $2.1
    formake nroff -man $2.1 '>' $1
    ;;
  ptyget|ptyspawn|ptyio)
    dependon $2.8
    nroff -man $2.8
    formake nroff -man $2.8 '>' $1
    ;;
  *)
    nosuchtarget
    ;;
esac
