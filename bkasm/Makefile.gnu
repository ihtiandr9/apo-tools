
ifndef TERMUX
	ifeq ($(OS),Windows_NT)
		DETECTED_OS := Windows
	else
		DETECTED_OS := $(shell uname -s)
	endif

	ifeq ($(DETECTED_OS),FreeBSD)
		PREFIX= /usr/local
	else
		PREFIX= /usr
		DETECTED_OS := $(shell uname -s)
	endif
endif 

APP:=bkasm
OUTDIR:=build
CC=$(PREFIX)/bin/gcc
LIBDIR:=$(PREFIX)/lib
CFLAGS= -Isrc -I$(PREFIX)/include

## -I/usr/include

SRCS=\
	 $(wildcard src/*.c)

OBJS=\
	 $(SRCS:src/%.c=$(OUTDIR)/%.o)

HDRS=\
	 $(wildcard src/*.h)

.PHONY: all clean
all: $(OUTDIR)/$(APP)

$(OUTDIR)/$(APP): $(OBJS) $(OUTDIR)
	$(CC) -Wl,-L,$(LIBDIR) -o $@ $(OBJS) -Wl,-lc

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
