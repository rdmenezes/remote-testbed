# Makefile for updating the Re-Mote source documentation

REPODIR = $(PWD)
SVNDIR	= $(PWD)/checkout
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

update-svn:
	$(MAKE) -C $(SVNDIR) -f $(REPODIR)/Makefile update-mimetypes

update-mimetypes:
	svn update
	git --git-dir=$(REPODIR)/.git ls-files | while read file; do \
		type=; \
		case "$$file" in \
		*.html)	type=text/html 		;; \
		*.htm)	type=text/html 		;; \
		*.css)	type=text/css   	;; \
		*.gif)	type=text/gif   	;; \
		*.png)	type=image/png		;; \
		*.jpg)	type=image/jpege	;; \
		*.jpeg)	type=image/jpeg		;; \
		*.pdf)	type=application/pdf	;; \
		esac; \
		test "$$type" = "$$(svn propget svn:mime-type $$file)" && continue; \
		svn propset svn:mime-type $$type $$file; \
	done
	svn commit -m "Set mime-type properties for documentation files"
