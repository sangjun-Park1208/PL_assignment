#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define ARGMAX 3
#define RESV_FILE "./reservation.txt"
#define BUF_SIZE 1024
#define RESV_NUM 100
#define PATH_MAX 1024

/***** prompt *****/
void admin_prompt(void);
void user_prompt(void);

/***** branch edit *****/ 		   	// make directory
void add_br(int brnum); 		   	// add branch
void del_br(int brnum); 		   	// delete branch

/***** room edit *****/ 	 		   		 // record format : 	#date  #time  #ID\n
void ins_rm(int rmnum); 					 // insert room
void apd_rm(int rmnum); 				     // append room
void del_rm(int rmnum); 					 // delete room
void sort_rm(int rmnum);	
char* get_roomNum(int brnum, int rmnum);	 // make filename : "branchNum-roomNum"

/***** edit file *****/
void edt_admin(int offset); 	   	// admin file format :	#br-rm  #_maxnum\n
void edt_resv(int offset); 		   	// resv file format : 	#ID  #brn  #rm  #num  #date  #time  #duration\n

/***** print info *****/
void pr_ad_select_mode(void);			// admin_selece Mode
void pr_ad_brabch_info(void); 			// admin_branch info
void pr_ad_usr_mode(void); 				// admin_input ID
void pr_ad_room_info(int brnum); 		// admin_room information
void pr_us_id_input(void); 				// user_input id prompt
void pr_us_br_info(void); 				// user_branch info
void pr_us_rm_info(int brnum);			// user_room info
void pr_us_resv_info1(int rmnum);		// user_reservation info1(new reservation)
void pr_us_resv_info2(RESV* resv_v[]);	// user_reservation info2(edit reservation)

/***** other functions *****/
void init_input(void);									// initialize input (global)value
int split(char* string, char* seperator, char* argv[]); // parsing function
int get_input(void);									// get input number

/***** reservation info struct *****/
typedef struct _RESV{
	char ID[11];
	int branch;
	int room;
	char date[6];
	int time;
	int resv_time;
	int resv_num;
}RESV;

/***** global variable *****/
RESV resv;
RESV resv_v[RESV_NUM];
char input[20];
char buf[BUF_SIZE];


int main(){
	int fd;
	int argc;
	char* argv[ARGMAX];

	while(1){
		system("clear");
		memset(input, '\0', 20);
		for(int i=0; i<ARGMAX; i++)
			memset(argv[i], '\0', ARGMAX);
		
		pr_ad_select_mode();
		printf("input >> ");
		fgets(input, 20, stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);

		if(!strcmp(argv[0], "exit"))
			exit(0);
		
		if(argc == 0 || argc > 1)
			continue;

		if(atoi(argv[0]) < 0 || atoi(argv[0]) > 2){
			printf("wrong input.. select 1 or 2\n");
			continue;
		}

		switch(atoi(input_num)){
			case 1:
				admin_prompt();
				break;
			case 2:
				user_prompt();
				break;
			default:
				break;
		}
	}
	exit(0);
}

void admin_prompt(void){
	int inputNum;

	pr_ad_branch_info();

	printf("1) Add branch\n");
	printf("2) Edit branch\n");
	printf("3) Delete branch\n");
	printf("4) Exit\n");
	printf("5) Return to First step\n\n");
	printf("input >> ");

	inputNum = get_input();

	switch(inputNum){
		case 1:
			system("clear");
			pr_ad_branch_info();
			add_branch();
			break;
			
		case 2:
			sytem("clear");
			pr_ad_branch_info();
			edit_branch();
			break;

		case 3:
			system("clear");
			pr_ad_branch_info();
			delete_branch();

		case 4:
			printf("program exit..\n");
			sleep(1);
			exit(0);
		
		case 5:
			init_resv(&resv);
			return;

		default:
			printf("Wrong input. Back to First step..\n");
			sleep(1);
			init_resv(&resv);
			system("clear");
			return;
	}

	return;
}

void user_prompt(RESV* resvp, RESV* resv_v[]){
	system("clear");
	printf("---------------------\n");
	printf("|	Input your ID	|\n")
	printf("---------------------\n\n");

	printf("ID >> ");

	int argc;
	char* argv[ARGMAX];
	fgets(input, 20, stdin);
	input[strlen(input)-1] = '\0';
	argc = split(input, " ", argv);


	strcpy(resvp->ID, argv[0]);

	int fd;
	int length;
	
	if(access(RESV_FILE, F_OK) < 0){
		if((fd = open(RESV_FILE, O_WRONLY | O_CREAT)) < 0)
			fprintf(stdout, "open \"%s\" error\n", RESV_FILE);
		close(fd);
	}

	if((fd = open("./reservation.txt", O_RDONLY | O_CREAT)) < 0){
		fprintf(stderr, "open \"%s\" error\n", RESV_FILE);
		exit(1);
	}
	
	memset(buf, '\0', BUF_SIZE);
	int i=1;
	int isFirst = 1;
	while(length = read(fd, buf, sizeof(RESV)) > 0){
		buf[strlen(buf)-1] = '\0';
		argc = split(buf, " ", argv);

		if(!strcmp(argv[0], resvp->ID)){
			strcpy(resv_v[i]->ID, argv[0]);
			resv_v[i]->branch = atoi(argv[1]);
			resv_v[i]->room = atoi(argv[2]);
			strcpy(resv_v[i]->date, argv[3]);
			resv_v[i]->time = atoi(argv[4]);
			resv_v[i]->resv_time = atoi(argv[5]);
			resv_v[i]->resv_num = atoi(argv[6]);
			ifFirst = 0;
			i++;
		}
	}
	
	close(fd);


	char pathbuf[PATH_MAX];
	memset(pathbuf, '\0', PATH_MAX);
	char tmpbuf[PATH_MAX/2];
	memset(tmpbuf, '\0', PATH_MAX/2);

	if(isFirst){
		system("clear");
		pr_us_br_info();
		printf("input branch number >> ");
		fgets(input, 20, stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);

		if(atoi(argv[0]) == 0){
			printf("return to First step..\n");
			sleep(1);
			return;
		}
		strcpy(tmpbuf, argv[0]);
		resvp->branch = atoi(argv[0]);					// input #reservation_branch
		if(access(argv[0], F_OK) < 0){
			printf("%d branch doesn't exist.\n", resvp->branch);
			printf("return to First step..\n");
			sleep(1);
			return;
		}

		system("clear");
		pr_us_rm_info(atoi(argv[0]));
		printf("input room number >> ");
		fgets(input, 20, stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);
		if(atoi(argv[0]) == 0){
			printf("return to First step..\n");
			sleep(1);
			return;
		}
		sprintf(pathbuf, "%s/%s", tmpbuf, argv[0]);
		resvp->room = atoi(argv[0]);					// input #reservation_room
		if(access(argv[0], F_OK) < 0){
			printf("%d room in %d branch doesn't exist\n", resvp->room, resvp->branch);
			printf("return to First step..\n");
			sleep(1);
			return;
		}




		time_t t = time(NULL);
		struct tm lt;
		localtime_r(&t, &lt);
		char date[7];
		memset(date, '\0', 7);
		sprintf(date, "%02d%02d%02d", lt.tm_year+1900-2000, lt.tm_mon+1, lt.tm_mday);

		system("clear");
		pr_us_resv_info1(resvp->room);

		printf("input reservation date >> ");
		fgets(input, 20, stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);

		strcpy(resvp->date, atoi(argv[0]));				// input date
		if(strcmp(date, resvp->date) >= 0){ 		// wrong input(past or present date input)
			printf("input reservation date is invalid.\n");
			printf("return to First step..\n");
			sleep(1);
			return;
		}


		printf("input reservation start time >> ");
		fgets(input, 20, stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);

		resvp->time = atoi(argv[0]);					// input time
		if(resvp->time < 8 || resvp->time > 22){
			printf("valid reservation time : 08:00 ~ 22:00\n");
			printf("return to First step.. \n");
			sleep(1);
			return;
		}


		printf("input reservation period >> ");
		fgets(input, 20, stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);
		
		resvp->resv_time = atoi(argv[0]);				// input resv_time
		if(resvp->time + resvp->resv_time >= 22){
			printf("valid reservation time : 08:00 ~ 22:00\n");
			printf("return to First step..\n");
			return;
		}


		printf("input person number >> ");
		fgets(input, 20, stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);
		
		resvp->resv_num = atoi(argv[0]);				// input resv_num


		if((fd = open(RESV_FILE, O_RDWR)) < 0){
			fprintf(stderr, "open error in \"%s\"\n", RESV_FILE);
			exit(1);
		}
		
		int append = 0;
		RESV tmpresv;
		strcpy(tmpresv.ID, resvp->ID);
		tmpresv.branch = resvp->branch;
		tmpresv.room = resvp->room;
		strcpy(tmpresv.date, resvp->date);
		tmpresv.time = resvp->time;
		tmpresv.resv_time = resvp->resv_time;

		while(length = read(fd, buf, sizeof(RESV)) > 0){
			buf[strlen(buf)-1] = '\0';
			argc = split(buf, " ", argv);

			if(strcmp(argv[3], resvp->date) == 0){ 
				if(resvp->time < argv[4] && resvp->time + resvp->resv_time <= argv[4]){
					write(fd, &tmpresv, sizeof(RESV));
					break;
				}
			}
			else if(strcmp(argv[3], resvp->date) < 0){ // next date
				write(fd, &tmpresv, sizeof(RESV));
				break;
			}
		
		}

		printf("*** Complete %d branch - %d room : %02d:00 ~ %02d:00(%d hours) - %02d person reservation.\n",
				resvp->branch, resvp->room, resvp->time, resvp->time + resvp->resv_time, resvp->resv_num);

		init_input();
		close(fd);
	}
	else if(!isFirst){
		pr_us_resv_info2(resv_v);

	}


	return;
}

void add_branch(void){

}

void edit_branch(void){

}

void delete_branch(void){

}


void ins_rm(int rmnum){

}

void apd_rm(int rmnum){

}

void del_rm(int rmnum){

}

void sort_rm(int rmnum){

}

char* get_roomNum(int brnum, int rmnum){

}


void edt_admin(int offset){

}

void edt_resv(int offset){

}


void pr_ad_select_mode(void){
	printf("-----Select Mode-----\n");
	printf("|	1. Admin mode	|\n");
	printf("|	2. User mode	|\n");
	printf("|	3. Exit			|\n");
	printf("---------------------\n");
	return;
}

void pr_ad_branch_info(void){
	
}

void pr_ad_usr_mode(void){

}	

void pr_ad_room_info(int brnum){

}

void pr_us_id_input(void){

}	

void pr_us_br_info(void){

}

void pr_us_rm_info(int brnum){

}

void pr_us_resv_info1(int rmnum){

}

void pr_us_resv_info2(RESV* resv_v[]){

}

void init_input(RESV* resvp){
	memset(resvp->ID, 0, 11);
	resvp->branch = 0;
	resvp->room = 0;
	resvp->date = {0, };
	resvp->time = 0;
	resvp->resv_time = 0;
	resvp->resv_num = 0;
	return;
}

int split(char* string, char* seperator, char* argv[]){
	int argc = 0;
	char* ptr = NULL;

	ptr = strtok(string, seperator);
	while(ptr != NULL){
		argv[argc++] = ptr;
		ptr = strtok(NULL, " ");
	}
	return argc;
}

int get_input(void){
	int inputNum;
	int argc;
	char* argv[ARGMAX];
	memset(input, '\0', 20);
	for(int i=0; i<ARGMAX; i++)
		memset(argv[i], '\0', ARGMAX);

	fgets(input, 20, stdin);
	input[strlen(input)-1] = '\0';
	argc = split(input, " ", argv);
	inputNum = atoi(argv[0]);
	return inputNum;
}

