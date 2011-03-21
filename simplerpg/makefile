CC = g++

CFLAGS = -Wall -O2

COMPILE = $(CC) $(CFLAGS) -c -lm -lncurses -g

OBJFILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: myprog

myprog: $(OBJFILES)
	$(CC) -o myprog $(OBJFILES) -lncurses

%.o: %.cpp
	$(COMPILE) -o $@ $<

clean:
	rm *.o
	rm myprog
