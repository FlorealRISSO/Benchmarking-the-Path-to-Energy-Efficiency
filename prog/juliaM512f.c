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
  const __m512 xmin = _mm512_set1_ps(XMIN);
  const __m512 ymax = _mm512_set1_ps(YMAX);
  const __m512 xstep = _mm512_set1_ps(XSTEP);
  const __m512 ystep = _mm512_set1_ps(YSTEP);
  const __m512 max_iter = _mm512_set1_ps(MAXITER);
  const __m512 threshold = _mm512_set1_ps(4.0);
  const __m512i one = _mm512_set1_epi32(1);
  const __m512i zero = _mm512_set1_epi32(0);
  const __m512 a = _mm512_set1_ps(A);
  const __m512 b = _mm512_set1_ps(B);

  int ti = 0;
  for (int i = 0; i < SIZE; i += (16 * COLORS)) {
    int col = ti % COL;
    int line = ti / LINE;
    ti += 16;

    // XMIN + col * step_x
    __m512 x =
        _mm512_set_ps(col + 15, col + 14, col + 13, col + 12, col + 11,
                      col + 10, col + 9, col + 8, col + 7, col + 6, col + 5,
                      col + 4, col + 3, col + 2, col + 1, col + 0);
    x = _mm512_mul_ps(x, xstep);
    x = _mm512_add_ps(x, xmin);

    // YMIN - line * step_y
    __m512 y = _mm512_set_ps(line, line, line, line, line, line, line, line,
                             line, line, line, line, line, line, line, line);
    y = _mm512_mul_ps(y, ystep);
    y = _mm512_sub_ps(ymax, y);

    __m512 zx = x;
    __m512 zy = y;

    __m512i iter = _mm512_set1_epi32(2);
    __mmask16 mask = _mm512_int2mask(0xFFFFFFFF);

    for (int j = 2; j <= MAXITER; j++) {

      /* LOOP CONDITION */
      __m512 x2 = _mm512_mul_ps(zx, zx);
      __m512 y2 = _mm512_mul_ps(zy, zy);
      // Check if |x2 + y2| < 4.0

      __m512 sum = _mm512_add_ps(x2, y2);
      __mmask16 zmask = _mm512_cmp_ps_mask(sum, threshold, _CMP_LE_OQ);

      mask = _mm512_kand(mask, zmask);

      // Check if all pixels have converged

      if (_kortestz_mask16_u8(mask, mask)) {
        break;
      }
      /* END LOOP CONDITION */

      /* LOOP BODY */
      // Upsate iteration count
      __m512i _iter = _mm512_maskz_add_epi32(mask, zero, one);
      iter = _mm512_add_epi32(_iter, iter);

      // y = 2 * x * y + b
      zy = _mm512_mul_ps(zx, zy);
      zy = _mm512_mul_ps(zy, _mm512_set1_ps(2.0));
      zy = _mm512_add_ps(zy, b);

      // x = x * x - y * y + a
      zx = _mm512_sub_ps(x2, y2);
      zx = _mm512_add_ps(zx, a);
      /* END LOOP BODY */
    }

    int tidx[16];
    _mm512_storeu_si512((__m512i *)tidx, iter);
    for (int j = 0; j < 16; j++) {
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
