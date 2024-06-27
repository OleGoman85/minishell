# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# Directories
SRCDIR = .
GNLDIR = ./gnl

# Source files
SRC = $(SRCDIR)/mini_here_doc.c \
      $(GNLDIR)/get_next_line.c \
      $(GNLDIR)/get_next_line_utils.c

# Header files
HEADERS = $(SRCDIR)/mini_here_doc.h \
          $(GNLDIR)/get_next_line.h

# Output executable
TARGET = mini_here_doc

# Build target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(SRC) $(HEADERS)
	@$(CC) $(CFLAGS) -o $@ $(SRC)

# Clean up build files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
