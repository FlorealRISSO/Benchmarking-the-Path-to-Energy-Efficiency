#!/bin/bash

die() { printf '%s\n' "$*" >&2; exit 1; }

# Check if the script is being run as root
[ $(id -u) -eq 0 ] || die 'Error: This script must be run as root.'

# Check the params
[ "$#" -ne 2 ] && die "Usage: $0 <folder> <iterations>"
[ ! -d "$1" ] && die "Error: $1 is not a directory."

folder="$1"
iterations="$2"
timestamp=$(date +"%Y%m%d_%H%M%S")
cmd='./audit.sh'


for file in "$folder"/*; do
  if [ -x "$file" ] && [ -f "$file" ]; then
    filename=$(basename "$file")
    echo "Executing $filename $iterations times:"
    for ((i = 1; i <= iterations; i++)); do
      $cmd "$file" >> "results/$filename$timestamp.log"
    done
    echo "Done."
  fi
done

