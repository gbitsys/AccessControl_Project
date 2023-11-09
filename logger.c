#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>

/*typedef struct log_entry {

    int uid; /* user id (positive integer) 
    int access_type; /* access type values [0-2] 
    int action_denied; /* is action denied values [0-1] 

    time_t date; /* file access date 
    time_t time; /* file access time 

    char *file; /* filename (string)
    char *fingerprint; /* file fingerprint

    char* filename;

}Entry;
*/

FILE *
fopen(const char *path, const char *mode) 
{   
    int uid = getuid(); /* user id (positive integer) */
    int access_type; /* access type values [0-2] */
    int action_denied; /* is action denied values [0-1] */

    time_t times; /* file access time     format: Sun Sep 16 01:03:52 1973\n\0 */
    time(&times);
    struct tm *time_info;
    time_info = localtime(&times);
    // Get the formatted date and time string
    char* date_time_str = asctime(time_info);

    char timestamp[8+1+1];  // +'\n' +'\0'
    strncpy(timestamp, date_time_str + 11, 8);
    timestamp[8] = '\n';
    timestamp[9] = '\0';
        
    char date2[4+1+1];
    char date1[11+1 + sizeof(date2)];
    strncpy(date1, date_time_str, 10);
    date1[10] = ' ';
    date1[11] = '\0';
    strncpy(date2, date_time_str + 20, 4);
    date2[4] = '\n';
    date2[5] = '\0';
    strcat(date1, date2);

    char *filename  ; /* filename (string) */
    char *fingerprint; /* file fingerprint */

    ////////////////////////////////////////////////////////

    if (access(path, F_OK) != -1) {
        // The file exists
        printf("File already exists: %s\n", path);
    } else {
        // The file does not exist
        printf("File does not exist: %s\n", path);
    }

    FILE *original_fopen_ret;
    FILE *(*original_fopen)(const char*, const char*);

    /* call the original fopen function */
    original_fopen = dlsym(RTLD_NEXT, "fopen");
    original_fopen_ret = (*original_fopen)(path, mode);


    /* add your code here */
    /* ... */
    /* ... */
    /* ... */
    /* ... */


    return original_fopen_ret;
}


size_t 
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{

    size_t original_fwrite_ret;
    size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

    /* call the original fwrite function */
    original_fwrite = dlsym(RTLD_NEXT, "fwrite");
    original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);


    /* add your code here */
    /* ... */
    /* ... */
    /* ... */
    /* ... */


    return original_fwrite_ret;
}
