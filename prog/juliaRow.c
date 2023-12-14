#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cons.h"


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
  double a = A;
  double b = B;

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
