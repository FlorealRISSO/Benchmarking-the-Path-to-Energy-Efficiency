#!/bin/bash

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

die() { printf '%s\n' "$*" >&2; exit 1; }

folder='results'
[ ! -d $folder ] && die "Error: $folder is not a directory."

[ "$#" -ne 1 ] && die "Usage: $0 <timestamp>"
mkdir -p analyzes

timestamp=$1
output="analyzes/data_$timestamp.csv"

echo 'file time avg-energy max-energy min-energy' > "$output"

for file in "$folder"/*$timestamp.log; do
	executable=$(basename "${file:0:-19}")
	awk -v executable=$executable '
	NR==2 {
		max=$2
		min=$2
	}

	NR>1 {
		time += $1
		energy+= $2
		if(min > $2) {
			min = $2
		}
		if (max < $2) {
			max = $2
		}
	}

	END {
		line = NR - 1
		time = time / line
		energy = energy / line
		# convert the values into Joule
		energy /= 1000000
		min /= 1000000
		max /= 1000000

		print executable, time, energy, max, min
	}
' $file >> "$output"
done

