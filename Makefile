CC = gcc
LINKERFLAG = -Wall -Wextra -pipe -pedantic

SRC := Team15.c
OUT := a.out
INPUT := input.txt

all: run clean

run: $(SRC) $(INPUT)
	@$(CC) $(SRC) $(LINKERFLAG)
	@./$(OUT) $(INPUT)

clean: $(OUT)
	@rm $(OUT)