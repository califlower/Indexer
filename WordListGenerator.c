#include "WordListGenerator.h"
#include <unistd.h>

bool isText(char *name)
{
    size_t len = strlen(name);
    return len > 4 && strcmp(name + len - 4, ".txt") == 0;
}

int isFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
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
		printf("%s\n ", iter->token);
		printf("{");
		struct occ *oIter = iter->occHead;
		while (oIter)
		{
			printf("%i ", oIter->repetitions);
			printf("%s", oIter->file);
			printf(",");
			oIter=oIter->next;
		}
		printf("}");
		printf("\n");
		iter = iter->next;
		
	}
}


/*populates a linked list with all the locations of a text file in a directory
this includes subfolders and whatnot */

void getAllTxt(char * directory)
{
	
	
	if (strstr(directory, "/") == NULL && isText(directory))
	{
		
		struct directoryList *iter=malloc(sizeof(struct directoryList));
		iter->dir=directory;
		iter->dirName=directory;
		iter->next=NULL;	
		dirHead=iter;
		return;
	}
	
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
					iter->dirName=malloc(PATH_MAX);
					strncpy(iter->dirName,directoryName, sizeof(PATH_MAX));
					iter->next=NULL;
					dirHead=iter;
				}
				else
				{
					#ifdef _WIN32
						while (iter->next!=NULL)
							iter=iter->next;
						
						struct directoryList *newDirectory=malloc(sizeof(struct directoryList));
						newDirectory->dir=dirFinal;
						
						newDirectory->dirName=malloc(PATH_MAX);
						strncpy(newDirectory->dirName,directoryName, PATH_MAX);
						newDirectory->next=NULL;
						iter->next=newDirectory;
						
					#else
						while (iter->next!=NULL)
							iter=iter->next;
						
						struct directoryList *newDirectory=(struct directoryList *)malloc(sizeof(struct directoryList));
						newDirectory->dir=dirFinal;
						newDirectory->dirName=malloc(PATH_MAX);
						strncpy(newDirectory->dirName,directoryName, PATH_MAX);
					
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



struct occ *insertOcc(struct occ *head, char *file)
{
	/*creates the node to add and the iterator*/
	struct occ *toInsert=(struct occ *)malloc(sizeof(struct occ));
	struct occ *iter;
	toInsert->file=file;
	
	toInsert->repetitions=1;
	
	iter=head;
	
	/* if linked list is empty or the first element is larger than the input*/
	
	if (iter==NULL)
	{
	
		head=toInsert;
		head->next=iter;
		return head;
	}
	
	else if (strcmp(iter->file,file)==0)
	{

		iter->repetitions++;
		iter->file=file;
		return head;
	}
	else
	{
		
		while (iter!=NULL)
		{   
	
			if (strcmp(iter->file,file)==0)
			{
				iter->repetitions++;
				return head;
			}				
			if (iter->next == NULL) 
			{
				toInsert->next=NULL;
				iter->next=toInsert;
				return head;
			}
			iter=iter->next;
		}
		return head;
	}

}

/* a seperate function for insertion. Makes it easier to play around with stuff seperatly from
the part that actually gets all the words */
void insert(char input[], char *file)
{
	
	struct node *toInsert=(struct node *)malloc(sizeof(struct node));
	struct node *iter;
	
	memcpy(toInsert->token,input, 200);
	
	
	
	iter=root;
	
	if (iter==NULL)
	{
		toInsert->occHead=NULL;
		toInsert->next=NULL;
		toInsert->occHead=insertOcc(toInsert->occHead,file);
		root=toInsert;
		root->next=iter;
		
		return;	
	}
	else if (strcmp(iter->token,input)==0)
	{
		insertOcc(iter->occHead, file);
	}
	else
	{
		/* traverse while it is not null*/
		while (iter!=NULL)
		{   		
			if (strcmp(iter->token,input)==0)
 			{ 
				iter->occHead=insertOcc(iter->occHead, file);
				return;				
 			}	 

			if (iter->next == NULL ) 
			{
				toInsert->next=NULL;
				toInsert->occHead=NULL;
				toInsert->occHead=insertOcc(toInsert->occHead,file);
				iter->next=toInsert;
				return;
			}
			iter=iter->next;
		}
	}
	return;
}


/*Reads all the words from all the files*/
void getAllWords()
{

	struct directoryList *iter=dirHead;
	
	while (iter!=NULL)
	{
		FILE *file=fopen(iter->dir,"r");
		char string[200];
		
		while(fscanf(file, "%*[^A-Za-z0-9]"), fscanf(file, "%199[a-zA-Z0-9]", string) > 0)
		{
			insert(string,iter->dirName);
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
