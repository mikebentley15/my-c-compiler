CC           := clang
CFLAGS       :=
CFLAGS       += -I../../scanner
CFLAGS       += -I../../bootstrap
SOURCE       := $(wildcard *.c)
TARGETS      := $(SOURCE:%.c=%)
RUN_TARGETS  := $(TARGETS:%=run__%)

.PHONY: all run clean
all: run $(TARGETS)
run: $(RUN_TARGETS)
clean:
	rm $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: run__%
run__%: %
	./$<
