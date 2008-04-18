#!/bin/bash
#
# NAME
# ----
# control-mote.sh - script for executing the control commands
#
# SYNOPSIS
# --------
# /sbin/control-mote.sh TTY CMD
#
# DESCRIPTION
# -----------
# The script makes an use of a bootloader for a particular platform
# to issue one of the available commands (STOP/START/RESET). 
#
# OPTION
# ------
# TTY::
#
#	The first argument is a USB device name used by the bootloader
#	to execute the command.
#
# CMD::
# 
#	The second one is the command itself to be executed.
#
# ENVIRONMENT VARIABLES
# ---------------------
# PLATFORM::
#
#	It contains a name of the platform. The featured platforms
#       are TMoteSky and MicaZ.
#
# EXAMPLE
# -------
# /sbin/control-mote.sh /dev/ttyUSB0 reset
#
# AUTHOR 
# ------
# Copyright (c) 2008 Rostislav Spinar <rostislav.spinar@cit.ie>
#
#######################################################################
# The script
#######################################################################

#Get parameters from command line
TTY="$1"
CMD="$2"

#Have it as a env. variable
PLATFORM="$platform"

#Check the parameters first
#Suppose to have 2 args
if [ "$#" -ne 2 ]; then
	echo "Incorrect number of arguments!"
	exit
fi

if [[ ! -n "$TTY" && ! -c "$TTY" ]]; then
	echo "Port has to be defined and point to a valid character device! Port=$TTY!"
	exit
fi

#Run command(reset, stop & start)
case "$CMD" in
	"reset" | "stop" | "start")
		
		echo "$PLATFORM : command $CMD"

		if [ "$CMD" = "start" ]; then
			CMD="reset"
		fi

		case "$PLATFORM" in
			"TMoteSky")
				#echo "tos-bsl --telosb -c $TTY --$CMD"
				#echo "cppbsl -D -b -c $TTY --$CMD"
				#tos-bsl --telosb -c "$TTY" --"$CMD"
				cppbsl -D -b -c "$TTY" --"$CMD" > /dev/null 2>&1
				RESULT=$?
				;;
			"MicaZ")
				#echo "uisp -dprog=mib510 -dserial=$TTY -dpart=ATmega128 --$CMD"
				uisp -dprog=mib510 -dserial=$TTY -dpart=ATmega128 --"$CMD" > /dev/null 2>&1
				RESULT=$?
				;;
			*)
				echo "Unknown platform $PLATFORM!"
				exit
				;;
		esac
		;;
	*)
		echo "Unknown command $CMD!"
		exit
		;;
esac

echo "Done!"
exit "$RESULT"

