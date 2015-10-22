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



struct occ
{
	char *file;
	int repetitions;
	struct occ *next;
};

/* a list of all the words */
struct node
{
	char token[200];
	struct occ *occHead;
	struct node *next;
}*root;


struct directoryList
{
	char *dir;
	char *dirName;
	
	struct directoryList *next;
}*dirHead;

/*checks if file is a text */
bool isText(char *name);

/*populates a linked list with locations of all text files in a directory*/
void getAllTxt(char *directory);

void insert(char input[], char *file);

/*gets all the words in either a file or directory, and creates a linked list of them*/
void getAllWords();


void debugPrintWords();

void debugPrintFiles();
