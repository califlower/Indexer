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
	struct node *temp = root;
	while(temp!=NULL){
		printf("%s\n", temp->token);
		temp = temp->next;
	}
	
	//printf("LOCATIONS OF ALL TEXT FILES\n");
	//printf("************************************\n");
	//while (iter)
	//{
	//	printf ("%s\n", iter->dir);
	//	iter=iter->next;
	//}
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
			printf("%s\n", string);
		
        		struct node *newnode; // creates a new empty node
       			newnode  = (struct node*)malloc(sizeof(struct node));    // allocs memory to node         
       	 		newnode->token=string;
       	 		newnode->repetitions=0;
       	 		newnode->file=iter->dir;
       	 		newnode->next = NULL; // this creates the node and stores the input into the data variable
       	 		
       	 		int checker = 0;
        		struct node *temp;
        		temp = root;
        		
        		/*while (temp!=NULL){
        			if (temp->token == newnode->token && temp->file == newnode->file){
        			temp->repetitions++;
        			checker = 1;
        			}
        		temp = temp->next;
        		*/
        		
       	 		if (root == NULL){ // if the root node is empty, point the root node to this pointer.
        			root = newnode;
       	 		}	
       	 			
       	 		else{
 				newnode->next=root; // if the link list is not empty, set whatever head point was pointing to previously equal to newnode-> next
        			root=newnode;
       	 		}
        		
		}
	fclose(file);
	iter=iter->next;
	}
  	
}


int main()
{	

	getAllTxt("/Users/Saif/Desktop/testcases/comb");
	
	debugPrintFiles();
	
	getAllWords();
	return 0;
}
