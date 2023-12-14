// MIT License
//
// Copyright (c) 2023 Floréal Risso <floreal.risso@univ-tlse3.fr>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
  memcpy(pixels, PPM_HEAD, PPM_SIZE);

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
  int idx = 0;

  for (int k = 0; k < SIZE; k += 3) {
    int col = idx % COL;
    int line = idx / LINE;

    double x = XMIN + col * XSTEP;
    double y = YMAX - line * YSTEP;

    int i = 2;
    while (i <= MAXITER) {
      double x2 = x * x;
      double y2 = y * y;

      if ((x2 + y2) > 4.0) {
        break;
      }

      y = 2.0 * x * y + B;
      x = x2 - y2 + A;

      i++;
    }

    if (i > MAXITER) {
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
