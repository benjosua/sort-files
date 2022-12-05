#include <stdio.h> 
#include <stdlib.h> 
#include <dirent.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 

int main() 
{ 
	// current directory 
	char cwd[1024]; 
	getcwd(cwd, sizeof(cwd));
	
	// directory pointer 
	DIR *dir; 
	
	// directory entry structure 
	struct dirent *entry; 
  
	// stat structure 
	struct stat statbuf; 
  
	// open current directory 
	dir = opendir(cwd); 
	
	// check if directory is opened correctly 
	if (!dir) 
	{ 
		printf("ERROR: Unable to open directory.\n"); 
		exit(1); 
	} 
	
	// read entries of directory one by one 
	while ((entry = readdir(dir)) != NULL) 
	{ 
		// get the status of the file 
		if (lstat(entry->d_name, &statbuf) == 0) 
		{ 
			// check if it is a file 
			if (S_ISREG(statbuf.st_mode)) 
			{ 
				// get the file extension 
				char *extension = strrchr(entry->d_name, '.'); 
				
				// check if file has an extension 
				if (extension != NULL) 
				{ 
					// create a new directory with the same name as the file extension 
					char new_dir[1024]; 
					strcpy(new_dir, cwd); 
					strcat(new_dir, "/"); 
					strcat(new_dir, extension + 1); 
					mkdir(new_dir, 0700); 
					
					// move the file to the new directory 
					char source[1024], destination[1024]; 
					strcpy(source, cwd); 
					strcat(source, "/"); 
					strcat(source, entry->d_name); 
					
					strcpy(destination, new_dir); 
					strcat(destination, "/"); 
					strcat(destination, entry->d_name); 
					
					rename(source, destination); 
				} 
			} 
		} 
	} 
	
	// close the directory 
	closedir(dir); 
	
	// program ends successfully 
	return 0; 
}