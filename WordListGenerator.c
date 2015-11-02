#include "WordListGenerator.h"


/*************
 * Determines if a char file name is a text file given the full file name
 * This was created because linux has no way to determine file extensions
**************/
bool isText(char *name)
{
	size_t len = strlen(name);
	return len > 4 && strcmp(name + len - 4, ".txt") == 0;
}

/*************
 * This will create an output file based on the results of the previous run functions
 * GetALllText and GetAllWords should be run in that order, before this function.
 * Makes uses of global node variables that cannot really be accessed
 *************/
void createOutFile(char *name)
{
	struct node *iter = root;
	FILE *file=fopen(name,"w");
	fprintf(file,"{\"list\" : [");
	fprintf(file,"\n");
	while(iter)
	{
		struct occ *oIter = iter->occHead;
		fprintf(file,"	{");
		fprintf(file, "\"%s\"", iter->token);
		fprintf(file," : [\n");
		
		
		while (oIter)
		{
			fprintf(file,"		{");
			fprintf(file,"\"%s\"", oIter->file);
			fprintf(file,":");
			fprintf(file,"%i ", oIter->repetitions);
			fprintf(file,"}");
			oIter=oIter->next;
			if (oIter)
				fprintf(file,",\n");
			else
				fprintf(file,"\n");
		}
		
		iter = iter->next;
		if (iter)
			fprintf(file,"	]},\n");
		else
			fprintf(file,"	]}\n");;
		
	}
	fprintf(file,"]}");
	fclose(file);
}

/***************
 * For debugging purposes, print all the locations of all the text files in the specific directory
**************/
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

/**********************
 * For Debugging purposes print all words and their occurrences
 * Should be identical to the output file
********************/
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


/**************************
 * Recursively goes through an entire directory and
 * stores all the locations of every text file
************************/

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
					strcpy(iter->dirName,directoryName);
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



/*********
 * Given a head node, inserts an occurences into the occurrence list
 * Made into a seperate function to make the other functions neater
 * Code was rcycled from a previous project
**********/
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
		head->previous=NULL;
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
				if (iter->previous!=NULL)
				{
						char *TempFile=iter->previous->file;
						int TempRepetitions=iter->previous->repetitions;
						
						iter->previous->file=iter->file;
						iter->previous->repetitions=iter->repetitions;
						
						iter->file=TempFile;
						iter->repetitions=TempRepetitions;
				}
				
				return head;
			}				
			if (iter->next == NULL) 
			{
				toInsert->next=NULL;
				toInsert->previous=NULL;
				iter->next=toInsert;
				return head;
			}
			iter=iter->next;
		}
		return head;
	}

}

/****************
 * a seperate function for insertion. Makes it easier to play around with stuff seperatly from 
 * the part that actually gets all the words. This one helps insert stuff into the word list. 
 ***************/
void insert(char input[], char *file)
{
	
	struct node *toInsert=(struct node *)malloc(sizeof(struct node));
	struct node *iter;
	
/*Did it this way because a char array and a string are not in fact identical. 
A string is a char array, but a char array is not neccarily a string. Who knew.  */
	memcpy(toInsert->token,input, 200);
	
	iter=root;
	
	if (iter==NULL || strcmp(input,root->token)<0)
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
			else if (strcmp(input, iter->next->token)<0)
			{
				toInsert->next=iter->next;
				iter->next=toInsert;
				toInsert->occHead=NULL;
				toInsert->occHead=insertOcc(toInsert->occHead,file);
				return;
				
			}
			iter=iter->next;
		}
	}
	return;
}


/*********
 * Reads every word from every text file that was put into the list by the 
 * getAllText. Should only ever be run after getAllText
************/
void getAllWords()
{

	struct directoryList *iter=dirHead;
	
	while (iter!=NULL)
	{
		FILE *file=fopen(iter->dir,"r");
		char string[200];
		
/* Only scans alphanumeric stuff. Takes care of parsing in one nice swoop */
		while(fscanf(file, "%*[^A-Za-z0-9]"), fscanf(file, "%199[a-zA-Z0-9]", string) > 0)
		{
			insert(string,iter->dirName);
		}	
		fclose(file);
		
		iter=iter->next;
	}
}


/********
 * Main file was put in here because the program test cases are not run from the main file
 * It just accepts command line args
 * This main deals with some prompts like outputing overwriting and stuff
*******/
int main(int argc, char** argv)
{	
	FILE *file;
	if (argc!=3)
	{
		printf("You have inputted the wrong amount of arguments. You need to have 3 arguments (this includes the program name itself");
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
