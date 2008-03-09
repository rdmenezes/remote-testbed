#!/bin/sh
#
# NAME
# ----
# remote.sh - script for maintaining the /dev/remote device hierarchy
#
# SYNOPSIS
# --------
# /etc/udev/scripts/remote.sh PLATFORM MOTEMAC MOTEPATH PROGRAMMER CONTROLLER
#
# DESCRIPTION
# -----------
# This script is designed to be called via a UDEV rule's RUN key. It
# sets up extra information for added motes and cleans up after removed
# motes, as well as notifies the motehost about new events. It assumes
# that when a device is added the associated TTY has already been
# created.
#
# When called for an add ACTION, the script expects two command line
# arguments described below. No arguments are expected when handling a
# remove ACTION.
#
# OPTIONS
# -------
# PLATFORM::
#
#	The first argument is the name of the mote platform.
#
# MOTEMAC::
#
#	The second argument is the mote MAC, part of which can often be
#	acquired using %s{serial} or similar sysfs entry and combined
#	with a well-known vendor and product ID.
#
# MOTEPATH::
#
#	The third is the desired device path, which should be a unique
#	enough to fully describe the bus, where the mote is attached,
#	but should be reproducable in such a way that the same device
#	path should be used when reinserting the same mote into the same
#	port.
#
# PROGRAMMER::
#
#	The path to the mote flash programmer.
#
# CONTROLLER:
#	The path to the external script for controlling the mote.
#
# ENVIRONMENT VARIABLES
# ---------------------
# The script has been designed to rely on as few environment variables
# as possible to make it more generic. Instead it relies on being passed
# all important variables as command line arguments. The only
# environment variable used is the ACTION environment variable set by
# UDEV.
#
# FILES
# -----
# /dev/remote/${MOTEMAC}/path::
#
#	The physical UDEV device path (or similar) for the mote device
#	with the given MAC address. This file is created by this script
#	using the MOTEPATH command line argument.
#
# /dev/remote/${MOTEMAC}/platform::
#
#	This file contains the name of the mote platform and is created
#	by this script using the MOTEPLATFORM command line argument.
#
# /dev/remote/${MOTEMAC}/programmer::
#
#	Symlink to the mote flash programmer as specified via the
#	PROGRAMMER option. The symlink is created by this script.
#
# /dev/remote/${MOTEMAC}/controller::
#
#	Symlink to the mote controller as specified via the CONTROLLER
#	option. The symlink is created by this script. This option is
#	relevant only for motes which need external script for their
#	control.
#
# /dev/remote/${MOTEMAC}/tty::
#
#	Symlink to the tty device that can be used for controlling the
#	mote with the given MAC address. This should be set up using a
#	UDEV rule containing the SYMLINK key. This script assumes that
#	the tty symlink exists when it is called. If this is not the
#	case it will error out.
#
# EXAMPLE
# -------
# As an example, the dig528-2 and MicaZ UDEV rules use the following
# arguments:
#
#  /etc/udev/scripts/remote.sh dig528-2 0050C237%s{serial} %E{PHYSDEVPATH} /sbin/hc08sprg
#  /etc/udev/scripts/remote.sh MicaZ 04036010%s{serial} %E{PHYSDEVPATH} /sbin/program-mote.sh /sbin/control-mote.sh
#
# AUTHOR
# ------
# Copyright (c) 2007 Jonas Fonseca <fonseca@diku.dk>
#		2008 Rostislav Spinar <rostislav.spinar@cit.ie>
#
########################################################################
# Configuration section
########################################################################

# The path to the device hierarchy
DEVROOT="/dev/remote"

# File to log messages to; leave empty to disable logging
LOGFILE="/var/log/remote-udev.log"

# Path to the MCH event pipe
MCHPIPE="/var/run/motehost.events"

# Path to the program that should be used for signaling MCH
PLUGPIPER="/sbin/plugpiper"

########################################################################
# The script
########################################################################

PLATFORM="$1"
MOTEMAC="$2"
MOTEPATH="$3"
PROGRAMMER="$4"
CONTROLLER="$5"

info ()
{
	test -n "$LOGFILE" && echo "[$(date)] $ACTION: $*" >> "$LOGFILE"
}

die ()
{
	info "$*"
	exit 0
}

test -d "$DEVROOT" || die "Device root is not a directory ($DEVROOT)"

case "$ACTION" in
add)
	test -n "$MOTEMAC" || die "No mote MAC defined"
	test -n "$MOTEPATH" || die "No device path defined"
	test -e "$DEVROOT/$MOTEMAC/tty" || die "No TTY exists for '$MOTEMAC'"
	test -e "$DEVROOT/$MOTEMAC/path" && die "Path exists for '$MOTEMAC'"
	echo "$MOTEPATH" > "$DEVROOT/$MOTEMAC/path"
	echo "$PLATFORM" > "$DEVROOT/$MOTEMAC/platform"
	ln -s "$PROGRAMMER" "$DEVROOT/$MOTEMAC/programmer" ||
		die "Failed to create program symlink"
	if [ -n "$CONTROLLER" ]; then
		ln -s "$CONTROLLER" "$DEVROOT/$MOTEMAC/controller" ||
			die "Failed to create program symlink"
	fi
	info "mote '$MOTEMAC' with path '$MOTEPATH' and platform '$PLATFORM'"
	;;

remove)
	tmpdir="$DEVROOT/.tmp"
	# Use the tmpdir as a semaphore when multiple motes are removed
	test -d "$tmpdir" && return
	mkdir -p "$tmpdir" || die "Failed to create temp dir '$tmpdir'"
	# Remove devices where no tty link exists.
	ls "$DEVROOT/" | while read motedir;
	do
		dir="$DEVROOT/$motedir"
		test -e "$dir/tty" && continue;
		info "Removing mote '$motedir'"
		# Cannot die here since we need to remove the temp dir
		mv "$dir" "$tmpdir" || info "Failed to move '$dir' to '$tmpdir'"
	done
	# The temp directory is empty?
	rmdir "$tmpdir" && return
	# Fatal but the mote host should still be notified
	rm -rf "$tmpdir" || info "Failed to remove directory '$tmpdir'"
	;;
esac

# Notify mote host
test -p "$MCHPIPE" || die "Event pipe does not exist at '$MCHPIPE'"
"$PLUGPIPER" "$MCHPIPE" || die "Event program returned error"
