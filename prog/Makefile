CC=gcc
CFLAGS=-O3 -march=native -mavx -fno-stack-protector -Wno-attributes

all:
	mkdir -p build
	## $(CC) $(CFLAGS) julia.c -o build/julia
	$(CC) $(CFLAGS) juliaM256.c -o build/juliaM256
	$(CC) $(CFLAGS) juliaM256f.c -o build/juliaM256f
	$(CC) $(CFLAGS) -fopenmp juliaM256f.c -o build/juliaM256f
	$(CC) $(CFLAGS) -fopenmp juliaM256fomp.c -o build/juliaM256fomp
	# $(CC) $(CFLAGS) juliaM512f.c -o build/juliaM512f -mavx512f
	# $(CC) $(CFLAGS) -fopenmp juliaM512fomp.c -o build/juliaM512fomp -mavx512f
	$(CC) $(CFLAGS) juliaRow.c -o build/juliaRow
	$(CC) $(CFLAGS) juliaMap.c -o build/juliaMap


clean:
	rm -f build

format:

