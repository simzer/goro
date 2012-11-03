
cc=gcc

bin = ttt

sources = ai.c \
    board.c \
    boarditerator.c \
    game.c \
    ttt.c \
    ui.c

objects = $(sources:.c=.o) 

cflags = -Wall -Wextra -g -W

libs =

all : $(bin)

clean:
	rm -rf $(bin) $(objects)

$(bin) : $(objects)
	$(cc) $(cflags) -o $(bin) $(objects) $(libs)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $< 
