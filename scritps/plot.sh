#!/bin/bash

die() { printf '%s\n' "$*" >&2; exit 1; }

folder='results'
[ ! -d $folder ] && die "Error: $folder is not a directory."

[ "$#" -ne 1 ] && die "Usage: $0 <timestamp>"
timestamp=$1

echo 'file time energy' > data.csv

for file in "$folder"/*$timestamp.log; do
	executable=$(basename "${file:0:-19}")
	awk -v executable=$executable '
	NR>1 {
		time += $1
		energy+= $2
	}

	END {
		num_rows = NR - 1
		time = time / num_rows
		energy = energy / num_rows

		print executable, time, energy
	}
' $file >> data.csv
done

