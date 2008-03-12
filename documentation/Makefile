# Makefile for updating the Re-Mote source documentation

REPODIR = $(PWD)
GUIDIR	= ../remote-gui
WSDIR	= ../remote-ws
#MCIDIR	= ../remote-mci

DOCDIRS = $(realpath $(GUIDIR) $(WSDIR) $(MCIDIR))

update-doc:
	git rm -r remote-*
	$(foreach docdir,$(DOCDIRS),$(MAKE) -C $(docdir) DOCDIR=$(PWD)/$(notdir $(docdir)) doc;)
	git add remote-*
	-git ls-tree -r --name-only HEAD | while read file; do \
		git diff HEAD --stat $$file | grep -q "2 insertions(+), 2 deletions(-)" && git checkout HEAD $$file; \
		git diff HEAD --stat $$file | grep -q "1 insertions(+), 1 deletions(-)" && git checkout HEAD $$file; \
	done
	git diff && git commit -m "Update documentation"

update-git:
	git svn dcommit

update-svn:
	cd $(REPODIR)/svn
	svn update

update-properties:
	find . | grep -v .svn | while read file; do \
		*.html)	svn propset svn:mime-type text/html $$file	;; \
		*.htm)	svn propset svn:mime-type text/html $$file	;; \
		*.css)	svn propset svn:mime-type text/css $$file	;; \
		*.gif)	svn propset svn:mime-type text/gif $$file	;; \
		*.png)	svn propset svn:mime-type image/png $$file	;; \
		*.jpg)	svn propset svn:mime-type image/jpeg $$file	;; \
		*.jpeg)	svn propset svn:mime-type image/jpeg $$file	;; \
		*.pdf) svn propset svn:mime-type application/pdf $$file	;; \
	done
	svn commit -m "Set mime-type properties for documentation files"
	cd $(REPODIR)
