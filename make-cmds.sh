AR='ar cr'
RANLIB=ranlib
LIBS=''
DNSLIBS=''

case "$1" in
sunos-5.*)
  # we can survive without /usr/ucbinclude and /usr/ucblib
  LIBS='-lsocket -lnsl'
  DNSLIBS=-lresolv
  RANLIB=:
  ;;
unix_sv*)
  LIBS='-lsocket -lnsl'
  DNSLIBS=-lresolv
  RANLIB=:
  ;;
sunos-4.*)
  DNSLIBS=-lresolv
  ;;
irix-*)
  RANLIB=:
  ;;
hp-ux-*)
  RANLIB=:
  ;;
sco*)
  LIBS='-lsocket -lnsl'
  RANLIB=:
  ;;
aix-*)
  LIBS=-lbsd # appears to not screw up readdir(); ibm did something right!
  ;;
nextstep-*)
  ;;
esac

echo '#!/bin/sh'
case "$2" in
compile)
  echo exec "$CC" -c '${1+"$@"}'
  ;;
load)
  echo 'main="$1"; shift'
  echo exec "$LD" '-o "$main" "$main".o ${1+"$@"}' "$LIBS"
  ;;
loaddns)
  echo 'main="$1"; shift'
  echo exec "$LD" '-o "$main" "$main".o ${1+"$@"}' "$DNSLIBS" "$LIBS"
  ;;
makelib)
  echo 'main="$1"; shift'
  echo 'rm -f "$main"'
  echo "$AR" '"$main" ${1+"$@"}'
  echo "$RANLIB" '"$main"'
  ;;
esac
