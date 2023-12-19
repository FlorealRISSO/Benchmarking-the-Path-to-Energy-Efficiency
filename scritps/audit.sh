#!/bin/sh
time=$(which time)

die() { printf '%s\n' "$*" >&2; exit 1; }

# Check if the script is being run as root
[ $(id -u) -eq 0 ] || die 'Error: This script must be run as root.'

# Check if a commande is provided
[ "$1" ] || die 'Error: no argument provided'

# Log file
log=mojitos-result.log

mojitos -f 10 -r -o $log &
pid=$!

# Execution of the commmand with 'time'
etime=$( { $time -f "%e" "$@"; } 2>&1)
kill $pid

awk -v etime="$etime" '
	NR==2 {prev_timestamp = $1}  
	NR>1 {
		core += $2
		ram += $3
	}	
	END {
		jcore = core
		jram = ram
		jtotal = jcore + jram
		if (NR == 2) {
			print "?", jtotal, jcore, jram, "?"
			exit 
		}
		time = etime
		watt = jtotal / time
		print time, jtotal, jcore, jram, watt
	}
' $log

