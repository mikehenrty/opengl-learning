OUT = run
INC_DIR = ./inc
SRC_DIR = ./src
OBJ_DIR = ./obj
C_FILES= $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES= $(addprefix $(OBJ_DIR)/,$(notdir $(C_FILES:.c=.o)))

CC = clang
INCS = -I $(INC_DIR)
CFLAGS = $(INCS) -Wall -Werror -Wno-unused
LFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -lglfw3

$(OUT): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(OUT) main.c $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(OBJ_FILES) run
