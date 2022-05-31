#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int split(char* string, char* seperator, char* argv[]);
void print_select_mode(void);
void print_branch_information(void);
void admin_mode(void);
void user_mode(void);
void add_branch(void);
void edit_branch(void);
void remove_branch(void);

int main(){
	int input;
	while(1){
		fflush(stdin);
		system("clear");
		print_select_mode();
		scanf("%d", &input);
		
		if(input == 1){
			admin_mode();
		}
		else if(input == 2){
			user_mode();
		}
		else if(input == 3){
			printf("Exit program..\n");
			sleep(1);
			exit(0);
		}
		else{
			printf("Select 1~3\n");
			sleep(1);
			continue;
		}
	}
	exit(0);
}


void print_select_mode(void){
	printf("Select Mode\n");
	printf("1. Admin mode\n");
	printf("2. User mode\n");
	printf("3. Exit\n\n");
	return;
}

void print_branch_information(void){
	char dirname[5];
	printf("Branch list\n");
	for(int i=1; i<7; i++){
		memset(dirname, 0, 2);
		sprintf(dirname, "./%d", i);
		if(access(dirname, F_OK) == 0)
			printf("%d (opened)\n", i);
		else
			printf("%d (closed)\n", i);
	}
}


void admin_mode(void){
	int input;
	while(1){
		system("clear");
		print_branch_information();
		printf("\n--------------\n");
		printf("1) Add branch\n");
		printf("2) Edit branch\n");
		printf("3) Remove branch\n");
		printf("4) Back to menu\n");
		printf(" >> ");
		scanf("%d", &input);
		fflush(stdin);

		if(input == 1){
			add_branch();
			break;
		}
		else if(input == 2){
			edit_branch();
			break;
		}
		else if(input == 3){
			remove_branch();
			break;
		}
		else if(input == 4){
			printf("Back to menu..\n");
			sleep(1);
			return;
		}
		else{
			printf("Wrong input. Select(1~4)\n");
			sleep(1);
			continue;
		}
	
	}
	return;
}

void add_branch(void){
	printf("add branch\n");
	sleep(1);
	return;
}

void edit_branch(void){
	printf("edit branch\n");
	sleep(1);
	return;
}

void remove_branch(void){
	printf("remove branch\n");
	sleep(1);
	return;
}


void user_mode(void){
	return;
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
