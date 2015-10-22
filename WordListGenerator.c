#include "WordListGenerator.h"
#include <unistd.h>

bool isText(char *name)
{
    size_t len = strlen(name);
    return len > 4 && strcmp(name + len - 4, ".txt") == 0;
}


/* prints all the locations of the text files */
/* For debugging */
void debugPrintFiles()
{
	struct directoryList *iter=dirHead;
	printf("LOCATIONS OF ALL TEXT FILES\n");
	printf("************************************\n");
	while (iter)
	{
		printf ("%s\n", iter->dir);
		iter=iter->next;
		
	}
}

void debugPrintWords()
{
	struct node *iter = root;
	while(iter)
	{
		printf("%s\n", iter->token);
		printf("%s\n", iter->file);
		iter = iter->next;
		
	}
}


/*populates a linked list with all the locations of a text file in a directory
this includes subfolders and whatnot */

void getAllTxt(char * directory)
{
	DIR *file;
	file = opendir (directory);

	/*If the directory is invalid simply return */
	if (!file) 
        return;


	while (true) 
	{
		struct dirent * entry;
		char * directoryName;
		entry = readdir (file);
	
	/*break the loop if it can't open the file */
		if (!entry)		
			break;
  
		directoryName = entry->d_name;
	
	

	
		char path[PATH_MAX];   
	
		if (strcmp (directoryName, "..") != 0 && strcmp (directoryName, ".") != 0) 
		{
			
			/* This block basically lumps a few strings together so that it can go into subdirectories. Probably not the best way to do things
			but it works well and doesn't seem to cause any issues */
			
			char *sl="/";
			char *dirPart1= malloc(strlen(sl)+strlen(directory)+1);
			strcpy(dirPart1,directory);
			strcat(dirPart1,sl);
			
			char *dirFinal = malloc(strlen(directoryName)+strlen(dirPart1)+1);
			
			strcpy(dirFinal, dirPart1);
			strcat(dirFinal, directoryName);
			
			/*if its a text file, throw it into the directory linked list */
			if (isText(directoryName)==true)
			{	
			
				struct directoryList *iter=dirHead;
				if (!iter)
				{
					iter=malloc(sizeof(struct directoryList));
					iter->dir=dirFinal;
					dirHead=iter;
				}
				else
				{
					#ifdef _WIN32
						while (iter->next)
							iter=iter->next;
						
						struct directoryList *newDirectory=malloc(sizeof(struct directoryList));
						newDirectory->dir=dirFinal;
						iter->next=newDirectory;
					#else
						while (iter->next!=NULL)
							iter=iter->next;
						
						struct directoryList *newDirectory=(struct directoryList *)malloc(sizeof(struct directoryList));
						newDirectory->dir=dirFinal;
						newDirectory->next=NULL;
						iter->next=newDirectory;
					#endif
				}
		
			}
			/*RECURSION*/
			getAllTxt(dirFinal);
		}
	}

	closedir(file);
}


void insert(char input[])
{
	
	struct node *toInsert=(struct node *)malloc(sizeof(struct node));
	struct node *iter;
	
	
	memcpy(toInsert->token,input, 200);
	
	iter=root;
	
	if (iter==NULL)
	{
		root=toInsert;
		root->next=iter;
		
		
	}

	else
	{
		/* traverse while it is not null*/
		while (iter!=NULL)
		{   
	
			
			if (iter->next == NULL ) 
			{
				toInsert->next=iter->next;
				iter->next=toInsert;
				return;
			}
			iter=iter->next;
		}
	}

	return;
}

void getAllWords()
{
	char linestring[BUFSIZ];
	struct directoryList *iter=dirHead;
	
	while (iter!=NULL)
	{
		FILE *file=fopen(iter->dir,"r");
		char string[200];
		
		while(fscanf(file, "%*[^A-Za-z]"), fscanf(file, "%199[a-zA-Z]", string) > 0)
		{
			
			insert(string);
			
		}
		
		
		
	fclose(file);
	iter=iter->next;
	}
  	
}


int main()
{	

	getAllTxt("C:/Users/cal13/dropbox");
	getAllWords();
	debugPrintFiles();
	debugPrintWords();
	return 0;
}
