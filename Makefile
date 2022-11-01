EXE 		= main
SCENE 		= final_scene

SRCDIR 		= src
TESTDIR 	= tests
BINDIR 		= ../../bin
TESTBINDIR 	= ../../bin/tests
OBJDIR 		= ../../build/$(EXE)

TARGET 		= $(BINDIR)/$(EXE)
SRC    		= $(wildcard $(SRCDIR)/*.c)
OBJ    		= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

TESTTARGETS = $(notdir $(basename $(TESTOBJ)))
TESTS 		= $(wildcard $(TESTDIR)/*.c)
TESTOBJ 	= $(TESTS:$(TESTDIR)/%.c=$(OBJDIR)/%.o)
TESTDEPS 	= $(filter-out %$(EXE).o, $(OBJ))

CC     		= gcc
CFLAGS 		= -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic 

.PHONY: all tests clean

all: $(BINDIR) $(OBJDIR) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -lm -lpthread -o $@
	cp $(SRCDIR)/$(SCENE) $(BINDIR)/$(SCENE)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(TESTBINDIR) $(BINDIR) $(OBJDIR) $(TESTTARGETS)

$(TESTTARGETS): $(OBJ) | $(TESTOBJ)
	$(CC) $(TESTDEPS) $(OBJDIR)/$@.o -o $(TESTBINDIR)/$@

$(OBJDIR)/%.o: $(TESTDIR)/%.c | $(TARGET)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR) $(OBJDIR) $(TESTBINDIR):
	mkdir -p $@

clean:
	rm -r ../../bin
	rm -r ../../build
