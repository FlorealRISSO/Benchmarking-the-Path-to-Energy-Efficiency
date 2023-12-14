#!/bin/sh

die() { printf '%s\n' "$*" >&2; exit 1; }

# Check if the script is being run as root
[ $(id -u) -eq 0 ] || die 'Error: This script must be run as root.'

# Check if a commande is provided
[ "$1" ] || die 'Error: no frequency provided.'

fmin=`cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq`
fmax=`cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq`

if [ $1 -lt $fmin ] || [ $1 -gt $fmax ]; then
	die "Error: frenquency should be $fmin < $1 < $fmax."
fi

echo $1 | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_max_freq

