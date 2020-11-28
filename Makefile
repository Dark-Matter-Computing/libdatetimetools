# -*- Makefile -*-
# To build a static library from a project.
# Supported Architectures: Various
#
# Using GNC C compiler [Linux and Unix], and Clang [macOS]
#
#
# By Thomas H. Vidal
# 

project = libdatetimetools

## Source Tree
SOURCEDIR = ./src
OBJDIR = ./obj
# OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c))
SOURCES = $(sort $(shell find . -name '*.c'))
OBJECTS = $(sort $(notdir $(patsubst %.c,%.o,$(SOURCES))))
LIBDIR = ./lib

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
		#cc = clang
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

CFLAGS=-g -Wall -Wextra -pedantic -D__USE_FIXED_PROTOTYPES__ -std=c99 \
	   -pedantic -W -Wundef -Wstrict-prototypes -Wmissing-prototypes \
	   -Wmissing-declarations -Wcast-qual -Wwrite-strings

CPPFLAGS= -Weffc++

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
# 	-pedantic
# 	-W
# 	-Wundef
# 	-Wstrict-prototypes
# 	-Wmissing-prototypes
# 	-Wmissing-declarations
# 	-Wcast-qual
# 	-Wwrite-strings
#
# CPPFLAGS --
# 	-Weffc++

# Primary Build Targets
build: $(OBJECTS)
	ar -rc $(LIBDIR)/$(target).a $(OBJDIR)/$<

$(OBJECTS): $(SOURCES)
	@$(COMPILE.c) -c -o $(OBJDIR)/$@ $<
	
# instead of using the macro PROGNAME, I could use the built-in macro
# "$@". $@ = the name before the colon on the target line.  ("$<" is the
# built-in macro to access the first dependency (files listed AFTER the colon).
# I chose NOT to use the built-in because I used the phony target "build" and I
# don't want the executable being named "build."  I used build, so that I could
# define a target called rebuild, that would call clean and then call build!

# Special Targets
# Build target to get the assembly language output - delete if not wanted
assembler_out: $(OBJDIR)/%.o 
	$(CC) -o2 -S -c -o $(OBJDIR)/$@.s $(SOURCEDIR)/$<

# Admin Targets
rebuild: clean build

clean:
	rm -f $(OBJDIR)/*.o $(LIBDIR)/$(target).a 

rebuild: clean build

variable_test:
	@echo $(OSFLAG)
	@echo $(target)
	@echo $(CC)
	@echo "$(project).a"
	@echo $(SOURCES)
	@echo $(OBJECTS)

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
