CC           := clang
CFLAGS       :=
CFLAGS       += -I../../shared
CFLAGS       += -I../../bootstrap
SOURCE       := $(wildcard *.c)
TARGETS      := $(SOURCE:%.c=%)
RUN_TARGETS  := $(TARGETS:%=run__%)

.PHONY: all run clean
all: run $(TARGETS)
run: $(RUN_TARGETS)
clean:
	rm -f $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: run__%
run__%: %
	./$<
