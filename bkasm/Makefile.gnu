
ifndef TERMUX
	ifeq ($(OS),Windows_NT)
		DETECTED_OS :=Windows
	else
		DETECTED_OS := $(shell uname -s)
	endif

	ifeq ($(DETECTED_OS),FreeBSD)
		PREFIX:= /usr/local
		LIBDIRS:=-L,$(PREFIX)/lib,-lc
	else
		ifeq ($(DETECTED_OS),Windows)
			PREFIX:= tools/MinGW64
			LIBDIRS:=-L,$(PREFIX)/lib32,-L,$(PREFIX)/x86_64-w64-mingw32/lib32,-lgcc -m32
		else
			PREFIX= /usr
			LIBDIRS:=-L,$(PREFIX)/lib,-lc
		endif
	endif
endif 

APP:=bkasm
OUTDIR:=build
CC=$(PREFIX)/bin/gcc

## -static-libgcc -m32

CFLAGS= -Isrc -I$(PREFIX)/include
ifeq ($(DETECTED_OS),Windows)
CFLAGS+= -m32
endif

SRCS=\
	 $(wildcard src/*.c)

OBJS=\
	 $(SRCS:src/%.c=$(OUTDIR)/%.o)

HDRS=\
	 $(wildcard src/*.h)

.PHONY: all clean
all: $(OUTDIR)/$(APP)

$(OUTDIR)/$(APP): $(OBJS) $(OUTDIR)
	$(CC) $(OBJS) -o $@ -Wl,$(LIBDIRS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/%.o: src/%.c $(HDRS) | $(OUTDIR)
	@echo $< "==>" $@
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OUTDIR)/%.S: src/%.c $(HDRS) | $(OUTDIR)
	@echo $< "==>" $@
	@$(CC) $(CFLAGS) -o $@ -S $<

clean:
	@echo cleaning $(OUTDIR) dir
	@-rm -rf $(OUTDIR)
## if(NOT DEFINED _Release_)
## add_definitions(-ggdb -Wno-switch)
## endif()

## debug target

## add_custom_target(debug
##    COMMAND	gdb ./bkasm
##    COMMENT "GDB Debug"
##    DEPENDS ./bkasm
## 

## unittest tests one unit

## add_custom_target(unittest
##     COMMAND	./bkasm ../tests/test.asm
##     COMMENT "Run Test"
##     DEPENDS ./bkasm
## )

## script unittests

## add_custom_target(run_tests
##     COMMAND	../tests/python/bin/python3 ../tests/tests.py
##     COMMENT "Run Tests"
##     DEPENDS ./bkasm
## )

## valgrind test

## add_custom_target(valgrind
## #    COMMAND	valgrind -s --leak-check=full ./bkasm ../tests/test.asm
##    COMMAND	valgrind -s ./bkasm ../tests/test.asm
##    COMMENT "Run Memory tests"
##    DEPENDS ./bkasm
## 

## install(TARGETS bkasm DESTINATION /usr/bin)
