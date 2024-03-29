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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cons.h"

void calculateJuliaSet(uint8_t *pixels);
void exportJuliaSet(FILE *f, uint8_t *pixels);

int main() {
  uint8_t *pixels = malloc(SIZE);

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
