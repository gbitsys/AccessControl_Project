#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h> // Include unistd.h for access

FILE* fopen(const char* path, const char* mode){
	if (access(path, F_OK) != -1) {
        // The file exists
        printf("File already exists: %s\n", path);
    } else {
        // The file does not exist
        printf("File does not exist, new file created: %s\n", path);
    }
	FILE* (original_fopen)(const char*, const char*);
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	return (*original_fopen)(path, mode);
}
