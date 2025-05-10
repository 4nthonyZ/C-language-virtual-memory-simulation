CC = gcc
CFLAGS = -Wall -g
EXE = translate

SRC = main.c task1.c task2.c task3.c task4.c

OBJ = $(SRC:.c=.o)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXE)

format:
	clang-format -style=file -i *.c *.h
