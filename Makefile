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

.PHONY: all clean run

all: libpictools.so libpictools.a

run: run_test
	LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./run_test
	xdg-open ./image.png &

debug: run_test
	gdb ./run_test

run_test: test/test.c libpictools.so
	$(CC) $(CFLAGS) -lpictools -L./ $(LFLAGS) $^ -lm -o $@

libpictools.so: $(OBJ)
	$(CC) $(LFLAGS) -shared -lm $^ -o $@

libpictools.a: $(OBJ)
	$(AR) -cvq $@ $^

build/%.o: src/%.c src/%.h | build/
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

tags: $(SRC)
	ctags --extras=rq $(SRC)

build/:
	mkdir -p build

clean:
	rm -rf build

wipe: clean
	rm -f *.png
	rm -f run_test
	rm -f tags
	rm -f *.so
