# see https://yuukidach.github.io/2019/08/05/makefile-learning/
# Variable setup
CC:=gcc
CFLAGS:=-g -Ofast -Wall -Wextra -fsanitize=address -fsanitize=leak -std=gnu11
LIBS:=-lSDL2 -lm -lSDL2_ttf
VPATH:=conway gfx

BIN = bin

# Get source and object
SRCS = $(filter-out $(wildcard */*_tests.c */*test.c */*tests.c */*_test.c), $(wildcard *.c */*.c))
OBJS = $(addprefix $(BIN)/, $(SRCS:.c=.o))
SRCS_TEST = $(filter-out $(wildcard main.c */main.c gfx/*), $(wildcard *.c */*.c))
OBJS_TEST = $(addprefix $(BIN)/, $(SRCS_TEST:.c=.o))

# Create the target
main: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^ $(LIBS) $(LDFLAGS)
	./$(BIN)/$@

# Convert the source in object, but before all, run `$(BIN)` aka mkdir
$(BIN)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS) -lSDL2

doc:
	mkdir -p ./doc/bin
	pdflatex -output-directory ./doc/bin ./doc/report.tex

# Echo the source and object values
help:
	@echo "src: $(SRCS)"
	@echo "obj: $(OBJS)"
	@echo "obj_test: $(OBJ_TEST)"
	
clean:
	rm -rf $(BIN)
	
.PHONY: help clean main doc