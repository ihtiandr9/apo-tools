OS=$(uname -s)
if [ "$OS" == "FreeBSD" ]; then
  MAKE=gmake
else
  MAKE=make
fi
  $MAKE -j4 -f Build/Makefile.gnu $1
