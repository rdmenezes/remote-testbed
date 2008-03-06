SUBDIRS = doc libutil protocols

if CONFIG_DIKU_MCH
SUBDIRS += diku_mch
endif

if CONFIG_DIKU_MCS
SUBDIRS += mcs diku_mcs
endif

# TODO: At some point the contrib EXTRA_DIST should probably be moved to
# contrib/Makefile.am with the choice to autogenerate some of them
# depending on the configure scripts via AC_OUTPUT. --jonas

EXTRA_DIST = \
 AUTHORS \
 COPYING \
 INSTALL \
 NEWS \
 README \
 TODO \
 bootstrap \
 contrib

doc doxygen:
	$(MAKE) -C doc code

rfc:
	$(MAKE) -C doc/rfc

ctags:
	find . -name "*.cc" -or -name "*.h" | ctags -L -

.PHONY: doc

include $(top_srcdir)/Makefile.lib
