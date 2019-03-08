TARGET := slunwrap
CCFLAGS := -Wall -ansi -pedantic

$(TARGET): $(TARGET).c
	gcc $(CCFLAGS) $< -o $@

clean:
	rm -f $(TARGET)

.PHONY: clean
