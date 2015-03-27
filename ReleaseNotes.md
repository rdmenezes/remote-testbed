# Release Notes #

## Version 2.x (to be released as version 2.0) ##

  * MCI: Add custom configuration parser and drop dependency on Boost. ([issue 31](https://code.google.com/p/remote-testbed/issues/detail?id=31))
  * MCI: Make MCS use the logging infrastructure and use syslog when daemonizing ([issue 22](https://code.google.com/p/remote-testbed/issues/detail?id=22))

## Version 1.x (to be released as version 1.1) ##

  * MCI: Add [man-pages](http://remote-testbed.googlecode.com/svn/documentation/remote-mci/) for mote host and mote server as well as UDEV scripts.
  * MCI: Fix the mote control server's service loop introduced in code for [issue 18](https://code.google.com/p/remote-testbed/issues/detail?id=18).
  * MCI: Fix --enable-mch handling in the configure script.
  * MCI: Fix dig528-2 platform typo.

## Version 1.0 ##

  * Create contrib areas that provide examples and other utilities.
  * DB, GUI, WS: Add small build systems.
  * MCI: Use Autoconf/Automake based buildsystem to ease building for small environments.
  * MCI: Switch the mote control host to use UDEV instead of hotplug.

## Version 0.9 ##

The basic infrastructure with DIKU specific mote control host and server. This version was developed on [sourceforge](http://www.sourceforge.net/projects/re-mote).