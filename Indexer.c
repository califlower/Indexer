#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>

bool isText(char const *name)
{
    size_t len = strlen(name);
    return len > 4 && strcmp(name + len - 4, ".txt") == 0;
}
/* List the files in "dir_name". */
void getAllFiles (const char * directory)
{
    DIR *file;
    file = opendir (directory);

    
    if (!file) 
    {
        return;
    }
    while (true) 
    {
        struct dirent * entry;
        const char * directoryName;

        entry = readdir (file);
        if (!entry)		
            break;
  
        directoryName = entry->d_name;
	
	if (isText(directoryName)==true)
	printf ("%s/%s\n", directory, directoryName);
  
        char path[PATH_MAX];   
	if (strcmp (directoryName, "..") != 0 && strcmp (directoryName, ".") != 0) 
	{
		getAllFiles(path);
        }
	
    }

    closedir(file);
    
}

int main ()
{
    getAllFiles ("C:/Users/cal13/Downloads");
    return 0;
}
