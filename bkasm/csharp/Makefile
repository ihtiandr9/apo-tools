CSC = mcs
CFLAGS = -out:bkasm.exe
SRCS = $(wildcard *.cs)
GUI_SRCS = $(wildcard gui/*.cs)

all: bkasm.exe

bkasm.exe: $(SRCS)
	$(CSC) $(CFLAGS) $(SRCS)

gui/bkasm_gui.exe: bkasm.exe $(GUI_SRCS)
	$(CSC) -out:gui/bkasm_gui.exe -r:System.Windows.Forms -r:System.Drawing $(GUI_SRCS)

run: bkasm.exe
	mono bkasm.exe $(ARGS)

run_gui: gui/bkasm_gui.exe
	mono gui/bkasm_gui.exe

gui: gui/bkasm_gui.exe

test: bkasm.exe
	python3 tests/test.py

unittest: bkasm.exe
	mono bkasm.exe ../tests/test.asm

clean:
	rm -f bkasm.exe prog.bin gui/bkasm_gui.exe

.PHONY: all run gui run_gui test unittest clean
