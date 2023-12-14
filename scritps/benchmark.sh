#!/bin/sh

die() { printf '%s\n' "$*" >&2; exit 1; }

# Check if the script is being run as root
[ $(id -u) -eq 0 ] || die 'Error: This script must be run as root.'

fmin=`cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq`
fmax=`cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq`

step=100000
cmd='python3 julia.py'

rm -f benchmark.log
for freq in $(seq $fmin $step $fmax); do
	echo $freq | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_max_freq
	printf "$freq: %s\n" "$(./audit.sh $cmd)" >> "benchmark.log"
done
