SUBMAKES     := $(wildcard */Makefile)
SUBMAKES     += $(wildcard */makefile)
SUBDIRS      := $(sort $(dir $(SUBMAKES)))
SUBRUNS      := $(SUBDIRS:%=run__%)
SUBCLEANS    := $(SUBDIRS:%=clean__%)

-include color_out.mk

.PHONY: all
all: $(SUBRUNS)
	$(call color_out,GREEN,"All tests pass")

.PHONY: clean
clean: $(SUBCLEANS)

.PHONY: run__%
run__%:
	$(MAKE) --directory $*

.PHONY: clean__%
clean__%:
	$(MAKE) clean --directory $*
