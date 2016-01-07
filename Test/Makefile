#
# test for library
# these tests all need to run on a propeller board
#
# build targets available:
#  default:   test installed LMM library (override with MODEL=cmm)
#  testall:   test all installed memory models
#  testbuild: test recently built LMM library
#  testbuildall: test all recently built libraries
#  clean:     delete temporary files
#

#MODEL = xmmc
#MODEL = cmm
MODEL = lmm

CC = propeller-elf-gcc
CFLAGS = -m$(MODEL) -std=c99 -Os -Wall -u __serial_exit
LMMCFLAGS = -mlmm -std=c99 -Os -Wall -u __serial_exit

ifeq ($(TEST),build)
  BUILD=../../../build/lib/obj/$(MODEL)
  LIB=$(BUILD)/libc.a
  LIB_MC=$(BUILD)/libm.a $(BUILD)/libc.a
  LIBPTHREAD=$(BUILD)/libpthread.a $(BUILD)/libc.a
  LIBLMM=../../../build/lib/obj/lmm/libc.a
else
  LIBPTHREAD=-lpthread
  LIB_MC=-lm
endif

default: time.run tests
	make clean

tests: strtod.run scanf.run sprintf.run strtol.run wstring.run ctype.run

testall:
	make time.run
	make clean
	make MODEL=cmm tests
	make clean
	make MODEL=lmm tests
	make clean
	make MODEL=xmmc tests
	make clean

testbuild:
	make TEST=build
	make clean

testbuildall:
	make TEST=build testall
	make clean

#
# note: the timing functions in time.elf are quite sensitive, so run them
# in lmm mode
#
time.elf: time.c
	$(CC) $(LMMCFLAGS) -o $@ $^ $(LIBLMM)

rtc.elf: rtc.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

ctype.elf: ctype.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

wstring.elf: wstring.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

strtol.elf: strtol.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

strtod.elf: strtod.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

sprintf.elf: sprintf.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIB_MC)

scanf.elf: scanf.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

tmpfile.elf: tmpfile.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)


mutex.elf: mutex.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBPTHREAD)

%.run: %.elf
	propeller-load -bc3 $^ -r -t -q

clean:
	rm -rf *.o *.elf
