| This project is not actively being developed anymore. |
|:------------------------------------------------------|

The Re-Mote Testbed Framework is a collection of modules for providing remote access to wireless sensor network testbeds with a main focus on modularity, abstraction levels, and responsiveness. The components of the framework fall into three
categories: Client software, server software and host/gateway software. The
client is a Java based GUI featuring mote console windows useful for
debugging applications. The server software consists of a mote server daemon
written in C++, a relational database based on MySQL, and a mote information
server based on Apache Tomcat and Axis. The host software is a mote host daemon written in C++ with site specific configuration in shell scripts.

## Getting Started ##

  * If you are **using the Re-Mote client** basic information is available in the UserManual.

  * If you are **deploying a testbed** see the AdministratorManual and read the SecurityConsiderations.

  * If you **want to contribute** read the DeveloperManual and take a look at the RoadMap to get an idea of what needs work.

## Questions and Bug Reporting ##

Questions can be mailed to the [mailing list](http://groups.google.com/group/remote-testbed). All discussions related to testbeds and sensor networks are welcome. Note, first time posters are moderated to prevent spam.

If you found a bug in the client or one of the other components, please enter it into the [issue tracker](http://code.google.com/p/remote-testbed/issues/list). If the bug has already been reported you are encouraged to add extra information as a comment. This will help to confirm that it is reproducible and raise the attention of developers.

## Credit ##

The project is being developed in collaboration between [DIKU](http://www.diku.dk/) and [CIT](http://www.cit.ie/) and headed by [Jonas Fonseca](http://www.diku.dk/hjemmesider/studerende/fonseca/) and [Rostislav Špinar](http://testbed.aws.cit.ie/rosta/). The initial work was done by [Jan Flora](http://www.nflora.dk/studie/) and Esben Zeuthen at DIKU under the supervision of [Philippe Bonnet](http://www.diku.dk/~bonnet/).

The Re-Mote Testbed Framework has been developed in the context of [Embedded WiSeNts](http://www.embedded-wisents.org/) and has further been developed in the context of the [CRUISE IST](http://www.ist-cruise.eu/cruise/).