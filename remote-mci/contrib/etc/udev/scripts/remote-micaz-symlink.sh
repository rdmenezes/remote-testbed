#!/bin/sh
#
# NAME
# ----
# remote-micaz-symlink.sh - script for creating symlinks for MicaZ 
#
# SYNOPSIS
# --------
# /etc/udev/scripts/remote-micaz-symlink.sh MOTEMAC
#
# DESCRIPTION
# -----------
# This script is designed to be called via a UDEV rule's PROGRAM key.
# The output from the script is used by SYMLINK rule to create both
# control and data links to tty devices. 

# The first tty device on the MIB520 programming board is always for 
# programming and controlling of MicaZ and second one is for 
# retrieving data from MicaZ.
#
# When called the script expects one command line arguments described 
# below.
#
# OPTIONS
# -------
# MOTEMAC::
#
#	The first argument is the mote MAC, part of which can often be
#	acquired using %s{serial} or similar sysfs entry and combined
#	with a well-known vendor and product ID.
#
#
# FILES
# -----
# /dev/remote/${MOTEMAC}/tty/control::
# /dev/remote/${MOTEMAC}/tty/data::
#
#	Symlinks to the tty device that can be used for controlling the
#	mote with the given MAC address. This should be set up using a
#	UDEV rule containing the SYMLINK key.
#
# EXAMPLE
# -------
# As an example here is the arguments given from the MicaZ UDEV rule:
#
#	/etc/udev/scripts/remote-micaz-symlink.sh 04036010%s{serial}
#
# AUTHOR
# ------
# Copyright (c) 2007 Jonas Fonseca <fonseca@diku.dk>
#                    Rosta Spinar <rostislav.spinar@cit.ie>
#
########################################################################
# Configuration section
########################################################################

# The path to the device hierarchy
DEVROOT="/dev/remote"

# File to log messages to; leave empty to disable logging
LOGFILE="/var/log/remote-udev.log"

########################################################################
# The script
########################################################################

MOTEMAC="$1"

info ()
{
	test -n "$LOGFILE" && echo "[$(date)] $*" >> "$LOGFILE"
}

die ()
{
	info "$*"
	exit 0
}

test -n "$MOTEMAC" || die "Symlink MicaZ: No mote MAC defined"

if test -e "$DEVROOT/$MOTEMAC/tty/control"
then
	echo data
else
	echo control
fi
