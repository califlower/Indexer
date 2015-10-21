/*
A list of all the files that a word occurs in
*/

struct occList
{
	char *file
	occList *next;
};

/* a list of all the words */
struct wordList
{
	struct occList *head;
	char *word;
}wordHead;

/*gets all the words in either a file or directory, and creates a linked list of them*/
wordList *getAllWords(DIRECTORY);