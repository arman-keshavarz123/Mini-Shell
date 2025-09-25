# Compiler and flags
CC = gcc
CFLAGS = -g -pedantic -std=gnu17 -Wall -Werror -Wextra
LDFLAGS = 

# Target executable
TARGET = nyush
SOURCE = nyush.c
OBJECT = nyush.o

# Default target
.PHONY: all
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECT)
	$(CC) $(OBJECT) -o $(TARGET) $(LDFLAGS)

# Compile source to object
$(OBJECT): $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE) -o $(OBJECT)

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(OBJECT) $(TARGET)

# Install (optional - copies to /usr/local/bin)
.PHONY: install
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall (optional - removes from /usr/local/bin)
.PHONY: uninstall
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Debug build with additional flags
.PHONY: debug
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build with optimization
.PHONY: release
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Check for memory leaks with valgrind (if available)
.PHONY: check
check: $(TARGET)
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "Running valgrind check..."; \
		valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET); \
	else \
		echo "Valgrind not found. Install valgrind to run memory checks."; \
	fi

# Show help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all      - Build the nyush shell (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  debug    - Build with debug flags"
	@echo "  release  - Build optimized release version"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  uninstall- Remove from /usr/local/bin"
	@echo "  check    - Run valgrind memory check"
	@echo "  help     - Show this help message"
