#include"logger.h"

FILE *
fopen(const char *path, const char *mode) 
{   
    int log = strcmp(path, "file_logging.log"); //if 0 open log file
    FILE *original_fopen_ret;
    FILE *(*original_fopen)(const char*, const char*); //function pointer to original fopen

    /* call the original fopen function */
    original_fopen = dlsym(RTLD_NEXT, "fopen");

    int uid = getuid(); /* user id (positive integer) */
    int access_type; /* access type values [0-2] */
    int action_denied=0; /* is action denied values [0-1] */
    static int counter=0;

    /* determine access type*/
    //first checking if file exists
    if (euidaccess(path, F_OK)!=-1){ 
      if (mode[0]=='r'){
        if (euidaccess(path, R_OK)!=-1){
          //open file with access
          access_type = 1;
          action_denied = 0;
        } else {
          //open file with no access
          access_type = 1;
          action_denied = 1;
        }
      }
        if(mode[0]=='w' || mode[0]=='a'){
          if (euidaccess(path, W_OK)!=-1){
            //write file with access
            access_type = 2;
            action_denied = 0;
          } else {
            //write file with no access
            access_type = 2;
            action_denied = 1;
          } 
        }
    }
    //if file doesn't exist and we want to write in it  
    else {
      if (mode[0]=='w' || mode[0]=='a'){
        action_denied = 0;
        access_type = 0; //file created
      }
    }

    if (action_denied==0) original_fopen_ret = (*original_fopen)(path,mode);
    else {
      original_fopen_ret = (*original_fopen)(path,"r");
    }
  
    char *filename = realpath(path, NULL); /* filename (string) */
    if(log != 0){
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

        
        MD5_CTX md5Context;
        MD5_Init(&md5Context);

        char buffer[4096];
        size_t bytesRead;
        unsigned char fingerprint[MD5_DIGEST_LENGTH]; //result of the md5 hashing

        if (original_fopen_ret!=NULL){
          fseek(original_fopen_ret, 0, SEEK_SET);
          while ((bytesRead = fread(buffer, 1, sizeof(buffer), original_fopen_ret)) > 0) {
          MD5_Update(&md5Context, buffer, bytesRead);
          }
          
          MD5_Final(fingerprint, &md5Context);
        }
        
        

        FILE *logg;
        logg = (*original_fopen)("file_logging.log", "a");
        fprintf(logg, "%d %s %s %s %d %d  open ", uid, filename, date1, timestamp, access_type, action_denied);
        if (original_fopen_ret!=NULL){
          for (int i=0; i<MD5_DIGEST_LENGTH; i++){
            fprintf(logg, "%02x", fingerprint[i]);
          }      
        }
        fprintf(logg, "\n");
        fclose(logg);
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
    
    // Get user ID
    int uid = getuid();
    int access_type = 2; /* access type values [0-2] */
    int action_denied = 1; /* is action denied values [0-1] */

    // Get the file descriptor
    int fileDescriptor = fileno(stream);

    // Get the file path using the file descriptor
    char path[4096];
    char procPath[4096];
    snprintf(procPath, sizeof(procPath), "/proc/self/fd/%d", fileDescriptor);
    ssize_t pathSize = readlink(procPath, path, sizeof(path));
    if (pathSize != -1) {
      if (euidaccess(path, W_OK)!=-1){
        //write file with access
        access_type = 2;
        action_denied = 0;
      } else {
        //write file with no access
        access_type = 2;
        action_denied = 1;
      } 
    
        FILE * logg = fopen("file_logging.log", "a");

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


        MD5_CTX md5Context;
        MD5_Init(&md5Context);

        char buffer[4096];
        size_t bytesRead;

        fseek(stream, 0, SEEK_SET);

        while ((bytesRead = fread(buffer, 1, sizeof(buffer), stream)) > 0) {
            MD5_Update(&md5Context, buffer, bytesRead);
        }
        unsigned char fingerprint[MD5_DIGEST_LENGTH];
        MD5_Final(fingerprint, &md5Context);

        fprintf(logg, "%d %s %s %s %d %d ", uid, path, date1, timestamp, access_type, action_denied);
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            fprintf(logg, "%02x", fingerprint[i]);
        }

        fprintf(logg, "\n");
        fclose(logg);

    }

    original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream); //works for w and w+
    if (action_denied==0) return original_fwrite_ret;
    return 0;
}
