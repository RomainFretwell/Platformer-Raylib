SOURCES := $(wildcard *.c)
HEAD := $(wildcard *.h)
TARGET := game
ZIPNAME := Platformer.zip

.PHONY: compile run clean cleanzip zip

compile: $(SOURCES) $(HEAD)
	gcc $(SOURCES) -lraylib -lm -o $(TARGET) -Wall -Wextra -O2

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)

cleanzip:
	rm -f $(ZIPNAME)

zip:
	zip -r $(ZIPNAME) . -x ".*" -x "$(ZIPNAME)" -x "$(TARGET)"