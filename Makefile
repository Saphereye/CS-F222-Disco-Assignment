CC = gcc
FLAGS = -Wall -Wextra -Wunused -pipe -std=c99

SRC := Team15.c
OUT := a.out
INPUT1 := input1.txt
INPUT2 := input2.txt

all: run clean

run: $(SRC) $(INPUT)
	@$(CC) $(SRC) $(FLAGS)
	@./$(OUT) $(INPUT1) $(INPUT2)

clean: $(OUT)
	@rm $(OUT)
