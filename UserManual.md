﻿#summary Manual for using the Re-Mote client
#labels Section-Manual,Featured

# Re-Mote User Manual #

This manual for the Re-Mote client tries to give a very basic introduction by presenting some simple examples accompanied by screenshots.



## Installing and Running the Re-Mote Client ##

The Re-Mote Client is provided as an executable .jar archive and can be downloaded from the [download area](http://code.google.com/p/remote-testbed/downloads/list?can=2&q=label:Type-Executable). You should generally always choose the latest version; the one with the highest version number. Prerelease versions may exists that should be considered less stable. You are welcome to help test these! Also, remember to check back for occasional updates.

To run the client you need to have the Java runtime installed on your system. It has been tested with the Sun Java 6 distribution for Linux and Windows, other Java versions may also work.

When you have downloaded the client .jar file and checked that Java is installed, you should be able to run the client by simply double clicking on the .jar file. Alternatively, get a command prompt and type:
```
    $ java -jar Re-Mote.jar
```

This should invoke Java and run the client. If not, check your Java installation. Please report any Java problems you may encounter. For instructions on how to do this see the end of this document.

## Getting Started ##

Upon startup, the client presents a login window. Use the access credentials you got from the Re-Mote administrator. Next time you log on, the client will remember the name of the project and user you have previously used.

![http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/authenticate.png](http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/authenticate.png)

Once the client runs, you are presented with an overview of all the motes in the testbed. You are able to change the way motes are sorted so you can quickly find the ones you want. By default the motes are sorted by their net address, which will often be the same as the TOS address if you are using TinyOS. An icon displays which platform each mote has.

![http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/all-motes.png](http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/all-motes.png)

You can take control of motes by dragging selected motes from the "All motes" view onto the "Control motes" view. In the following example, control of mote 3, 4, and 7 has been acquired. Before taking control you may want to refresh the "All motes" view to get a fully updated view of the available motes.

![http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/controlled-motes.png](http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/controlled-motes.png)

Consoles can be opened for each of the motes you control. From the console windows, you can view mote console output and type input. Also, you can enable local logging for mote console output.

![http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/mote-console.png](http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/mote-console.png)

You can program, start, stop and reset any motes you control using the icons in either the mote console or the "Controlled motes" view depending on whether you want the command to be performed for one or more motes you have selected.

![http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/programming.png](http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/programming.png)

To release control of a mote simply mark it in the "Controlled motes" view and press the trash can icon.

![http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/release-mote.png](http://remote-testbed.googlecode.com/svn/documentation/images/screenshots/release-mote.png)

## Configuration File ##

The client automatically creates a configuration file called **.re-mote**. This file is used for keeping information about which testbed server and port the client should connect to on startup. It also stores the most recent project and user name for easy later authorization.

## Reporting Bugs and Problems ##

If you find a bug or you stumble upon a problem, please take the time to report it. The easiest way to report bugs is to visit the  project site and [enter a new issue](http://code.google.com/p/remote-testbed/issues/entry) into the issue tracker. Other problems are best reported to the [discussion forum](http://groups.google.com/group/remote-testbed), where questions can also be posted.