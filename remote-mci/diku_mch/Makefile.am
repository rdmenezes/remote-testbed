bin_PROGRAMS = diku_mch

diku_mch_SOURCES = \
 Configuration.cc \
 Configuration.h \
 DeviceManager.cc \
 DeviceManager.h \
 Mote.cc \
 Mote.h \
 MoteHost.cc \
 MoteHost.h \
 SerialControl.cc \
 SerialControl.h

diku_mch_DEPENDENCIES = \
 $(top_builddir)/libutil/libutil.a \
 $(top_builddir)/protocols/libprotocols.a

diku_mch_LDADD = \
 $(diku_mch_DEPENDENCIES) \
 $(BOOST_PROGRAM_OPTIONS_LIB) \
 $(BOOST_REGEX_LIB)

SUBDIRS	= plugpiper

include $(top_srcdir)/Makefile.lib
