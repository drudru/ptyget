if test -r "$2.use"
then
  dependon "$2.use"
  dependon makelib `cat "$2.use"`
  directtarget
  formake ./makelib $1 `cat "$2.use"`
  ./makelib $1 `cat "$2.use"`
else
  nosuchtarget
fi
