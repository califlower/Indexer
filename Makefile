COMPILER = gcc
CCFLAGS  = -Wall -g -O3
all: indexer

WordListGenerator.o: WordListGenerator.c
	$(COMPILER) $(CCFLAGS) -c WordListGenerator.c
indexer: WordListGenerator.o
	$(COMPILER) $(CCFLAGS) -o indexer WordListGenerator.o
clean:
	rm -f indexer
	rm -rf *.o