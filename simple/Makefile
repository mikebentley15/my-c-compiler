CC         = gcc
SOURCE     = simple-compiler.c
TARGET     = simple-compiler
CFLAGS     = -g

.PHONY: all run clean
all: $(TARGET)
run: $(TARGET)
	./$(TARGET) < $(SOURCE)
clean:
	rm -f $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
