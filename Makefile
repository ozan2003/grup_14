# Grup üyeleri:
# - Eren Çoban - G221210090
# - Onur Eğrikılıç - G221210066
# - Ozan Malcı - B211210039
# - Semih Öztürk - G221210003
# - Yiğit Talha Adagülü - G221210093

CC := gcc
CCFLAGS := -g -std=c99 -Wall -Wextra -Wshadow -pedantic
# These folders must be present.
SRCDIR := ./src
INCDIR := ./include
BINDIR := ./bin
LIBDIR := ./lib
# The command that is used for deleting.
RM := rm
# You can customize the target executable name.
TARGET_EXEC := main

# wildcard function returns a space-separated list 
# of names of existing files that match one of the given file name patterns.
#
# SOURCES will be our .c files.
#
# * searches your filesystem for matching filenames.
SOURCES := $(wildcard $(SRCDIR)/*.c)

# $(patsubst pattern,replacement,text)
# The function patsubst replaces each sub-string of a string by another according to a pattern.
# % acts like a wildcard character. It's most often used in rule definitions and in some specific functions.
#
# OBJECTS will be our .o files correspondent with files in SOURCES (file.c -> file.o).
#
# % in a prerequisite/function of a pattern rule stands for the same stem(filename)
# that was matched by the % in the target.
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(LIBDIR)/%.o,$(SOURCES))

# -I is prepended to the front of each individual .hpp file
# with addprefix function.
INC_PARAMS := $(addprefix -I,$(INCDIR))

# $@ stands for the name of the target.
# $^ stands for the name of the dependencies, seperated by spaces.
# $< stands for the first name of the dependencies.
#
# Define a pattern rule that compiles every .o file with its correspondent .c file.
$(LIBDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CCFLAGS) $(INC_PARAMS) -c $< -o $@

# Link every .o file.
$(BINDIR)/$(TARGET_EXEC): $(OBJECTS)
	$(CC) $(CCFLAGS) $^ -o $@

# Phony targets are used if is not an actual file to be build. 
# They allow us to explicitly tell Make that they don't represent physical files.
.PHONY: all
all: $(BINDIR)/$(TARGET_EXEC)

.PHONY: run
run: $(BINDIR)/$(TARGET_EXEC)
	$(BINDIR)/$(TARGET_EXEC)

.PHONY: clean
clean:
	$(RM) $(BINDIR)/$(TARGET_EXEC) $(LIBDIR)/*.o
