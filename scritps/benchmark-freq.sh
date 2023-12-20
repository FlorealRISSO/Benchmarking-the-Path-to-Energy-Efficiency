#!/bin/sh

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

# Check if a commande is provided
[ "$1" ] || die "Usage $0 <cmd>"

fmin=`cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq`
fmax=`cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq`

step=100000
cmd=$@

rm -f benchmark.log
for freq in $(seq $fmin $step $fmax); do
	echo $freq | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_max_freq
	printf "$freq: %s\n" "$(./audit.sh $cmd)" >> "benchmark.log"
done
