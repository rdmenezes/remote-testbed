bin_PROGRAMS = remote-mch

remote_mch_SOURCES = \
 DeviceManager.cc \
 DeviceManager.h \
 Mote.cc \
 Mote.h \
 MoteHost.cc \
 MoteHost.h \
 SerialControl.cc \
 SerialControl.h

remote_mch_DEPENDENCIES = \
 $(top_builddir)/libutil/libutil.a \
 $(top_builddir)/protocols/libprotocols.a

remote_mch_LDADD = \
 $(remote_mch_DEPENDENCIES)

SUBDIRS	= notify
EXTRA_DIST = README

include $(top_srcdir)/Makefile.lib
