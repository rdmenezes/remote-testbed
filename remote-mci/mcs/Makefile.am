bin_PROGRAMS = remote-mcs

INCLUDES = \
 $(MYSQL_CFLAGS) \
 -I/usr/include/mysql++

remote_mcs_SOURCES = \
 FileDescriptor.cc \
 FileDescriptor.h \
 Host.cc \
 Host.h \
 HostListener.cc \
 HostListener.h \
 MCIAddress.h \
 Mote.cc \
 Mote.h \
 MoteAddresses.cc \
 MoteAddresses.h \
 MoteControlInfrastructure.h \
 Session.cc \
 Session.h \
 SessionListener.cc \
 SessionListener.h \
 database.cc \
 database.h \
 moteserver.cc

remote_mcs_DEPENDENCIES = \
 $(top_builddir)/libutil/libutil.a \
 $(top_builddir)/protocols/libprotocols.a

remote_mcs_LDADD = \
 $(remote_mcs_DEPENDENCIES) \
 -lmysqlclient \
 -lmysqlpp

remote_mcs_LDFLAGS = \
 $(MYSQL_LDFLAGS)

include $(top_srcdir)/Makefile.lib
