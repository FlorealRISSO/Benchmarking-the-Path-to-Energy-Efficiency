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

# Check if the script is being run as root
[ $(id -u) -eq 0 ] || die 'Error: This script must be run as root.'

# Check the params
[ "$#" -ne 2 ] && die "Usage: $0 <folder> <iterations>"
[ ! -d "$1" ] && die "Error: $1 is not a directory."

folder="$1"
iterations="$2"
timestamp=$(date +"%Y%m%d_%H%M%S")
cmd='./audit.sh'

mkdir -p results

for file in "$folder"/*; do
  if [ -x "$file" ] && [ -f "$file" ]; then
    filename=$(basename "$file")
    echo "Executing $filename $iterations times:"
    echo 'time energy_total energy_core energy_ram power' > "results/$filename$timestamp.log"
    for ((i = 1; i <= iterations; i++)); do
      $cmd "$file" >> "results/$filename$timestamp.log"
    done
    echo "Done."
  fi
done

