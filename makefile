SRCS = main.c calculator.c symboltable.c list.c shunting-yard.c dict.c stack.c tokenizer.c
OBJS = $(SRCS:%.c=%.o)
CC = gcc
PROG = calc
CFLAGS = -Wall -Wextra -Wmissing-prototypes --pedantic -std=c99 -g
LDFLAGS = -lm

.PHONY: all clean run

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(PROG)

run: $(PROG)
	./$(PROG)

calculator.o: calculator.c tokenizer.h calculator.h symboltable.h \
  shunting-yard.h
dict.o: dict.c dict.h
list.o: list.c list.h
main.o: main.c calculator.h
shunting-yard.o: shunting-yard.c shunting-yard.h tokenizer.h \
  symboltable.h stack.h
stack.o: stack.c stack.h
symboltable.o: symboltable.c symboltable.h dict.h list.h
tokenizer.o: tokenizer.c tokenizer.h
