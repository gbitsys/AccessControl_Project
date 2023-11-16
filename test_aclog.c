#include"test_aclog.h"
int main() 
{
	int i;
	size_t bytes;
	FILE *file;
	char filenames[6][7] = {"file_0", "file_1", 
			"file_2", "file_3", "file_4",
			"file_5"};


	/* example source code */

/*	for (i = 0; i < 6; i++) {

		file = fopen(filenames[i], "w+");
		if (file == NULL) 
			printf("fopen error\n");
		else {
			bytes = fwrite(filenames[i], strlen(filenames[i]), 1, file);
      fprintf(file,"\n");
			fclose(file);
		}
  }
    
    FILE * file2;
    file2 = fopen("file_test", "w");
    if (file2==NULL){
      fprintf(stderr, "file fopen error\n");
    }
    else {
      fwrite(filenames[3], strlen(filenames[3]), 1, file2);
      fprintf(file2, "\n");
      fclose(file2);
    }
    
	  chmod("file_test", 0440);
    
    file2 = fopen("file_test", "r");

   if (file2==NULL){
    fprintf(stderr, "fopen error\n");
   } else {
    fclose(file2);
  }*/ 
    
    FILE* file2; 
    file2 = fopen("file_test", "w");
    fclose(file2);

    chmod("file_test", 0220);
    file2 = fopen("file_test", "w"); 
    if (file2==NULL){
      printf("fopen error\n");
    } else {
      fclose(file2);
    } 
  
    return 0;
}
