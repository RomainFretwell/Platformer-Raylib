CC := gcc
CFLAGS := -Wall -Wextra -O2
LDFLAGS := -lraylib -lm
SOURCES := main.c constants.c draw.c math2.c
TARGET := main

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

.PHONY: run clean

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)