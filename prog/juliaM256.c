#include <emmintrin.h>
#include <fcntl.h>
#include <immintrin.h>
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
  __m256d winx =
      _mm256_set_pd(XMIN, XMIN + step_x, XMIN + 2 * step_x, XMIN + 3 * step_x);
  __m256d winy =
      _mm256_set_pd(YMAX, YMAX - step_y, YMAX - 2 * step_y, YMAX - step_y * 3);
  __m128i wini = _mm_setr_epi32(4, 4, 4, 4);

  int idx = 3;
  for (int k = 0; k < SIZE; k += 3) {
    if (idx != 3) {
      int col = idx % COL;
      int line = idx / LINE;

      double x = XMIN + col * step_x;
      double y = YMAX - line * step_y;
      winx[0] = winx[1];
      winx[1] = winx[2];
      winx[2] = winx[3];
      winx[AVXD - 1] = x;

      winy[0] = winy[1];
      winy[1] = winy[2];
      winy[2] = winy[3];
      winy[AVXD - 1] = y;

      wini[0] = wini[1];
      wini[1] = wini[2];
      wini[2] = wini[3];
      wini[AVXD - 1] = 2;
    }
    idx += 1;

    // int i = wini[0];
    int i = 2;
    double x = winx[0];
    double y = winy[0];
    int new_i = 0;
    // printf("i: %d, x: %f, y: %f\n", i, x, y);
    while (i <= iterationmax && (x * x + y * y) <= 4) {
      // load
      __m256d x256 = winx;
      __m256d y256 = winy;

      // calculate y = (((x * y) * 2) + B);
      __m256d r1 = _mm256_mul_pd(x256, y256);
      __m256d r2 = _mm256_set1_pd(2.0);
      __m256d r3 = _mm256_mul_pd(r1, r2);
      __m256d b = _mm256_set1_pd(B);
      winy = _mm256_add_pd(r3, b);

      // calculate x
      __m256d x2 = _mm256_mul_pd(x256, x256);
      __m256d y2 = _mm256_mul_pd(y256, y256);

      __m256d r6 = _mm256_sub_pd(x2, y2);
      __m256d a = _mm256_set1_pd(A);
      __m256d r7 = _mm256_add_pd(r6, a);

      winx = r7;
      x = winx[0];
      y = winy[0];
      i++;
      new_i++;
    }
    // __m128i ri = _mm_setr_epi32(new_i, new_i, new_i, new_i);
    // wini = _mm_add_epi32(wini, ri);

    if (i > iterationmax && (x * x + y * y) <= 4) {
      pixels[k + 0] = 0;
      pixels[k + 1] = 0;
      pixels[k + 2] = 0;
    } else {

      pixels[k + 0] = (4 * i) % 256;
      pixels[k + 1] = 2 * i;
      pixels[k + 2] = (6 * i) % 256;
    }
  }
}
