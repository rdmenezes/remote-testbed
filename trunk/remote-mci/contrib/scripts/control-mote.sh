#!/bin/bash
#
# control-mote.sh TTY CMD
#
# TODO: Description!
#
# Rostislav Spinar 2008
# rostislav.spinar@cit.ie

#Get parameters from command line
TTY="$1"
CMD="$2"

#Have it as a env. variable
PLATFORM="$platform"

#Check the parameters first
#Suppose to have 2 args
if [ "$#" -ne 2 ]; then
	echo "Uncorrect number of arguments"
	exit
fi

if [[ -n "$TTY" && -c "$TTY" ]]; then
	echo "Using port $TTY..."
else
	echo "Port has to be defined..."
	exit
fi

#Run command(reset, stop & start(reset from DIKU_MCH))
case "$CMD" in
	"reset" | "stop" | "start")
		if [ "$PLATFORM" = "TMoteSky" ]; then
			echo "The $PLATFORM mote...$CMD"
			if [ "$CMD" = "start" ]; then
				CMD="reset"
			fi
			#echo "tos-bsl --telosb -c $TTY --$CMD"
			echo "cppbsl -D -b -c $TTY --$CMD"
			#tos-bsl --telosb -c "$TTY" --"$CMD"
			cppbsl -D -b -c "$TTY" --"$CMD"
		elif [ "$PLATFORM" = "MicaZ" ]; then
			echo "The $PLATFORM mote...$CMD"
			if [ "$CMD" = "start" ]; then
				CMD="reset"
			fi
			echo "uisp -dprog=mib510 -dserial=$TTY -dpart=ATmega128 --$CMD"
			uisp -dprog=mib510 -dserial=$TTY -dpart=ATmega128 --"$CMD"
		else
			echo "Unknown platform..."
			exit
		fi
		;;
	*)
		echo "Unknown command..."
		exit
		;;
esac

echo "We are done."

