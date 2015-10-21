#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>

/*
A list of all the files that a word occurs in
*/

struct occList
{
	char *file;
	struct occList *next;
};

/* a list of all the words */
struct wordList
{
	struct occList *head;
	char *word;
	struct wordList *next;
}*wordHead;

struct directoryList
{
	char *dir;
	struct directoryList *next;
}*dirHead;

/*checks if file is a text */
bool isText(char *name);

/*populates a linked list with locations of all text files in a directory*/
void getAllTxt(char *directory);

/*gets all the words in either a file or directory, and creates a linked list of them*/
void getAllWords();


void debugPrintWords();

void debugPrintFiles();





