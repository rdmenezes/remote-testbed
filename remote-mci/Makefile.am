SUBDIRS = doc protocols

if CONFIG_DIKU_MCH
SUBDIRS += diku_mch
endif

if CONFIG_DIKU_MCS
SUBDIRS += mcs diku_mcs
endif

# TODO: At some point these should probably be moved to
# contrib/Makefile.am with the choice to autogenerate
# some of them depending on the configure scripts via
# AC_OUTPUT. --jonas
CONTRIB_EXTRA_DIST = \
 contrib/etc/* \
 contrib/etc/init.d/* \
 contrib/etc/udev/rules.d/* \
 contrib/etc/udev/scripts/*

EXTRA_DIST = \
 AUTHORS \
 COPYING \
 INSTALL \
 NEWS \
 README \
 TODO \
 bootstrap \
 $(CONTRIB_EXTRA_DIST)

doc doxygen:
	$(MAKE) -C doc code

rfc:
	$(MAKE) -C doc/rfc

ctags:
	find . -name "*.cc" -or -name "*.h" | ctags -L -

.PHONY: doc

include $(top_srcdir)/Makefile.lib
