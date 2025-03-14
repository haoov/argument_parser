# Project
TARGET	:=	argparser.a

# Compiler
CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror
CINC	:= -I $(INC_DIR)

# Files
INC_DIR	:= inc
SRC_DIR	:= src
OBJ_DIR	:= obj
BIN_DIR	:= bin
SRCS	:= $(wildcard $(SRC_DIR)/*.c)
OBJS	:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Rules
all : $(TARGET)

# Create library archive
$(TARGET) : $(OBJS) | $(BIN_DIR)
	ar -src $@ $^

# Compile to object files
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CINC) $< -o $@

# Create directory
$(OBJ_DIR) $(BIN_DIR) :
	mkdir -p $@

# Remove object files
clean :
	rm -rf $(OBJ_DIR)

# Remove bin file
fclean : clean
	rm -rf $(BIN_DIR)

re : fclean all

.PHONY : all clean fclean re

