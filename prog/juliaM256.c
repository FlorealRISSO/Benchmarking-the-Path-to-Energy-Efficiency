#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <emmintrin.h>
#include <immintrin.h>

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
  // Constants
  const __m256d xmin = _mm256_set1_pd(XMIN);
  const __m256d ymax = _mm256_set1_pd(YMAX);
  const __m256d xstep = _mm256_set1_pd(XSTEP);
  const __m256d ystep = _mm256_set1_pd(YSTEP);
  const __m256d max_iter = _mm256_set1_pd(MAXITER);
  const __m256d threshold = _mm256_set1_pd(4.0);
  const __m256d one = _mm256_set1_pd(1.0);
  const __m256d a = _mm256_set1_pd(A);
  const __m256d b = _mm256_set1_pd(B);


  int ti = 0;
  for (int i = 0; i < SIZE; i += (4 * COLORS)) {
    int col = ti % COL;
    int line = ti / LINE;
    ti += 4;

    // XMIN + col * step_x
    __m256d x = _mm256_set_pd(
      col + 3,
      col + 2,
      col + 1,
      col + 0
    );
    x = _mm256_mul_pd(x, xstep);
    x = _mm256_add_pd(x, xmin);

    // YMIN - line * step_y
    __m256d y = _mm256_set_pd(
      line,
      line,
      line,
      line
    );
    y = _mm256_mul_pd(y, ystep);
    y = _mm256_sub_pd(ymax, y);

    __m256d zx = x;
    __m256d zy = y;

    __m256d iter = _mm256_set1_pd(2);

    __m256d mask = _mm256_set1_pd(NAN);
    for (int j = 2; j <= MAXITER; j++) {

      /* LOOP CONDITION */
      __m256d x2 = _mm256_mul_pd(zx, zx);
      __m256d y2 = _mm256_mul_pd(zy, zy);
      // Check if |x2 + y2| < 4.0
      __m256d sum = _mm256_add_pd(x2, y2);
      __m256d zmask = _mm256_cmp_pd(sum,
                                    threshold, _CMP_LE_OQ);

      mask = _mm256_and_pd(mask, zmask);

      // Check if all pixels have converged
      if (_mm256_testz_pd(mask, _mm256_set1_pd(NAN))) {
        break;
      }
      /* END LOOP CONDITION */

      /* LOOP BODY */
      // Update iteration count
      __m256d incr = _mm256_and_pd(mask, one);
      iter = _mm256_add_pd(iter, incr);

      // y = 2 * x * y + b
      zy = _mm256_mul_pd(zx, zy);
      zy = _mm256_mul_pd(zy, _mm256_set1_pd(2.0));
      zy = _mm256_add_pd(zy, b);

      // x = x * x - y * y + a
      zx = _mm256_sub_pd(x2, y2);
      zx = _mm256_add_pd(zx, a);
      /* END LOOP BODY */
    }


    __m128i _iter = _mm256_cvtpd_epi32(iter);
    int *tidx = (int*) &_iter;
    for (int j = 0; j < 4; j++) {
      int idx = tidx[j];
      int k = j * 3;
      if (idx > MAXITER) {
        pixels[i + k + 0] = 0;
        pixels[i + k + 1] = 0;
        pixels[i + k + 2] = 0;
      } else {
        pixels[i + k + 0] = (4 * idx) % 256;
        pixels[i + k + 1] = 2 * idx;
        pixels[i + k + 2] = (6 * idx) % 256;
      }
    }
  }
}
