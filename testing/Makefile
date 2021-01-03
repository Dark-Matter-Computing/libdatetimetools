# -*- Makefile -*-
# Supported Architectures: Various
#
# Using GNC C compiler [Linux and Unix], and Clang [macOS]
#
# By Thomas H. Vidal
# 

project = test_datetimetools
dependency_1 = datetools
dependency_2 = timetools
dependency_3 = testsuite

## Source Tree
SOURCEDIR = .
LIBSRC = ../src
BUILDDIR = ./build
BINDIR = ./bin
LIBDIR = /Volumes/development/products/libdatetimetools/lib

## Modules
TESTPRJ = $(BUILDDIR)/$(project).o
TESTSUITE = $(BUILDDIR)/testsuite.o

## Determine OS and Architecture

OSFLAG		:=
ifeq ($(OS),Windows_NT)
	OSFLAG += -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		OSFLAG += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),X86)
		OSFLAG += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG += -D LINUX
		target = $(project)_linux
		CC = gcc
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG += -D macOS
		target = $(project)
		cc = gcc
	endif

	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),X86_64)
		OSFLAG += -D AMD64
	endif
	ifeq ($(filter %86,$(UNAME_P)),)
		OSFLAG += -D IA32
	endif
	ifeq ($(filter arm%,$(UNAME_P)),)
		OSFLAG += -D ARM
	endif
endif

# Set Build Flags

CFLAGS= -Wall -Wextra -pedantic -D__USE_FIXED_PROTOTYPES__ -std=c89

CFLAGS2= -W -Wundef -Wstrict-prototypes -Wmissing-prototypes \
		 -Wmissing-declarations -Wcast-qual -Wwrite-strings

PROFFLAG= -pg

DEBUGFLAG= -g

LDFLAGS = -L$(LIBDIR)

LDLIBS = -ldatetimetools

#
# Compiler Flags
# 	-g 	-- Enable Debugging
# 	-Wall	-- Turn on all warnings
# 	-Wextra -- 
# 	-pedantic
# 	-D__USE_FIXED_PROTOTYPES__
# 		-- Force the compiler to use the correct headers
#	-ansi	-- Don't use the GNU extensions.  Stick to ANSI C.
#	-std=c99
#
# Compiler Flags 2 -- Used to emulate lint-like static analysis
# 	-W
# 	-Wundef
# 	-Wstrict-prototypes
# 	-Wmissing-prototypes
# 	-Wmissing-declarations
# 	-Wcast-qual
# 	-Wwrite-strings


# Primary Build Targets

build: $(BUILDDIR)/$(target).o $(BUILDDIR)/$(dependency_1).o \
	   $(BUILDDIR)/$(dependency_2).o $(BUILDDIR)/$(dependency_3).o

	$(CC) $(CFLAGS) $(CFLAGS2) -o $(BINDIR)/$(target) $(BUILDDIR)/$(target).o $(BUILDDIR)/$(dependency_1).o $(BUILDDIR)/$(dependency_2).o $(BUILDDIR)/$(dependency_3).o -lm
	
# instead of using the macro PROGNAME, I could use the built-in macro
# "$@". $@ = the name before the colon on the target line.  ("$<" is the
# built-in macro to access the first dependency (files listed AFTER the colon).
# I chose NOT to use the built-in because I used the phony target "build" and I
# don't want the executable being named "build."  I used build, so that I could
# define a target called rebuild, that would call clean and then call build!

$(BUILDDIR)/$(target).o: $(SOURCEDIR)/$(project).c
	$(CC) $(CFLAGS) $(CFLAGS2) -c -o $(BUILDDIR)/$(target).o $(SOURCEDIR)/$(project).c

$(BUILDDIR)/$(dependency_1).o: $(LIBSRC)/$(dependency_1).c
	$(CC) $(CFLAGS) $(CFLAGS2) -c -o $(BUILDDIR)/$(dependency_1).o $(LIBSRC)/$(dependency_1).c
	
$(BUILDDIR)/$(dependency_2).o: $(LIBSRC)/$(dependency_2).c
	$(CC) $(CFLAGS) $(CFLAGS2) -c -o $(BUILDDIR)/$(dependency_2).o $(LIBSRC)/$(dependency_2).c

$(BUILDDIR)/$(dependency_3).o: $(SOURCEDIR)/$(dependency_3).c
	$(CC) $(CFLAGS) $(CFLAGS2) -c -o $(BUILDDIR)/$(dependency_3).o $(SOURCEDIR)/$(dependency_3).c
	#
# Special Targets
# Build target to get the assembly language output - delete if not wanted
assembler_out: $(BUILDDIR)/$(target).o
	$(CC) -o2 -S -c -o $(BUILDDIR)/$(target).s $(SOURCEDIR)/$(project).c

# Debugging
debug: CFLAGS += $(DEBUGFLAG)
debug: clean build
	
# Profiling
profile: CFLAGS += $(PROFFLAG)
profile: clean build

# Admin Targets
rebuild: clean build

clean:
	rm -f $(BUILDDIR)/$(target).o
	rm -f $(BUILDDIR)/$(dependency_1).o
	rm -f $(BINDIR)/$(target)

variable_test:
	@echo $(OSFLAG)
	@echo $(target)
	@echo $(CC)
	@echo "$(project).c"

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
