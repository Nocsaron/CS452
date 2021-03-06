#This is a sample Makefile for Phase 1. Update the variables below as appropriate. It
# provides the following targets (assume test source files are named testN.c in this
# description).
#
#	make		(makes libphase1.a and all tests)
#
#	make testN 	(makes testN)
#	make testN.out	(runs testN and puts output in testN.out)
#	make tests	(makes all testN.out files, i.e. runs all tests)
#
#	make testN.v	(runs valgrind on testN and puts output in testN.v)
#	make valgrind	(makes all testN.v files, i.e. runs valgrind on all tests)
#
#	make clean	(removes all files created by this Makefile)

# Change the following if your 452-students directory is in a non-standard location.
452-STUDENTS = /home/posideon/Dropbox/452-students

# Set to version of USLOSS you want to use.
USLOSS_VERSION = 2.9

# Add your phase1 source files to the following. Do not include your test files.
SRCS = linked_list.c phase1.c

# Add any tests here. If the test is named foo then the source file is assumed to be foo.c.
TESTS = test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 test13 test14 test15 test16 test17 test18

# Change this if you want to change the arguments to valgrind.
VGFLAGS = --track-origins=yes --leak-check=full --max-stackframe=100000

# Change this if you need to link against additional libraries (probably not).
LIBS = -lphase1 -lusloss$(USLOSS_VERSION)

# Change this if you want change which flags are passed to the C compiler.
CFLAGS += -Wall -g 

# You shouldn't need to change anything below here. 

TARGET = libphase1.a

INCLUDES = -I$(452-STUDENTS)/include -I$(452-STUDENTS)/usloss/include

ifeq ($(shell uname),Darwin)
	DEFINES += -D_XOPEN_SOURCE
	OS = macosx
else
	OS = linux
endif


CC=gcc
LD=gcc
AR=ar    
CFLAGS += $(INCLUDES) $(DEFINES)
LDFLAGS = -L. -L $(452-STUDENTS)/usloss/lib/$(OS)
COBJS = ${SRCS:.c=.o}
DEPS = ${COBJS:.o=.d}
TSRCS = {$TESTS:=.c}
TOBJS = ${TESTS:=.o}
TOUTS = ${TESTS:=.out}
TVS = ${TESTS:=.v}

# The following is to deal with circular dependencies between the USLOSS and phase1
# libraries. Unfortunately the linkers handle this differently on the two OSes.

ifeq ($(OS), macosx)
	LIBFLAGS = -Wl,-all_load $(LIBS)
else
	LIBFLAGS = -Wl,--start-group $(LIBS) -Wl,--end-group
endif

%.d: %.c
	$(CC) -c $(CFLAGS) -MM -MF $@ $<

all: $(TARGET) $(TESTS)

$(TARGET):  $(COBJS)
	$(AR) -r $@ $^

tests: $(TOUTS)

%.out: %
	./$< 1> $@ 2>&1

$(TESTS):   %: $(TARGET) %.o
	$(LD) $(LDFLAGS) -o $@ $@.o $(LIBFLAGS)

clean:
	rm -f $(COBJS) $(TARGET) $(TOBJS) $(TESTS) $(DEPS) $(TVS)
	rm -fr *.d
	rm -f *.out
	clear

%.d: %.c
	$(CC) -c $(CFLAGS) -MM -MF $@ $<

valgrind: $(TVS)

%.v: %
	valgrind $(VGFLAGS) ./$< 1> $@ 2>&1

-include $(DEPS)
