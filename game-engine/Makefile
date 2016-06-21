OUT = game
INC_DIR = ./inc
SRC_DIR = ./src
OBJ_DIR = ./obj
C_FILES= $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES= $(addprefix $(OBJ_DIR)/,$(notdir $(C_FILES:.c=.o)))
PLATFORM = $(shell uname)


CC = clang
INCS = -I $(INC_DIR)
CFLAGS = $(INCS) -Wall -Werror -Wno-unused

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	LFLAGS = -lGL -lglfw -lm
endif

ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	LFLAGS = -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
endif

.PHONY all: clean $(OUT)

$(OUT): $(OBJ_DIR) $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(OUT) main.c $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJ_FILES) $(OBJ_DIR) $(OUT)

run: $(OUT)
	./$(OUT)
