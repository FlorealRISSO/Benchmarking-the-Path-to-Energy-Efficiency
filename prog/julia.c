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

#include "cons.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
