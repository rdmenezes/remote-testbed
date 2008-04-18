#!/bin/bash
#
# NAME
# ----
# program-mote.sh - script for programming of a mote
#
# SYNOPSIS
# --------
# /sbin/program-mote.sh TTY IMAGE
#
# DESCRIPTION
# -----------
# The script makes an use of a bootloader for a particular platform
# to program a binary into the mote's flash memory. It first sets
# the correct network ID and then converts the binary into the Intel 
# hex format.
#
# OPTION
# ------
# TTY::
#
# 	The first argument is a USB device name used by the bootloader
#       to program the mote.
#
# IMAGE::
# 
#	The second one is the binary itself.
#
# ENVIRONMENT VARIABLES
# ---------------------
# NETADDRESS::
#
#	The network ID is a unique identifier which needs to be assigned
#	to every single node within our network.
#
# PLATFORM::
#
#	It contains a name of the platform. The featured platforms
#       are TMoteSky and MicaZ.
#
# EXAMPLE
# -------
# /sbin/program-mote.sh /dev/ttyUSB0 blink.exe
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
IMAGE="$2"

#Have it as a env. variable
NETADDRESS="$netaddress"
PLATFORM="$platform"

IMAGEHEX="${IMAGE%.*}".ihex

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

if [[ ! -n "$NETADDRESS" ]]; then
	echo "NETADDRESS has to be defined!"
	exit
fi

if [[ ! -e "$IMAGE" ]]; then
	echo "Image file has to be present!"
	exit
fi

#This works for TOS1 & TOS2
echo "$PLATFORM : changing node id..."
echo "$PLATFORM : changing node id..."
echo "$PLATFORM : programming image..."

case "$PLATFORM" in
	"TMoteSky")
		#echo "tos-set-symbols --objcopy msp430-objcopy --objdump msp430-objdump --exe $IMAGE $IMAGE TOS_LOCAL_ADDRESS=$NETADDRESS TOS_NODE_ID=$NETADDRESS ActiveMessageAddressC\$addr=$NETADDRESS"
		tos-set-symbols --objcopy msp430-objcopy --objdump msp430-objdump --exe "$IMAGE" "$IMAGE" TOS_LOCAL_ADDRESS="$NETADDRESS" TOS_NODE_ID="$NETADDRESS" ActiveMessageAddressC\$addr="$NETADDRESS" > /dev/null 2>&1

		#echo "msp430-objcopy --output-target=ihex $IMAGE $IMAGEHEX"
		msp430-objcopy --output-target=ihex "$IMAGE" "$IMAGEHEX" > /dev/null 2>&1

		#echo "tos-bsl --telosb -c $TTY -epvrI $IMAGEHEX"
		#echo "cppbsl -D -b -c $TTY -p $IMAGEHEX"
		#tos-bsl --telosb -c "$TTY" -epvrI "$IMAGEHEX"
		
		cppbsl -D -b -c "$TTY" -p "$IMAGEHEX" > /dev/null 2>&1
		RESULT=$?

		#echo "Removing flash image (Intel Hex)..."
		#echo "rm -f $IMAGEHEX"
		rm -f "$IMAGEHEX"
		;;
	"MicaZ")
		#echo "tos-set-symbols --exe $IMAGE $IMAGE TOS_LOCAL_ADDRESS=$NETADDRESS TOS_NODE_ID=$NETADDRESS ActiveMessageAddressC\$addr=$NETADDRESS"
		tos-set-symbols --exe "$IMAGE" "$IMAGE" TOS_LOCAL_ADDRESS="$NETADDRESS" TOS_NODE_ID="$NETADDRESS" ActiveMessageAddressC\$addr="$NETADDRESS" > /dev/null 2>&1

		#echo "avr-objcopy --output-target=ihex $IMAGE $IMAGEHEX"
		avr-objcopy --output-target=ihex "$IMAGE" "$IMAGEHEX" > /dev/null 2>&1

		#echo "uisp -dprog=mib510 -dserial=$TTY --wr_fuse_h=0xde -dpart=ATmega128 --wr_fuse_e=ff --erase --upload if=$IMAGEHEX"
		uisp -dprog=mib510 -dserial="$TTY" --wr_fuse_h=0xde -dpart=ATmega128 --wr_fuse_e=ff --erase --upload if="$IMAGEHEX" > /dev/null 2>&1
		RESULT=$?

		#Another option for MicaZ
		#avrdude -cmib510 -P"$TTY" -U hfuse:w:0xd9:m -pm128  -U efuse:w:0xff:m -C/etc/avrdude/avrdude.conf -U flash:w:"$IMAGEHEX":a

		#echo "Removing flash image (Intel Hex)..."
		#echo "rm -f $IMAGEHEX"
		rm -f "$IMAGEHEX"
		;;
	*)
		echo "Unknown platform $PLATFORM!"
		exit
		;;
esac

echo "Done!"
exit "$RESULT"

