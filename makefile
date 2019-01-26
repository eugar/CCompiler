CC=g++
CFLAGS=-g -Wall

targets=cexpression cscanner ccompiler
objects= cexpression.o cscanner.o

all:$(targets)

cexpression: cexpression.cpp
	$(CC) $(CFLAGS) -c $@.cpp

cscanner: cscanner.cpp
	$(CC) $(CFLAGS) -c $@.cpp

ccompiler: ccompiler.cpp
	$(CC) $(CFLAGS) $(objects) $@.cpp -o $@

clean:
	rm -rf $(targets) *.o
