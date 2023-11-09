#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>


FILE *
fopen(const char *path, const char *mode) 
{   
    int log = strcmp(path, "file_logging.log"); //if 0 open log file

    FILE *original_fopen_ret;
    FILE *(*original_fopen)(const char*, const char*);

    /* call the original fopen function */
    original_fopen = dlsym(RTLD_NEXT, "fopen");
    original_fopen_ret = (*original_fopen)(path, mode);

    if(log != 0){
        int uid = getuid(); /* user id (positive integer) */
        int access_type; /* access type values [0-2] */
        int action_denied; /* is action denied values [0-1] */

        time_t times; /* file access time     format: Sun Sep 16 01:03:52 1973\n\0 */
        time(&times);
        struct tm *time_info;
        time_info = localtime(&times);
        // Get the formatted date and time string
        char* date_time_str = asctime(time_info);

        char timestamp[8+1];  // '\0'
        strncpy(timestamp, date_time_str + 11, 8);
        timestamp[8] = '\0';
            
        char date2[4+1];
        char date1[11+1 + sizeof(date2)];
        strncpy(date1, date_time_str, 10);
        date1[10] = ' ';
        date1[11] = '\0';
        strncpy(date2, date_time_str + 20, 4);
        date2[4] = '\0';
        strcat(date1, date2);

        char *filename = realpath(path, NULL); /* filename (string) */

        MD5_CTX md5Context;
        MD5_Init(&md5Context);

        char buffer[4096];
        size_t bytesRead;

        fseek(original_fopen_ret, 0, SEEK_SET);

        while ((bytesRead = fread(buffer, 1, sizeof(buffer), original_fopen_ret)) > 0) {
            MD5_Update(&md5Context, buffer, bytesRead);
        }
        unsigned char fingerprint[MD5_DIGEST_LENGTH];
        MD5_Final(fingerprint, &md5Context);

        ////////////////////////////////////////////////////////

        if (access(path, F_OK) != -1) {
            // The file exists
            access_type = 1;
            action_denied = 0;
        }
        else {
            // The file does not exist, create file
            if(mode[0] == 'w' || mode[0] == 'a'){
                action_denied = 0;
                access_type = 0;
            }
            else{
                access_type = 1;    //the file can't be accessed nor be created so the opening action is denied
                action_denied = 1;
            }
        }
        size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);
        original_fwrite = dlsym(RTLD_NEXT, "fwrite");
        FILE *logg;
        logg = (*original_fopen)("file_logging.log", "w");
        //(*original_fwrite)(&uid, sizeof(uid), 1, logg);
        //(*original_fwrite)('\n', sizeof(char), 1, logg);
        //(*original_fwrite)(date1, sizeof(char), strlen(date1), logg);
        fprintf(logg, "%d %s %s %s %d %d %s", uid, filename, date1, timestamp, access_type, action_denied, fingerprint);
        
    }

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
