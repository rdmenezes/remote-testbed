#summary Where to get the latest source code
#labels Section-Project

The source code is available either as [archives](http://code.google.com/p/remote-testbed/downloads/list?q=label:Type-Archive) or using one of the repositories listed below. Generally, the released archives are more stable, but may lack important features and fixes. If you want to keep in sync with the latest source code, tracking one of the repository below is recommended. It also allows you to easily keep track of local changes and makes it easy to contribute code back to the project.

**Google Code SVN Repository**

> The SVN repository hosted by Google Code is not used for day to day development, but serves mainly as an additional and convenient method of distribution. It is periodically [synchronized](SvnSynchronization.md) with one of the [git](http://git-scm.org/) repositories.

> Use this command to anonymously check out the latest project source code:
```
svn checkout http://remote-testbed.googlecode.com/svn/trunk/ remote-testbed
```

> You may also want to _[browse the code](http://code.google.com/p/remote-testbed/source/browse/trunk)_, or _[view recent changes](http://code.google.com/p/remote-testbed/source/list)_.

**DIKU Repositories**

> The DIKU git [repositories](http://repo.or.cz/w/remote.git) serves as the main integration place for new feature. They are hosted by [repo.or.cz](http://repo.or.cz/), where you can browse the code and view recent changes in the [remote-db](http://repo.or.cz/w/remote/remote-db.git), [remote-ws](http://repo.or.cz/w/remote/remote-ws.git), [remote-mci](http://repo.or.cz/w/remote/remote-mci.git), and [remote-gui](http://repo.or.cz/w/remote/remote-gui.git) gitweb pages.

> Use this command to anonymously clone and check out the latest project source code for a given repository:
```
git clone git://repo.or.cz/remote/remote-$repo.git
```


**CIT Repositories**

> Rosta maintains git repositories with CIT specific changes at http://testbed.aws.cit.ie/dev/remote/.

> Use the following command to anonymously clone and check out the latest project source code for a given repository:
```
git clone http://testbed.aws.cit.ie/dev/remote/remote-$repo.git
```