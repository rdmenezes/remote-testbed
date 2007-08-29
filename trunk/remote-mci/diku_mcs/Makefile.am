bin_PROGRAMS = diku_mcs

INCLUDES = \
 -I$(top_srcdir)/mcs \
 $(MYSQL_CFLAGS) \
 -I/usr/include/mysql++

diku_mcs_SOURCES = \
 Configuration.cc \
 Configuration.h \
 Host.cc \
 Host.h \
 HostListener.cc \
 HostListener.h \
 MoteAddresses.cc \
 MoteAddresses.h \
 moteserver.cc

diku_mcs_DEPENDENCIES = \
 $(top_builddir)/mcs/libmcs.a \
 $(top_builddir)/protocols/libprotocols.a

diku_mcs_LDADD = \
 $(diku_mcs_DEPENDENCIES) \
 -lmysqlclient \
 -lmysqlpp \
 $(BOOST_PROGRAM_OPTIONS_LIB)

diku_mcs_LDFLAGS = \
 $(MYSQL_LDFLAGS)

include $(top_srcdir)/Makefile.lib
