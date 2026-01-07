OS=$(uname -s)
if [ "$OS" == "FreeBSD" ]; then
  MAKE=gmake
else
  MAKE=make
fi
  $MAKE -f Makefile.gnu $1