#!/bin/bash

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

