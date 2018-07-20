VPATH = src include
MKDIR = mkdir
INCLUDE_DIR = include

CC=gcc

OBJ_DIR=./objs
objects = socket_server.o car.o
OBJS = $(addprefix $(OBJ_DIR)/,$(objects))

LIB_DIR = ./libs
lib = libcar
LIB_S=$(addprefix $(LIB_DIR)/,$(lib))
LIB_STATIC=$(addsuffix .a,$(LIB_S))
LIB_D=$(addprefix $(LIB_DIR)/,$(lib))

EXAMPLE_DIR=./examples
examples = main
EXAMPLES=$(addprefix $(EXAMPLE_DIR)/,$(examples))


all:$(OBJ_DIR) $(LIB_DIR) $(LIB_STATIC) $(LIB_DYNAMIC) $(EXAMPLES)

$(OBJ_DIR):
	$(MKDIR) $@
$(LIB_DIR):
	$(MKDIR) $@
$(LIB_STATIC):$(OBJS)
	ar crs $@ $(OBJS)
$(LIB_DYNAMIC):$(OBJS)
	$(CC) -shared -Wall -fPIC $^ -o $@

$(OBJ_DIR)/%.o:src/%.c
	$(CC) -g -o $@ -c $^ -I$(INCLUDE_DIR)

$(EXAMPLE_DIR)/main:examples/main.c
	$(CC) -g -o $@  $< -I$(INCLUDE_DIR) -L./libs -lcar

clean:
	$(RM) $(EXAMPLES) libs/* objs/*
