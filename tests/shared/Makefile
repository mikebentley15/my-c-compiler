CC           := clang
DEPDIR       := ../../shared
BOOTSTRAPDIR := ../../bootstrap
TESTHARNESS  := ../test_harness.c
CFLAGS       :=
CFLAGS       += -g
CFLAGS       += -I..
CFLAGS       += -I$(DEPDIR)
CFLAGS       += -I$(BOOTSTRAPDIR)
SOURCE       := $(wildcard *.c)
TARGETS      := $(SOURCE:%.c=%)
RUN_TARGETS  := $(TARGETS:%=run__%)

DEPS         :=
DEPS         += $(wildcard $(DEPDIR)/*.c)
DEPS         += $(wildcard $(DEPDIR)/*.h)
DEPS         += $(wildcard $(BOOTSTRAPDIR)/*.c)
DEPS         += $(wildcard $(BOOTSTRAPDIR)/*.h)
DEPS         += $(TESTHARNESS)

-include ../color_out.mk

.PHONY: all run clean
all: run $(TARGETS)
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
