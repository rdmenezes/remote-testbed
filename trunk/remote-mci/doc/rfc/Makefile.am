RFC_HTML = \
 101-udev-device-management.html

SUFFIXES = .html
CLEANFILES = $(RFC_HTML)
EXTRA_DIST = *.txt

all-local: $(RFC_HTML)

.PHONY: html

.txt.html:
	$(ASCIIDOC) -b xhtml11 -o $@ $<

include $(top_srcdir)/Makefile.lib
