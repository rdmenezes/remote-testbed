# Makefile for updating the Re-Mote source documentation

GUIDIR	= ../remote-gui
WSDIR	= ../remote-ws
#MCIDIR	= ../remote-mci

DOCDIRS = $(realpath $(GUIDIR) $(WSDIR) $(MCIDIR))

all:
	mkdir old
	mv remote-* old
	$(foreach docdir,$(DOCDIRS),$(MAKE) -C $(docdir) DOCDIR=$(PWD)/$(notdir $(docdir)) doc;)
	$(RM) -r old
	git diff --name-only | \
	while read file; do \
		git diff --stat $$file | grep -q "1 insertions(+), 1 deletions(-)" || continue; \
		git checkout HEAD $$file; \
	done
	git add remote-*
	git commit -m "Update documentation"
