#include "WordListGenerator.h"

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
			/* but it works well and doesn't seem to cause any issues */
			
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
					while (iter->next)
						iter=iter->next;
					
					struct directoryList *newDirectory=malloc(sizeof(struct directoryList));
					newDirectory->dir=dirFinal;
					iter->next=newDirectory;
				}
		
			}
			/*RECURSION*/
			getAllTxt(dirFinal);
		}
	}

	closedir(file);
}

void getAllWords()
{
	
	struct directoryList *iter=dirHead;
	while (iter)
	{
		FILE *textFile = fopen(iter->dir,"r");
		int character;
		char *token;
		   while ((character = fgetc(textFile)) != EOF) 
		   {
			   
			   char poop=(char)character;
			   printf("%c",poop);
			   /*if (!isalnum(character))
			   {
				if (!token)
				{
					continue;
				}
				struct wordList *wordIter=wordHead;
				if (!wordIter)
				{
					wordIter=malloc(sizeof(struct wordList));
					wordIter->word=token;
					wordHead=wordIter;
				}
				else
				{
					while (wordIter->next)
						wordIter=wordIter->next;
					
					struct wordList *newWord=malloc(sizeof(struct wordList));
					newWord->word=token;
					wordIter->next=newWord;
				}  
				continue;
			   }
			   else
			   {
				
				char inToChar=character+'0';
				char toAppend[2]={inToChar, '\0'};
				char *temp=malloc(sizeof(token)+sizeof(toAppend)+1);
				strcpy(temp,token);
				strcat(temp,toAppend);
				free(token);
				token=temp;
			   }*/
		   }
		   

		  fclose(textFile);
		iter=iter->next;
	}
  
	
	
}


int main()
{	

	getAllTxt("C:/Users/cal13/Dropbox");
	
	debugPrintFiles();
	
	getAllWords();
	return 0;
}

