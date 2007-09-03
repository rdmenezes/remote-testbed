# Makefile for updating the Re-Mote source documentation

GUIDIR	= ../remote-gui
WSDIR	= ../remote-ws
MCIDIR	= ../remote-mci

DOCDIRS = $(realpath $(GUIDIR) $(WSDIR) $(MCIDIR))

all:
	mkdir old
	mv remote-* old
	$(foreach docdir,$(DOCDIRS),$(MAKE) -C $(docdir) DOCDIR=$(PWD)/$(notdir $(docdir)) doc;)
	$(RM) -r old
	git add remote-*
	git commit -m "Update documentation"
