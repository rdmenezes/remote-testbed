#!/bin/sh
#
# Script to program DIG528-2 motes.
# Copyright (c) 2007 Jonas Fonseca <fonseca@diku.dk>
#
# Expects the TTY-path and program-file as the first and second parameter.

exec /sbin/hc08sprg "$1" 115200 "$2" "$macaddress" "$netaddress"
