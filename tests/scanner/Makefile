CC           := clang
TOPLEVEL     := ../..
DEPDIRS      :=
DEPDIRS      += $(TOPLEVEL)/shared
DEPDIRS      += $(TOPLEVEL)/scanner
BOOTSTRAPDIR := $(TOPLEVEL)/bootstrap
TESTHARNESS  := $(TOPLEVEL)/tests/test_harness.c
CFLAGS       :=
CFLAGS       += -g
CFLAGS       += -I$(TOPLEVEL)
CFLAGS       += -I$(dir $(TESTHARNESS))
CFLAGS       += $(addprefix -I,$(DEPDIRS))
CFLAGS       += -I$(BOOTSTRAPDIR)
SOURCE       := $(wildcard *.c)
TARGETS      := $(SOURCE:%.c=%)
RUN_TARGETS  := $(TARGETS:%=run__%)

DEPS         :=
DEPS         += $(foreach d,$(DEPDIRS),$(wildcard $(d)/*.c))
DEPS         += $(foreach d,$(DEPDIRS),$(wildcard $(d)/*.h))
DEPS         += $(wildcard $(BOOTSTRAPDIR)/*.c)
DEPS         += $(wildcard $(BOOTSTRAPDIR)/*.h)
DEPS         += $(TESTHARNESS)

-include ../color_out.mk

.PHONY: all run clean build
all: run $(TARGETS)
build: $(TARGETS)
run: $(RUN_TARGETS)
clean:
	rm -f $(TARGETS)

$(TARGETS): $(DEPS) Makefile

%: %.c
	$(call color_out,GREY,  $< -> $@)
	@$(CC) $(CFLAGS) -o $@ $<

.PHONY: run__%
run__%: %
	$(call color_out,BLUE,    $<)
	@./$<
