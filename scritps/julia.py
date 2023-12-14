#!/usr/bin/env python

# MIT License
# 
# Copyright (c) 2023 Floréal Risso <floreal.risso@univ-tlse3.fr>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import matplotlib.pyplot as plt
import numpy as np

taille = 500
xmin, xmax = -1, 1
ymin, ymax = -1, 1
iterationmax = 100

a = -0.8
b = 0.156

pixels = np.zeros((taille, taille, 3), dtype='i')

for line in range(taille):
    for col in range(taille):
        i = 1
        x = xmin+col*(xmax-xmin)/taille
        y = ymax-line*(ymax-ymin)/taille
        while i<=iterationmax and (x**2+y**2)<=4:

            x, y = x**2-y**2+a, 2*x*y+b
            i += 1
            if i>iterationmax and (x**2+y**2)<=4:
                pixels[col,line] = (0,0,0)
            else:
                pixels[col,line] = ((4*i)%256,2*i,(6*i)%256)

with open('julia.ppm', 'w') as f:
    f.write('P3\n')
    f.write(str(taille)+' '+str(taille)+'\n')
    f.write('255\n')
    for line in range(taille):
        for col in range(taille):
            f.write(str(pixels[col,line,0])+' '+str(pixels[col,line,1])+' '+str(pixels[col,line,2])+' ')
        f.write('\n')

# plt.imshow(pixels)
# plt.show(block=True) 
