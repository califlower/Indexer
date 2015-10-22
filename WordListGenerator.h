#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <ctype.h>
/*
A list of all the files that a word occurs in
*/

/* a list of all the words */
struct Token
{
	char *token;
	int repetitions;
	FILE *file;
	struct Token *next;
}head;

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





