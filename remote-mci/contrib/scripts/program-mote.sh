#!/bin/bash
#
# program-mote.sh TTY IMAGE
#
# TODO: Description!
#
# Rostislav Spinar 2008
# rostislav.spinar@cit.ie

#Get parameters from command line
TTY="$1"
IMAGE="$2"

#Have it as a env. variable
TOSADDRESS="$tosaddress"
PLATFORM="$platform"

IMAGEHEX="${IMAGE%.*}".ihex

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

if [[ -n "$TOSADDRESS" ]]; then
	echo "Using TOSADDRESS $TOSADDRESS..."
else
	echo "TOSADDRESS has to be defined..."
	exit
fi

if [[ -z "$IMAGE" ]]; then
	echo "Image has to be defined..."
	exit
fi

#This works for TOS1 & TOS2
case "$PLATFORM" in
	"TMoteSky")
		echo "Changing node id..."
		echo "tos-set-symbols --objcopy msp430-objcopy --objdump msp430-objdump --exe $IMAGE $IMAGE TOS_LOCAL_ADDRESS=$TOSADDRESS TOS_NODE_ID=$TOSADDRESS ActiveMessageAddressC\$addr=$TOSADDRESS"
		tos-set-symbols --objcopy msp430-objcopy --objdump msp430-objdump --exe "$IMAGE" "$IMAGE" TOS_LOCAL_ADDRESS="$TOSADDRESS" TOS_NODE_ID="$TOSADDRESS" ActiveMessageAddressC\$addr="$TOSADDRESS"

		echo "Converting flash image (Intel Hex)..."
		echo "msp430-objcopy --output-target=ihex $IMAGE $IMAGEHEX"
		msp430-objcopy --output-target=ihex "$IMAGE" "$IMAGEHEX"

		echo "Programming image on Tmote..."
		#echo "tos-bsl --telosb -c $TTY -epvrI $IMAGEHEX"
		echo "cppbsl -D -b -c $TTY -p $IMAGEHEX"
		#tos-bsl --telosb -c "$TTY" -epvrI "$IMAGEHEX"
		cppbsl -D -b -c "$TTY" -p "$IMAGEHEX"

		echo "Removing flash image (Intel Hex)..."
		echo "rm -f $IMAGEHEX"
		rm -f "$IMAGEHEX"
		;;
	"MicaZ")
		echo "Changing node id..."
		echo "tos-set-symbols --exe $IMAGE $IMAGE TOS_LOCAL_ADDRESS=$TOSADDRESS TOS_NODE_ID=$TOSADDRESS ActiveMessageAddressC\$addr=$TOSADDRESS"
		tos-set-symbols --exe "$IMAGE" "$IMAGE" TOS_LOCAL_ADDRESS="$TOSADDRESS" TOS_NODE_ID="$TOSADDRESS" ActiveMessageAddressC\$addr="$TOSADDRESS"

		echo "Converting flash image (Intel Hex)..."
		echo "avr-objcopy --output-target=ihex $IMAGE $IMAGEHEX"
		avr-objcopy --output-target=ihex "$IMAGE" "$IMAGEHEX"

		echo "Programming image on MicaZ..."
		echo "uisp -dprog=mib510 -dserial=$TTY --wr_fuse_h=0xde -dpart=ATmega128 --wr_fuse_e=ff --erase --upload if=$IMAGEHEX"
		uisp -dprog=mib510 -dserial="$TTY" --wr_fuse_h=0xde -dpart=ATmega128 --wr_fuse_e=ff --erase --upload if="$IMAGEHEX"

		#Another option for MicaZ
		#avrdude -cmib510 -P"$TTY" -U hfuse:w:0xd9:m -pm128  -U efuse:w:0xff:m -C/etc/avrdude/avrdude.conf -U flash:w:"$IMAGEHEX":a

		echo "Removing flash image (Intel Hex)..."
		echo "rm -f $IMAGEHEX"
		rm -f "$IMAGEHEX"
		;;
	*)
		echo "Unknown platform..."
		exit
		;;
esac

echo "We are done."
