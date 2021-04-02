# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2020-21

CC   = gcc
LD   = gcc
CFLAGS = -g  -O3 -ansi -Wall
LDFLAGS=-lm

# A phony target is one that is not really the name of a file
# https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
.PHONY: all clean run

all: project

project:  main.o 
	$(LD) $(CFLAGS) $(LDFLAGS) -o project  main.o 

main.o: main.c 
	$(CC) $(CFLAGS) -o main.o -c main.c

clean:
	@echo Cleaning...
	rm -f *.o project

run: project
	./project
