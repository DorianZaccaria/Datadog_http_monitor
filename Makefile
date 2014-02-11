##
# Edit the following variables to tune your project
##

# The project name, the compilation result will be store at the project
# directory (the root of the project) under this name
PROJECTNAME = DatadogMonitor

#
# The version of the project: Version.Subversion-Revision
PROJECTVERSION = 0
PROJECTSUBVERSION = 1
PROJECTREVISION = 0

# The installation directory
INSTALLDIR = /usr/local
BININSTALLDIR = $(INSTALLDIR)/bin
INCLUDEINSTALLDIR = $(INSTALLDIR)/include/$(PROJECTNAME)
LIBINSTALLDIR = $(INSTALLDIR)/lib
DOCINSTALLDIR = $(INSTALLDIR)/doc

# The tarball will be created under that name
PROJECT_TARNAME = $(PROJECTNAME)-v$(PROJECTVERSION)-$(PROJECTSUBVERSION).$(PROJECTREVISION)
# The archiving type tar.bz2
PROJECT_TARTYPE = tar.bz2

EXTRA_CPPFLAGS = -std=c++11 # -g -ggdb3
EXTRA_LDFLAGS = -lpcap -std=c++11
# END OF EDIT/CUSTOMISATION AREA
##

PROJECTDIR=${PWD}
SOURCEDIR =$(PROJECTDIR)/src
INCLUDEDIR=$(PROJECTDIR)/src/include
SCRIPTSDIR=$(PROJECTDIR)/scripts
LIBSDIR=$(PROJECTDIR)/lib
REPOSDIR=$(PROJECTDIR)/repo
BINDIR=$(PROJECTDIR)
DOCDIR=$(PROJECTDIR)/doc

DOXYFILE = $(DOCDIR)/Doxyfile

CPPFLAGS  = -W -Wall -Wextra
CPPFLAGS += -Werror
CPPFLAGS += $(EXTRA_CPPFLAGS)
CPPFLAGS += -I$(INCLUDEDIR) -I$(LIBSDIR)/include

LDFLAGS = $(EXTRA_LDFLAGS)

MAKEOPT  = --no-print-directory
MAKEOPT += CPPFLAGS="$(CPPFLAGS)"
MAKEOPT += PROJECTDIR="$(PROEJCTDIR)"
MAKEOPT += DOCDIR="$(DOCDIR)"
MAKEOPT += LDFLAGS="$(LDFLAGS)"
MAKEOPT += PROJECTNAME="$(BINDIR)/$(PROJECTNAME)"
MAKEOPT += CONFIG_HEADER_FILE="$(CONFIG_HEADER_FILE)"
MAKEOPT += SCRIPTSDIR="$(SCRIPTSDIR)"
MAKEOPT += DOCDIR="$(DOCDIR)"
MAKEOPT += DOXYFILE="$(DOXYFILE)"

TARBALL= $(PROJECT_TARNAME).$(PROJECT_TARTYPE)

include $(SCRIPTSDIR)/Makefile.functions

# BUILDING

all: $(SOURCEDIR)
	@make $(MAKEOPT) -C $(SOURCEDIR) all

build: doc all

$(DOXYFILE):
	$(SCRIPTSDIR)/generate_doxyfile.sh $(SOURCEDIR) $(DOCDIR) $(DOXYFILE)


help:
	@echo ""
	@echo "$(PROJECTNAME)'s Makefile targets help:"
	@echo ""
	@echo " -BUILDING-"
	@echo ""
	@echo "build:   generate compilation and documentation"
	@echo ""
	@echo "all:     compile the project and place the binary in"
	@echo "         $(BINDIR)/$(PROJECTNAME)"
	@echo ""
	@echo "doc:     generate the documentation of the source project in"
	@echo "         $(DOCDIR)"
	@echo ""
	@echo " -RELEASING-"
	@echo ""
	@echo "dist:    generate a project tarball: $(TARBALL)"
	@echo ""
	@echo " -CLEANING-"
	@echo ""
	@echo "clean: remove compilation files"
	@echo ""
	@echo "distclean: clean the project (target clean) and remove generated files"
	@echo ""

# DOCUMENTATION
.PHONY: doc
doc: $(DOXYFILE)
	@make $(MAKEOPT) -C $(DOCDIR) doc

# RELEASING

dist: distclean $(PROJECT_TARTYPE)

tar.bz2:
	tar -cvjf /tmp/$(TARBALL) --exclude=$(TARBALL) --exclude=.git ../`basename ${PWD}` \
	  && mv /tmp/$(TARBALL) .

# CLEANING

docclean:
	@make $(MAKEOPT) -C $(DOCDIR) distclean

clean: $(SOURCEDIR)
	@make $(MAKEOPT) -C $(SOURCEDIR) $@
	@$(call remove_file,$(BINDIR)/$(PROJECTNAME))

distclean: clean docclean
	@$(call remove_file,$(PROJECTDIR)/.init)
	@$(call remove_file,$(TARBALL))
	@$(call clean_dir,$(LIBSDIR))

# MISCS

$(INCLUDEDIR):
	@echo "No include directory at $(INCLUDEDIR)"
$(SOURCEDIR):
	@echo "No sources available at $(SOURCEDIR)"
$(REPOSDIR):
	@echo "No sources available at $(REPOSDIR)"
