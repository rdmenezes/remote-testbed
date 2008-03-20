dist_man1_MANS = \
 remote-device-add.7 \
 remote-device-name.7 \
 remote-device-remove.7 \
 remote-mch.1 \
 remote-mcs.1

txt_DOCS = \
 remote-device-add.7.txt \
 remote-device-name.7.txt \
 remote-device-remove.7.txt \
 remote-mch.1.txt \
 remote-mcs.1.txt

html_DOCS = \
 remote-device-add.7.html \
 remote-device-name.7.html \
 remote-device-remove.7.html \
 remote-mch.1.html \
 remote-mcs.1.html

EXTRA_DIST = $(txt_DOCS) $(html_DOCS) Doxyfile asciidoc.conf

CLEANFILES = *.xml remote-*.7.txt
DISTCLEANFILES = *.1 *.html

ASCIIDOC_FLAGS = -f $(srcdir)/asciidoc.conf -a version=$(PACKAGE_VERSION) -a source=$(PACKAGE)

SUFFIXES: .txt .xml .html

html: $(html_DOCS)

code: Doxyfile
	$(DOXYGEN) Doxyfile

.PHONY: code html dist-local

remote-device-%.7.txt: $(top_srcdir)/contrib/lib/udev/remote-device-% $(srcdir)/Makefile.am
	sed -n '2,/######/p' < $< | \
	sed 's/^#* *//' | \
	sed '/^COPYRIGHT/,/^--------/s/^--------.*/&\n[verse]/' > $@

.txt.xml:
	sed 's/\(remote-[^(]*\)(\([^)]*\))/*\1(\2)*/g' < $< | \
	$(ASCIIDOC) $(ASCIIDOC_FLAGS) -b docbook -d manpage -o $@ -

.txt.html:
	sed 's/\(remote-[^(]*\)(\([^)]*\))/*link:\1.\2.html[\1(\2)]*/g' < $< | \
	$(ASCIIDOC) $(ASCIIDOC_FLAGS) -b xhtml11 -d manpage -o $@ -

.xml:
	$(XMLTO) man -o $(srcdir) $<

include $(top_srcdir)/Makefile.lib
