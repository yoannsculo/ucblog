CC = gcc
BUILD_PATH = build
EXEC = $(BUILD_PATH)/ucblog
SRC = $(wildcard src/*.c) 
OBJ = $(SRC:src/%.c=$(BUILD_PATH)/%.o)

INCLUDES = -Iinclude \
	   -I/usr/include \
	   -I./tanoku-sundown-3c32220/src \
	   -I./tanoku-sundown-3c32220/html \
	   -I./ctemplate-1.0

CFLAGS = -W -Wall -g
LDFLAGS = -L/usr/lib -L$(BUILD_PATH) -lsundown

all: tmpl $(EXEC)

$(BUILD_PATH)/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

sundown:
	make -C tanoku-sundown-3c32220
	cp tanoku-sundown-3c32220/libsundown.so.1 $(BUILD_PATH)

ctemplate:
	make -C ctemplate-1.0

clean:
	-@(rm -f $(EXEC) $(OBJ))
	make -C tanoku-sundown-3c32220 clean
	make -C ctemplate-1.0 clean
