#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "cons.h"

void calculateJuliaSet(uint8_t *pixels);

int main() {
  const char *file_path = "julia.ppm";
  int fd;
  uint8_t *pixels;

  // Create a new file or truncate an existing file to the desired size
  fd = open(file_path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("Error creating/opening file");
    exit(EXIT_FAILURE);
  }

  // Set the size of the file
  if (ftruncate(fd, FSIZE) == -1) {
    perror("Error setting file size");
    close(fd);
    exit(EXIT_FAILURE);
  }

  // Map the file into memory
  pixels = mmap(NULL, FSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (pixels == MAP_FAILED) {
    perror("Error mapping file into memory");
    close(fd);
    exit(EXIT_FAILURE);
  }

  memcpy(pixels, "P6\n1000 1000\n255\n", PPM_SIZE);

  calculateJuliaSet(pixels + PPM_SIZE);

  // Unmap the file when done
  if (munmap(pixels, FSIZE) == -1) {
    perror("Error unmapping file");
    close(fd);
    exit(EXIT_FAILURE);
  }

  // Close the file descriptor
  close(fd);

  return 0;
}

INLINE void calculateJuliaSet(uint8_t *pixels) {
  double a = A;
  double b = B;

  // Precalculate constants outside the loop
  double step_x = (XMAX - XMIN) / LINE;
  double step_y = (YMAX - YMIN) / COL;
  int idx = 0;

  for (int k = 0; k < SIZE; k += 3) {
    int i = 2;

    int col = idx % COL;
    int line = idx / LINE;

    double x = XMIN + col * step_x;
    double y = YMAX - line * step_y;

    while (i <= iterationmax && (x * x + y * y) <= 4) {
      double x_temp = x * x - y * y + a;
      y = 2 * x * y + b;
      x = x_temp;

      i++;
    }

    if (i > iterationmax && (x * x + y * y) <= 4) {
      pixels[k + 0] = 0;
      pixels[k + 1] = 0;
      pixels[k + 2] = 0;
    } else {

      pixels[k + 0] = (4 * i) % 256;
      pixels[k + 1] = 2 * i;
      pixels[k + 2] = (6 * i) % 256;
    }
    idx += 1;
  }
}
