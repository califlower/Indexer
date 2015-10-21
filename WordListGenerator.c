#include "WordListGenerator.h"

bool isText(char *name)
{
    size_t len = strlen(name);
    return len > 4 && strcmp(name + len - 4, ".txt") == 0;
}

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
	
        if (!entry)		
            break;
  
        directoryName = entry->d_name;
	
	

	
        char path[PATH_MAX];   
	
	if (strcmp (directoryName, "..") != 0 && strcmp (directoryName, ".") != 0) 
	{
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
		
		getAllTxt(dirFinal);
        }
	
    }

    closedir(file);
    
    
}

int main()
{	

	getAllTxt("C:/Users/cal13/Dropbox");
	debugPrintFiles();
	return 0;
}

