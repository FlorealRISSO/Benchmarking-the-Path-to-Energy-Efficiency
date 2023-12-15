CC=gcc
CFLAGS=-O3 -march=native -mavx -fno-stack-protector

all:
	$(CC) $(CFLAGS) julia.c -o build/julia
	$(CC) $(CFLAGS) julia256.c -o build/julia256
	$(CC) $(CFLAGS) juliaM256.c -o build/juliaM256
	$(CC) $(CFLAGS) juliaRow.c -o build/juliaRow
	$(CC) $(CFLAGS) juliaMap.c -o build/juliaMap
	$(CC) $(CFLAGS) juliaTest.c -o build/juliaTest


clean:
	rm -f build/julia build/julia256 build/juliaM256 build/juliaRow build/juliaMap

format:

