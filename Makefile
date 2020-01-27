CC = gcc
MAIN = bin/main
INCL_DIR = include
DATA_DIR = data
SRC_DIR = src
OBJ_DIR = lib

LIBS = -lm -lasound -lncurses -pthread  
INCLUDES := $(wildcard $(INCL_DIR)/*.h) $(wildcard $(DATA_DIR)/*.dat)
CFLAGS = -Wall -I$(INCL_DIR)/ -I$(DATA_DIR)/ 

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all
all: $(OBJS)
	$(CC) -o $(MAIN) $? $(CFLAGS) $(LIBS)

$(OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDES)
	$(CC) -c -o $@ $< $(LIBS) $(CFLAGS)

clean:
	rm main
