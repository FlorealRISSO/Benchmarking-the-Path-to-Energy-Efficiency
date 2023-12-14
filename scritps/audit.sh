#!/bin/sh

die() { printf '%s\n' "$*" >&2; exit 1; }

# Check if the script is being run as root
[ $(id -u) -eq 0 ] || die 'Error: This script must be run as root.'

# Check if a commande is provided
[ "$1" ] || die 'Error: no argument provided'

# Log file
log=mojitos-result.log

mojitos -f 10 -r -o $log &
pid=$!

"$@"

kill $pid

awk '
	NR==2 {prev_timestamp = $1}  
	NR>1 {sum += $3} 
	END {
		joule = sum / 1000
		if (NR == 2) {
			print "?", joule, "?"
			exit 
		}
		time = $1 - prev_timestamp
		watt = joule / time
		print time, joule, watt
	}
' $log

