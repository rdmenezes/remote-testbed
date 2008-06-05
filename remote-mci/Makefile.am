AUTOMAKE_OPTIONS = foreign dist-zip

SUBDIRS = src

DIST_SUBDIRS = doc $(SUBDIRS)

dist_man1_MANS = \
 doc/remote-mch.1 \
 doc/remote-mcs.1

dist_man7_MANS = \
 doc/remote-device-add.7 \
 doc/remote-device-name.7 \
 doc/remote-device-remove.7

# TODO: At some point the contrib EXTRA_DIST should probably be moved to
# contrib/Makefile.am with the choice to autogenerate some of them
# depending on the configure scripts via AC_OUTPUT. --jonas

EXTRA_DIST = \
 COPYING \
 INSTALL \
 README \
 bootstrap \
 contrib

doc:
	$(MAKE) -C doc remote-doc DOCDIR=$(DOCDIR)

doxygen:
	$(MAKE) -C doc code

ctags:
	find . -name "*.cc" -or -name "*.h" | ctags -L -

.PHONY: doc

include $(top_srcdir)/Makefile.lib
