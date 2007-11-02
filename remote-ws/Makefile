################################################################################
# Configuration section
################################################################################
#
# Note, the following configuration settings can be put in a file called
# Makefile.local that is automatically sourced by this Makefile. See
# contrib/Makefile.local.* for examples.

## Webservice address information
#
# Information about the location of the webservice. It will be used when
# generating the client stubs from the WSDL files.

SERVICEHOST	= localhost
SERVICEPORT	= 8080
SERVICEPATH	= axis/services

## Webservice admin service information
#
# Information about the location and credentials for accessing the
# webservice framework with the AdminClient. Test the information using
# the deploy-test rule, which will try to contact the server and list
# the available webservices.
#
# To use the AdminClient program's default values, leave the variables
# empty.

ADMINHOST	= localhost
ADMINPORT	= 8080
ADMINUSER	=
ADMINPASS	=

## Location of the server class files
#
# The host and path specifying where the web service application
# infrastructure classes should be deployed. Note, that scp is used for
# this, so the DEPLOYLOCATION variable should hold a valid scp location.
#
# Leave DEPLOYHOST empty to deploy files locally.

DEPLOYHOST	=
DEPLOYPATH	= /path/to/tomcatX.X/webapps/axis/WEB-INF/classes/

################################################################################
# Build section
################################################################################

JAVAC		= javac
JAVA		= java
JAR		= jar
JAVADOC		= javadoc
SCP		= scp
CTAGS		= ctags

LIBDIR		= lib
WSDDDIR		= wsdd
BUILDDIR	= build
SERVERSRCDIR	= src
CLIENTSRCDIR	= $(BUILDDIR)/src
SERVERBINDIR	= $(BUILDDIR)/server
CLIENTBINDIR	= $(BUILDDIR)/client
WSDLDIR		= $(BUILDDIR)/wsdl
DOCDIR		= $(BUILDDIR)/doc

CLASSPATH	= $(shell find $(PWD)/$(LIBDIR) -name "*.jar" | tr '\n' ':')$(PWD)/$(SERVERJAR)

COMPILE		= $(JAVAC)
JAVA2WSDL	= $(JAVA) org.apache.axis.wsdl.Java2WSDL
WSDL2JAVA	= $(JAVA) org.apache.axis.wsdl.WSDL2Java
ADMINCLIENT	= $(JAVA) org.apache.axis.client.AdminClient

SERVERSRC	= $(shell find $(SERVERSRCDIR) -name "*.java")
SERVERJAR	= $(LIBDIR)/remote-ws-server.jar

WSDDFILES	= $(shell find $(WSDDDIR) -name "*.wsdd")
SERVICES	= $(patsubst $(WSDDDIR)/%.wsdd,%,$(WSDDFILES))

WSDLFILES	= $(patsubst %,$(WSDLDIR)/%.wsdl,$(SERVICES))
CLIENTSRC	= $(patsubst %,$(CLIENTSRCDIR)/%-stamp,$(SERVICES))
CLIENTJAR	= $(LIBDIR)/remote-ws-client.jar

DOCTITLE	= "Re-Mote Web Services"

## Handle local configuration settings
#
# ... and assemble the location variables

-include Makefile.local

SERVICELOCATION = http://$(SERVICEHOST):$(SERVICEPORT)/$(SERVICEPATH)
DEPLOYLOCATION	= $(DEPLOYHOST)$(if $(DEPLOYHOST),:)$(DEPLOYPATH)

## Export the CLASSPATH to avoid setting it on the command line
#
# This can potentially overwrite local CLASSPATH configuration but as
# far as remote-ws is concerned it should be self contained.

export CLASSPATH

## Basic build rules

all: client

clean:
	$(RM) -r $(BUILDDIR) $(SERVERJAR) $(CLIENTJAR) tags

tags:
	$(CTAGS) $(SERVERSRC)

strip-space:
	perl -p -i -e 's/[ \t]*$$//' $(SERVERSRC) $(WSDDFILES)

doc:
	$(JAVADOC) -doctitle $(DOCTITLE) -windowtitle $(DOCTITLE) \
		   -d $(DOCDIR) $(SERVERSRC)

.PHONY: all doc clean strip-space

## Rules for building the server and client class files
#
# This requires the immediate step of building WSDL files from the
# server Java classes. The client Java classes is then generated from
# the WSDL files.

wsdl: $(WSDLFILES)
client-src: $(CLIENTSRC)
client: $(CLIENTJAR)
server: $(SERVERJAR)

$(SERVERJAR): $(SERVERSRC)
	@$(RM) -r $(SERVERBINDIR)
	@mkdir -p $(SERVERBINDIR)
	$(COMPILE) -d $(SERVERBINDIR) $(SERVERSRC)
	$(JAR) cf $@ -C $(SERVERBINDIR) .

$(CLIENTJAR): $(CLIENTSRC)
	@$(RM) -r $(CLIENTBINDIR)
	@mkdir -p $(CLIENTBINDIR)
	$(COMPILE) -d $(CLIENTBINDIR) $(shell find $(CLIENTSRCDIR) -name "*.java")
	$(JAR) cf $@ -C $(CLIENTBINDIR) .

$(CLIENTSRCDIR)/%-stamp: $(WSDLDIR)/%.wsdl
	$(WSDL2JAVA) -o $(CLIENTSRCDIR) -pdiku.distlab.remote.$(*F) $<
	touch $@

$(WSDLDIR)/%.wsdl: $(WSDDDIR)/%.wsdd $(SERVERJAR)
	@mkdir -p $(WSDLDIR)
	$(JAVA2WSDL) -l $(SERVICELOCATION)/$(*F) -o $(WSDLDIR)/$(*F).wsdl \
		-p"$(*F)=$(*F).remote.distlab.diku" \
		"$(*F).$$(sed -n 's/.*className.*value="[^.]*\.\(.*\)".*/\1/p' < $<)"

$(BUILDDIR)/undeploy.wsdd:
	@mkdir -p $(CLIENTBINDIR)
	{ echo '<undeployment xmlns="http://xml.apache.org/axis/wsdd/">'; \
	  for i in $(SERVICES); do echo "<service name=\"$$i\"/>"; done; \
	  echo '</undeployment>'; } > $@

## Deployment rules
#
# Rules for deploying the Re-Mote webservices and for testing the setup.

RUNADMINCLIENT = $(ADMINCLIENT) \
		 $(if $(ADMINHOST),-h $(ADMINHOST)) \
		 $(if $(ADMINPORT),-p $(ADMINPORT)) \
		 $(if $(ADMINUSER),-u $(ADMINUSER)) \
		 $(if $(ADMINPASS),-w $(ADMINPASS))

deploy-test:
	$(RUNADMINCLIENT) list

deploy-wsdd:
	$(RUNADMINCLIENT) $(WSDDFILES)

deploy-jar: $(SERVERJAR)
	$(SCP) $< $(DEPLOYLOCATION)

deploy: deploy-jar deploy-wsdd

undeploy: $(BUILDDIR)/undeploy.wsdd
	$(RUNADMINCLIENT) $<
