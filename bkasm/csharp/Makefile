CSC = mcs
CFLAGS = -out:bkasm.exe
SRCS = $(wildcard *.cs)
GUI_SRCS = $(wildcard gui/*.cs)

all: bkasm.exe

bkasm.exe: $(SRCS)
	$(CSC) $(CFLAGS) $(SRCS)

run: bkasm.exe
	mono bkasm.exe $(ARGS)

test: bkasm.exe
	python3 tests/test.py

unittest: bkasm.exe
	mono bkasm.exe ../tests/test.asm

clean:
	rm -f bkasm.exe prog.bin

.PHONY: all run test unittest clean
