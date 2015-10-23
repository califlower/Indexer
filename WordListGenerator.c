#include "WordListGenerator.h"


/*Determines if a file is a text file or not */
bool isText(char *name)
{
    size_t len = strlen(name);
    return len > 4 && strcmp(name + len - 4, ".txt") == 0;
}

/* Create the output file given a character name */
void createOutFile(char *name)
{
	struct node *iter = root;
	FILE *file;
	file=fopen(name,"w");
	
	while(iter)
	{
		struct occ *oIter = iter->occHead;
		fprintf(file, "%s\n ", iter->token);
		fprintf(file,"{");
		
		while (oIter)
		{
			fprintf(file,"%i ", oIter->repetitions);
			fprintf(file,"%s", oIter->file);
			fprintf(file,",");
			oIter=oIter->next;
		}
		fprintf(file,"}");
		fprintf(file,"\n");
		iter = iter->next;
		
	}
	fclose(file);
}

/*Debug print all file locations of text files */
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

/* Print all words and their occurences for debugging purposes */
void debugPrintWords()
{
	struct node *iter = root;
	while(iter)
	{
		struct occ *oIter = iter->occHead;
		printf("%s\n ", iter->token);
		printf("{");
		
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
	DIR *file;
	
	if (strstr(directory, "/") == NULL && isText(directory))
	{
		
		struct directoryList *iter=malloc(sizeof(struct directoryList));
		iter->dir=directory;
		iter->dirName=directory;
		iter->next=NULL;	
		dirHead=iter;
		return;
	}
	
	
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
			char *dirFinal = malloc(strlen(directoryName)+strlen(dirPart1)+1);
			strcpy(dirPart1,directory);
			strcat(dirPart1,sl);
			
			strcpy(dirFinal, dirPart1);
			strcat(dirFinal, directoryName);
			
			/*if its a text file, throw it into the directory linked list */
			if (isText(directoryName)==true)
			{	
			
				struct directoryList *iter=dirHead;
				
				/* Some OS specific stuff that allows me to change things up depending on the OS */
				if (!iter)
				{
					
					iter=malloc(sizeof(struct directoryList));
					iter->dir=dirFinal;
					
					#ifdef _WIN32
						iter->dirName=malloc(PATH_MAX);
						strncpy(iter->dirName,directoryName, sizeof(PATH_MAX));
					#else
						iter->dirName=directoryName;
					#endif
					iter->next=NULL;
					dirHead=iter;
				}
				else
				{
					#ifdef _WIN32
						struct directoryList *newDirectory=malloc(sizeof(struct directoryList));
						while (iter->next!=NULL)
							iter=iter->next;
						
						
						newDirectory->dir=dirFinal;
						newDirectory->dirName=malloc(PATH_MAX);
						strncpy(newDirectory->dirName,directoryName, PATH_MAX);
						newDirectory->next=NULL;
						iter->next=newDirectory;
						
					#else
						struct directoryList *newDirectory=(struct directoryList *)malloc(sizeof(struct directoryList));
						while (iter->next!=NULL)
							iter=iter->next;
						
						
						newDirectory->dir=dirFinal;
						newDirectory->dirName=directoryName;
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
	
	/* if linked list is empty */
	
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
	
	/*Did it this way because a char array and a string are not in fact identical */
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

int main(int argc, char** argv)
{	
	FILE *file;
	if (argc!=3)
	{
		printf("You need the correct amount of arguments");
		return 1;
	}

	getAllTxt(argv[2]);
	getAllWords();
	/* Comment out debug commands
	debugPrintFiles();
	debugPrintWords();
	*/
	
	
	file = fopen (argv[1],"r");
	if (!file) 
		createOutFile(argv[1]);
	else
	{
		char str1[20];
		printf("Would you like to overrite an existing output file? Y/N ");
		scanf("%s", (char *)&str1);
		if (strcmp(str1,"Y")==0 || strcmp(str1,"y")==0)
		{
			createOutFile(argv[1]);
		}
		else
			return 0;
		

	}
	
	return 0;
}
