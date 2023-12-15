#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "cons.h"

void calculateJuliaSet(uint8_t pixels[SIZE][SIZE][3]);
void exportJuliaSet(FILE *f, uint8_t pixels[SIZE][SIZE][3]);

int main() {
  uint8_t pixels[SIZE][SIZE][3];

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

INLINE void exportJuliaSet(FILE *f, uint8_t pixels[SIZE][SIZE][3]) {
  fprintf(f, "P3\n");
  fprintf(f, "%d %d\n", SIZE, SIZE);
  fprintf(f, "255\n");

  for (int line = 0; line < SIZE; line++) {
    for (int col = 0; col < SIZE; col++) {
      fprintf(f, "%u %u %u ", pixels[col][line][0], pixels[col][line][1],
              pixels[col][line][2]);
    }
    fprintf(f, "\n");
  }
}

INLINE void calculateJuliaSet(uint8_t pixels[SIZE][SIZE][3]) {
  double a = A;
  double b = B;

  for (int line = 0; line < SIZE; line++) {
    for (int col = 0; col < SIZE; col++) {
      int i = 2;

      double x = XMIN + col * XSTEP;
      double y = YMAX - line * YSTEP;

      while (i <= MAXITER && (x * x + y * y) <= 4) {
        double x_temp = x * x - y * y + a;
        y = 2 * x * y + b;
        x = x_temp;

        i++;
      }

      if (i > MAXITER && (x * x + y * y) <= 4) {
        pixels[col][line][0] = 0;
        pixels[col][line][1] = 0;
        pixels[col][line][2] = 0;
      } else {
        pixels[col][line][0] = (4 * i) % 256;
        pixels[col][line][1] = 2 * i;
        pixels[col][line][2] = (6 * i) % 256;
      }
    }
  }
}
