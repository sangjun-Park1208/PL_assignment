#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int split(char* string, char* seperator, char* argv[]);

int main(){
	int argc;
	char* argv[3];
	char input[20];
	memset(input, 0, 20);
	printf(">> ");
	fgets(input, sizeof(input), stdin);
	input[strlen(input)-1] = '\0';
	argc = split(input, " ", argv);
	for(int i=0; i<argc; i++)
		printf("argv[%d] : %s \n", i, argv[i]);
	exit(0);

}



int split(char* string, char* seperator, char* argv[]){
	int argc = 0;
	char* ptr = NULL;
	ptr = strtok(string, seperator);
	while(ptr!=NULL){
		argv[argc++] = ptr;
		ptr = strtok(NULL, " ");
	}
	return argc;
}
