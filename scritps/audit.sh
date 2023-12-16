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
	NR>1 {
		core += $3
		ram += $4
	}	
	END {
		jcore = core / 1000000
		jram = ram / 1000000
		jtotal = jcore + jram
		if (NR == 2) {
			print "?", jtotal, jcore, jram, "?"
			exit 
		}
		time = $1 - prev_timestamp
		watt = jtotal / time
		print time, jtotal, jcore, jram, watt
	}
' $log

