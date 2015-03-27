#summary Development roadmap and release plans
#labels Section-Project,Featured

# The Re-Mote Development Roadmap #

This pages provides an overview of the plans concerning all future milestones.  For each milestone, tasks are listed into related groups. Any significant task has an associated issue for tracking its progress and status; issues should always be created when a task involves several modules. Most details for tasks are available in the task's associated issue page.

Get an overview of the status of open issues and what needs work via the issue tracker's search page; links are provided for each milestone. Resolved issues are displayed with a "strike through" link.

To complement the roadmap, larger tasks and design changes requiring detailed investigation should be described and discussed in [enhancement proposal documents](http://code.google.com/p/remote-testbed/w/list?can=2&q=label%3ASection-Proposal&sort=pagename&colspec=PageName+Summary+Type+Status+RevNum+ChangedBy+Changed).

Roadmap for previous releases are available in the RoadMapArchive.

## Re-Mote Testbed Framework version 2.0 ##

Open issues sorted by [status](http://code.google.com/p/remote-testbed/issues/list?q=label:milestone-release2.0&can=2&x=component&y=status&mode=grid) or [priority](http://code.google.com/p/remote-testbed/issues/list?q=label:milestone-release2.0&can=2&x=component&y=priority&mode=grid). [All](http://code.google.com/p/remote-testbed/issues/list?q=label:milestone-release2.0&can=1) issues belonging to this milestone.

### Usability ###

  * [Issue 4](https://code.google.com/p/remote-testbed/issues/detail?id=4): Update state of control motes in the all motes pane. This will not require any round-trip time, only an update of data already in the client.

  * [Issue 12](https://code.google.com/p/remote-testbed/issues/detail?id=12): Add application to make testbed administration easier. Crucial for large testbeds.

  * [Issue 16](https://code.google.com/p/remote-testbed/issues/detail?id=16): This should both improve error handling during start-up and make it possible to configure MCS server and port via a dialog.

  * [Issue 26](https://code.google.com/p/remote-testbed/issues/detail?id=26): Add message board web service to announce testbed updates.

  * [Issue 36](https://code.google.com/p/remote-testbed/issues/detail?id=36): Add description field to testbed projects.

  * [Issue 41](https://code.google.com/p/remote-testbed/issues/detail?id=41): Add user interface and infrastructure for configuring clients to access new testbeds.

### Infrastructure ###

  * [Issue 11](https://code.google.com/p/remote-testbed/issues/detail?id=11): Reorganizing the files of the DB module will further improve the self-documenting hierarchy of this module.

  * [Issue 17](https://code.google.com/p/remote-testbed/issues/detail?id=17): Although this design approach will require some more initial thoughts and work, it will reduce the need for future changes wrt. mote attribute handling.

  * [Issue 19](https://code.google.com/p/remote-testbed/issues/detail?id=19): Use inotify in MCH to watch the device hierarchy

  * [Issue 22](https://code.google.com/p/remote-testbed/issues/detail?id=22): Make logging easier and more configurable.

  * [Issue 30](https://code.google.com/p/remote-testbed/issues/detail?id=30): Improve MCS mysql exception handling.

  * [Issue 31](https://code.google.com/p/remote-testbed/issues/detail?id=31): Replace boost::program\_options with small custom configuration parser

  * [Issue 32](https://code.google.com/p/remote-testbed/issues/detail?id=32): Restructure the remote-ws Java package namespace

  * [Issue 35](https://code.google.com/p/remote-testbed/issues/detail?id=35): Add sane defaults for newly created databases.

### Interoperability ###

  * [Issue 23](https://code.google.com/p/remote-testbed/issues/detail?id=23): Move mote device hierarchy to /var/run/remote.

  * [Issue 38](https://code.google.com/p/remote-testbed/issues/detail?id=38): Conditionally #include header files based on config/config.h.

## Re-Mote Testbed Framework version "future" ##

Open issues sorted by [status](http://code.google.com/p/remote-testbed/issues/list?q=label:milestone-future&can=2&x=component&y=status&mode=grid) or [priority](http://code.google.com/p/remote-testbed/issues/list?q=label:milestone-future&can=2&x=component&y=priority&mode=grid). [All](http://code.google.com/p/remote-testbed/issues/list?q=label:milestone-future&can=1) issues belonging to this milestone.

### Usability ###

  * [Issue 27](https://code.google.com/p/remote-testbed/issues/detail?id=27): Bring together different sites based on the Re-Mote infrastructure

  * [Issue 25](https://code.google.com/p/remote-testbed/issues/detail?id=25): Improve handling of defunct motes.

  * [Issue 37](https://code.google.com/p/remote-testbed/issues/detail?id=37): Add user 'roles' and a UI for users to manage projects themselves.

  * [Issue 39](https://code.google.com/p/remote-testbed/issues/detail?id=39): Support IP ranges for the hosts the MCS should accept.

  * [Issue 40](https://code.google.com/p/remote-testbed/issues/detail?id=40): Plugins for network embedded programming IDEs.

### Interoperability ###

  * [Issue 8](https://code.google.com/p/remote-testbed/issues/detail?id=8): MCH should support IPv6, however, as long as it is not requested (and thus there are no testers) this can be postponed.

### Infrastructure ###

  * [Issue 34](https://code.google.com/p/remote-testbed/issues/detail?id=34): Move to JAX-WS as the platform for client RPC