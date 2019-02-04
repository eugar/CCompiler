CC=g++
CFLAGS=-g -Wall

ccom=ccompiler
targets=cexpression cscanner cparser
objects= cexpression.o cscanner.o cparser.o

all:$(targets) $(ccom)

cexpression: cexpression.cpp
	$(CC) $(CFLAGS) -c $@.cpp

cscanner: cscanner.cpp
	$(CC) $(CFLAGS) -c $@.cpp

cparser: cparser.cpp
	$(CC) $(CFLAGS) -c $@.cpp

ccompiler: ccompiler.cpp
	$(CC) $(CFLAGS) $(objects) $@.cpp -o $@

clean:
	rm -rf $(targets) $(ccom) *.o
