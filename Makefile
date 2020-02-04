CC=gcc
DEBUG=0

CFLAGS=-Wall -Wextra -Wpedantic --std=c11 -fPIC

ifeq ($(DEBUG),0)
	CFLAGS+=-O3
else
	CFLAGS+=-g
endif

LFLAGS=
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))

.PHONY: all clean run_%

all: libpictools.so libpictools.a

run_%: test_%
	./$<
	xdg-open image.png

test_%: test/%.c libpictools.a
	$(CC) $(CFLAGS) $^ -lm -o $@

libpictools.so: $(OBJ)
	$(CC) $(LFLAGS) -shared -lm $^ -o $@

libpictools.a: $(OBJ)
	rm -f $@
	$(AR) -cvq $@ $^

build/%.o: src/%.c src/%.h | build/
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

tags: $(SRC)
	ctags --extras=rq $(SRC)

build/:
	mkdir -p build

clean:
	rm -rf build
	rm -f test_*

wipe: clean
	rm -f *.png
	rm -f run_test
	rm -f tags
	rm -f *.so
	rm -f *.a
