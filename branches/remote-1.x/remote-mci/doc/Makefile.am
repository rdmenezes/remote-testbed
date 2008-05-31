txt1_DOCS = \
 remote-mch.1.txt \
 remote-mcs.1.txt

txt7_DOCS = \
 remote-device-add.7.txt \
 remote-device-name.7.txt \
 remote-device-remove.7.txt

man_DOCS = \
 remote-mch.1 \
 remote-mcs.1 \
 remote-device-add.7 \
 remote-device-name.7 \
 remote-device-remove.7

html_DOCS = \
 remote-device-add.7.html \
 remote-device-name.7.html \
 remote-device-remove.7.html \
 remote-mch.1.html \
 remote-mcs.1.html

EXTRA_DIST = \
 $(txt1_DOCS) \
 $(txt7_DOCS) \
 $(man_DOCS) \
 $(html_DOCS) \
 Doxyfile \
 asciidoc.conf

CLEANFILES = *.xml remote-*.7.txt
DISTCLEANFILES = *.1 *.7 *.html

ASCIIDOC_FLAGS = -f $(srcdir)/asciidoc.conf -a version=$(PACKAGE_VERSION) -a source=$(PACKAGE)

SUFFIXES: .txt .xml .html

html: $(html_DOCS)
man: $(man_DOCS)

code: Doxyfile
	$(DOXYGEN) Doxyfile

remote-doc: html
	test -z "$(DOCDIR)" || $(mkinstalldirs) -d $(DOCDIR)
	test -z "$(DOCDIR)" || $(INSTALL_DATA) $(html_DOCS) $(DOCDIR)

.PHONY: code html remote-doc 

$(txt7_DOCS): $(srcdir)/Makefile.am
	sed -n '2,/######/p' < $(top_srcdir)/contrib/lib/udev/$$(echo $@ | sed 's/.7.txt//') | \
	sed 's/^#* *//' | \
	sed '/^COPYRIGHT/,/^--------/s/^--------.*/&\n[verse]/' > $@

.txt.xml:
	sed 's/\(remote-[^(]*\)(\([^)]*\))/*\1(\2)*/g' < $< | \
	$(ASCIIDOC) $(ASCIIDOC_FLAGS) -b docbook -d manpage -o $@ -

.txt.html:
	sed 's/\(remote-[^(]*\)(\([^)]*\))/*link:\1.\2.html[\1(\2)]*/g' < $< | \
	$(ASCIIDOC) $(ASCIIDOC_FLAGS) -b xhtml11 -d manpage -o $@ -

.xml:
	$(XMLTO) man $<

include $(top_srcdir)/Makefile.lib
