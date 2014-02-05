# Include functions
include $(SCRIPTSDIR)/Makefile.functions

all: doc

doc:
	doxygen $(DOXYFILE)

distclean:
	@$(call remove_file,$(DOXYFILE))
	@$(call remove_dir,$(DOCDIR)/html)
