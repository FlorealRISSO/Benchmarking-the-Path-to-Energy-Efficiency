#include <emmintrin.h>
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "cons.h"

typedef union {
  __m256d simd;
  struct {
    double r1;
    double r2;
    double r3;
    double r4;
  } r;
  double rs[4];
} vecd4;

typedef union {
  __m128i simd;
  struct {
    int r1;
    int r2;
    int r3;
    int r4;
  } r;
  int rs[4];
} veci4;

void calculateJuliaSet(uint8_t *pixels);
void exportJuliaSet(FILE *f, uint8_t *pixels);

int main() {
  uint8_t pixels[SIZE];

  calculateJuliaSet(pixels);

  FILE *f = fopen("julia.ppm", "w");
  if (f == NULL) {
    fprintf(stderr, "Error opening file for writing\n");
    return 1;
  }

  exportJuliaSet(f, pixels);
  fclose(f);

  return 0;
}

INLINE void exportJuliaSet(FILE *f, uint8_t *pixels) {
  fprintf(f, "P3\n");
  fprintf(f, "%d %d\n", LINE, COL);
  fprintf(f, "255\n");

  int idx = 0;
  for (int i = 0; i < SIZE; i += COLORS) {
    fprintf(f, "%u %u %u ", pixels[i + 0], pixels[i + 1], pixels[i + 2]);

    int col = idx % COL;
    if (col == 0) {
      fprintf(f, "\n");
    }
    idx += 1;
  }
  fprintf(f, "\n");
}

INLINE void calculateJuliaSet(uint8_t *pixels) {
  vecd4 winx = {XMIN, XMIN + step_x, XMIN + 2 * step_x, XMIN + 3 * step_x};
  vecd4 winy = {YMAX, YMAX - step_y, YMAX - 2 * step_y, YMAX - step_y * 3};
  veci4 wini = {2};

  int idx = 3;
  for (int k = 0; k < SIZE; k += 3) {
    if (idx != 3) {
      int col = idx % COL;
      int line = idx / LINE;

      double x = XMIN + col * step_x;
      double y = YMAX - line * step_y;
      winx.rs[0] = winx.rs[1];
      winx.rs[1] = winx.rs[2];
      winx.rs[2] = winx.rs[3];
      winx.rs[AVXD - 1] = x;

      winy.rs[0] = winy.rs[1];
      winy.rs[1] = winy.rs[2];
      winy.rs[2] = winy.rs[3];
      winy.rs[AVXD - 1] = y;

      wini.rs[0] = wini.rs[1];
      wini.rs[1] = wini.rs[2];
      wini.rs[2] = wini.rs[3];
      wini.rs[AVXD - 1] = 2;
    }
    idx += 1;

    int i = wini.rs[0];
    // int i = 2;
    double x = winx.rs[0];
    double y = winy.rs[0];
    int new_i = 0;

    while (i <= iterationmax && (x * x + y * y) <= 4) {
      // load
      __m256d x256 = winx.simd;
      __m256d y256 = winy.simd;

      // calculate y = (((x * y) * 2) + B);
      __m256d r1 = _mm256_mul_pd(x256, y256);
      __m256d r2 = _mm256_set1_pd(2.0);
      __m256d r3 = _mm256_mul_pd(r1, r2);
      __m256d b = _mm256_set1_pd(B);
      winy.simd = _mm256_add_pd(r3, b);

      // calculate x
      __m256d x2 = _mm256_mul_pd(x256, x256);
      __m256d y2 = _mm256_mul_pd(y256, y256);

      __m256d r6 = _mm256_sub_pd(x2, y2);
      __m256d a = _mm256_set1_pd(A);
      __m256d r7 = _mm256_add_pd(r6, a);

      winx.simd = r7;
      x = winx.rs[0];
      y = winy.rs[0];
      i++;
      new_i++;
    }
    veci4 ri = {new_i};
    wini.simd = _mm_add_epi32(wini.simd, ri.simd);

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
