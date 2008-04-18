################################################################################
# Build section
################################################################################

JAVAC		= javac
JAVA		= java
JAR		= jar
JAVADOC		= javadoc
SCP		= scp
CTAGS		= ctags

GUILIBDIR	= lib
WSLIBDIR	= ../remote-ws/lib
BUILDDIR	= build
CLASSDIR	= $(BUILDDIR)/classes
BOOTDIR		= $(BUILDDIR)/boot
DOCDIR		= $(BUILDDIR)/doc
MAINJAR		= $(BOOTDIR)/main/main.jar
MAINSRC		= $(shell find src/ -name "*.java")
BOOTJAR		= boot/one-jar-boot-0.95.jar
BOOTMANIFEST	= boot/manifest.txt
REMOTEJAR	= Re-Mote.jar

WSJARS		= activation.jar axis.jar commons-discovery-0.2.jar \
		  commons-logging-1.0.4.jar jaxrpc.jar mail-1.4.jar \
		  remote-ws-client.jar saaj.jar wsdl4j-1.5.1.jar

WSLIBDIRJARS	= $(addprefix $(WSLIBDIR)/,$(WSJARS))
GUICLASSPATH	= $(shell find $(GUILIBDIR) -name "*.jar" | tr '\n' ':')
WSCLASSPATH	= $(shell find $(WSLIBDIR) -name "*.jar" | tr '\n' ':')
CLASSPATH	= $(WSCLASSPATH):$(GUICLASSPATH):$(MAINJAR):.

export CLASSPATH

all: $(REMOTEJAR)

clean:
	$(RM) -r $(BUILDDIR) $(REMOTEJAR) tags

tags:
	$(CTAGS) $(MAINSRC)

strip-space:
	perl -p -i -e 's/[ \t]*$$//' $(MAINSRC)

doc:
	$(JAVADOC) -d $(DOCDIR) $(MAINSRC)

test-build: $(MAINJAR)
	$(JAVA) remote.gui.ReMote

test-jar: $(REMOTEJAR)
	$(JAVA) -jar $(REMOTEJAR)

$(REMOTEJAR): $(MAINJAR) $(WSLIBDIRJARS)
	@mkdir -p $(BOOTDIR)/lib
	cd $(BOOTDIR) && $(JAR) xf $(PWD)/$(BOOTJAR)
	cp $(WSLIBDIRJARS) $(BOOTDIR)/lib
	$(JAR) cmf $(BOOTMANIFEST) $@ -C $(BOOTDIR) . && \
	$(JAR) uf $@ icons config lib || \
	$(RM) $@

$(MAINJAR): $(MAINSRC)
	@mkdir -p $(CLASSDIR) $(@D)
	$(JAVAC) -d $(CLASSDIR) $(MAINSRC)
	echo "Main-Class: remote.gui.ReMote" > $(@D)/manifest.txt
	$(JAR) cmf $(@D)/manifest.txt $@ -C $(CLASSDIR) .
