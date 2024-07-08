#
# Makefile
#

CC=gcc
TARGET=wordmorph
CFLAGS= -ansi -Wall -O3
LIBS= -lm

SRCFILES= main.c funcoes.c lista_adj.c acervo.c
INCFILES= funcoes.h
OBJFILES= main.o funcoes.o lista_adj.o acervo.o

default: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJFILES) core.* #*

main.o: funcoes.h

funcoes.o: funcoes.h


lista_adj.o: funcoes.h

acervo.o:funcoes.h