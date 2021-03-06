#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEN(arr) (sizeof(arr)/sizeof((arr)[0]))

int main(int argc, char* argv[]){
  int n,m;
  double sum = 0;
  char buff[1024];

  if(argc!=2)
    return EXIT_FAILURE;
  FILE* fd = fopen(argv[1],"r");
  if(fd==NULL)
    return EXIT_FAILURE;

  fgets(buff,LEN(buff),fd);
  sscanf(buff,"%d %d",&n, &m);
  printf("Reading %d lines by %d chars \n\n", n, m);
  for(int  i =0; i< n; i++){
    double d;
    unsigned int read_nums = 0;
    fgets(buff,LEN(buff),fd);
    printf("%s",buff);
    char* s;
    s=strtok(buff," ");
    while(s!=NULL){
      if(read_nums >= m)
	break;
      int status = sscanf(s,"%lf",&d);
      if(status!=1)
	return EXIT_FAILURE;
      read_nums++;
      sum+=d;
      s=strtok(NULL," ");
    }
  }
  printf("\nsum is: %lf\n",sum);
  fclose(fd);
  return EXIT_SUCCESS;
}
