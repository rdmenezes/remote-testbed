AUTOMAKE_OPTIONS = foreign dist-zip

SUBDIRS = doc libutil protocols

if CONFIG_MCH
SUBDIRS += mch
endif

if CONFIG_MCS
SUBDIRS += mcs
endif

# TODO: At some point the contrib EXTRA_DIST should probably be moved to
# contrib/Makefile.am with the choice to autogenerate some of them
# depending on the configure scripts via AC_OUTPUT. --jonas

EXTRA_DIST = \
 COPYING \
 INSTALL \
 README \
 bootstrap \
 contrib

doc doxygen:
	$(MAKE) -C doc code

ctags:
	find . -name "*.cc" -or -name "*.h" | ctags -L -

.PHONY: doc

include $(top_srcdir)/Makefile.lib
