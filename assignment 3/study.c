#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define RESV_FILE "./reservation.txt"
#define ADMIN_FILE "./admin.txt"
#define BUF_SIZE 1024
#define PATH_SIZE 1024
#define RESV_NUM 100
#define FILENAME_SIZE 10
#define DIRNAME_SIZE 8
#define ARGMAX 10


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
RESV tmpresv;
int dcnt;
char input[20];
char buf[BUF_SIZE];


/***** prompt *****/
void admin_prompt(void);
void user_prompt(RESV* resvp, RESV* resv_v);

/***** branch edit *****/ 	   	// make directory
void add_branch(void);		 		   	// add branch
void edit_branch(void);					// edit branch
void delete_branch(void); 				   	// delete branch

/***** reservation edit *****/
void edit_reservation(void);			// edit reservation in user mode
void cancel_reservation(void);			// cancle reservation in user mode

/***** edit file *****/
void edt_roomfile(char* opt, int idx); 	   	// admin file format :	#br-rm  #_maxnum\n
void edt_resvfile(char* opt, int idx); 	   	// resv file format : 	#ID  #brn  #rm  #num  #date  #time  #duration\n

/***** print info *****/
void pr_ad_select_mode(void);					// admin_selece Mode
void pr_ad_branch_info(void); 					// admin_branch info
void pr_ad_usr_mode(void); 						// admin_input ID
void pr_ad_room_info(int brnum); 				// admin_room information
void pr_us_id_input(void); 						// user_input id prompt
void pr_us_br_info(void); 						// user_branch info
void pr_us_rm_info(int brnum);					// user_room info
void pr_us_resv_info1(int brnum, int rmnum);	// user_reservation info1(new reservation)
int pr_us_resv_info2(RESV* resv_v);				// user_reservation info2(edit reservation)

/***** other functions *****/
void init_resv_v(RESV* resv_v);
void init_input(RESV* resvp);							// initialize input (global)value
int split(char* string, char* seperator, char* argv[]); // parsing function
int get_input(char* input);									// get input number


int main(){
	int fd;
	int argc;
	char* argv[ARGMAX];

	while(1){

		system("clear");
		memset(input, '\0', 20);
		pr_ad_select_mode();
		printf("input >> ");
		fgets(input, sizeof(input), stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);

		if(!strcmp(argv[0], "exit"))
			exit(0);
		
		if(argc == 0 || argc > 1)
			continue;

		if(atoi(argv[0]) < 0 || atoi(argv[0]) > 3){
			printf("wrong input.. select 1 or 2\n");
			sleep(1);
			continue;
		}

		switch(atoi(argv[0])){
			case 1:
				admin_prompt();
				break;
			case 2:
				user_prompt(&resv, resv_v);
				break;
			case 3:
				exit(0);
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

	fflush(stdin);	
	inputNum = get_input(input);
	printf("inputNum : %d\n", inputNum);

	switch(inputNum){
		case 1:
			system("clear");
			pr_ad_branch_info();
			add_branch();
			break;
			
		case 2:
			system("clear");
			pr_ad_branch_info();
			edit_branch();
			break;

		case 3:
			system("clear");
			pr_ad_branch_info();
			delete_branch();
			break;

		case 4:
			printf("program exit..\n");
			sleep(1);
			exit(0);
		
		case 5:
			init_input(&resv);
			init_resv_v(resv_v);
			return;

		default:
			printf("Wrong input. Back to First step..\n");
			sleep(1);
			init_input(&resv);
			init_resv_v(resv_v);
			system("clear");
			return;
	}

	return;
}

void user_prompt(RESV* resvp, RESV* resv_v){
	system("clear");
	pr_us_id_input();

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

	if((fd = open(RESV_FILE, O_RDONLY)) < 0){
		fprintf(stderr, "open \"%s\" error\n", RESV_FILE);
		exit(1);
	}
	
	memset(buf, '\0', BUF_SIZE);
	dcnt = 1;
	int isFirst = 1;
	while(length = read(fd, buf, sizeof(RESV)+7) > 0){
		buf[strlen(buf)-1] = '\0';
		argc = split(buf, " ", argv);

		if(!strcmp(argv[0], resvp->ID)){
			strcpy(resv_v[dcnt].ID, argv[0]);
			resv_v[dcnt].branch = atoi(argv[1]);
			resv_v[dcnt].room = atoi(argv[2]);
			strcpy(resv_v[dcnt].date, argv[3]);
			resv_v[dcnt].time = atoi(argv[4]);
			resv_v[dcnt].resv_time = atoi(argv[5]);
			resv_v[dcnt].resv_num = atoi(argv[6]);
			isFirst = 0;
			dcnt++;
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
		sprintf(pathbuf, "%s/%s-%s", tmpbuf, tmpbuf, argv[0]);
		resvp->room = atoi(argv[0]);					// input #reservation_room
		if(access(pathbuf, F_OK) < 0){
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
		pr_us_resv_info1(resvp->branch, resvp->room);

		printf("input reservation date >> ");
		fgets(input, 20, stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);

		strcpy(resvp->date, argv[0]);				// input date
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
		if(resvp->time + resvp->resv_time > 22){
			printf("valid reservation time : 08:00 ~ 22:00\n");
			printf("return to First step..\n");
			return;
		}


		printf("input person number >> ");
		fgets(input, 20, stdin);
		input[strlen(input)-1] = '\0';
		argc = split(input, " ", argv);
		
		resvp->resv_num = atoi(argv[0]);				// input resv_num



		/*** for "reservation.txt" file ***/
		if((fd = open(RESV_FILE, O_RDWR)) < 0){
			fprintf(stderr, "open error in \"%s\"\n", RESV_FILE);
			exit(1);
		}

		memset(tmpbuf, '\0', PATH_MAX/2);
		sprintf(tmpbuf, "%s %d %d %s %d %d %d\n", resvp->ID, resvp->branch, resvp->room, resvp->date, 
													resvp->time, resvp->resv_time, resvp->resv_num);

		int t;
		char tmp[PATH_MAX/2];
		memset(tmp, '\0', PATH_MAX/2);

		while(length = read(fd, buf, sizeof(RESV)+7) > 0){
			buf[strlen(buf)-1] = '\0';
			argc = split(buf, " ", argv);

			t = lseek(fd, 0, SEEK_CUR);
			strcpy(tmp, buf);
			if(strcmp(argv[3], resvp->date) == 0){ 
				if(resvp->time < atoi(argv[4]) && resvp->time + resvp->resv_time <= atoi(argv[4])){
					


					int k;
					strcpy(temp, tmpbuf);


					while(read(fd, buf, sizeof(RESV)+7) > 0){
						strcpy(temp, buf);
						k = lseek(fd, -(sizeof(RESV1)+7), SEEK_CUR);
						write(fd, tmpbuf, sizeof(RESV)+7);
			//			memset(temp, '\0', sizeof(RESV)+7);

					}
					lseek(fd, t, SEEK_SET);


//					write(fd, tmpbuf, sizeof(RESV)+7);

					break;
				}
			}
			else if(strcmp(argv[3], resvp->date) > 0)
				continue;
			else{ // next date
				write(fd, tmpbuf, sizeof(RESV)+7);
				break;
			}
		
		}
		close(fd);
		/*********************************/




		/*** for "#brNum-#rmNum" file ***/
		if((fd = open(pathbuf, O_RDWR)) < 0){
			fprintf(stderr, "open error in \"%s\"\n", pathbuf);
			exit(1);
		}

		memset(tmpbuf, '\0', PATH_MAX/2);
		sprintf(tmpbuf, "%s %d %d %s %d %d %d\n", resvp->ID, resvp->branch, resvp->room, resvp->date, 
													resvp->time, resvp->resv_time, resvp->resv_num);

		while(length = read(fd, buf, sizeof(RESV)+7) > 0){
			buf[strlen(buf)-1] = '\0';
			argc = split(buf, " ", argv);

			if(strcmp(argv[3], resvp->date) == 0){ 
				if(resvp->time < atoi(argv[4]) && resvp->time + resvp->resv_time <= atoi(argv[4])){
					write(fd, tmpbuf, sizeof(RESV)+7);
					break;
				}
			}
			else if(strcmp(argv[3], resvp->date) > 0)
				continue;
			else{ // next date
				write(fd, tmpbuf, sizeof(RESV)+7);
				break;
			}
		
		}
		close(fd);
		/*********************************/




		printf("*** Complete %d branch - %d room : %02d:00 ~ %02d:00(%d hours) - %02d person reservation.***\n",
				resvp->branch, resvp->room, resvp->time, resvp->time + resvp->resv_time, resvp->resv_time, resvp->resv_num);

		init_input(resvp);
		init_resv_v(resv_v);
		close(fd);
	}
	else if(!isFirst){
		int inputNum;
		int noResv;
		system("clear");
		noResv = pr_us_resv_info2(resv_v);
		if(noResv == 1){
			printf("Return to first step..\n");
			init_input(resvp);
			init_resv_v(resv_v);
			sleep(1);
			return;
		}

		printf("1) Edit reservation\n");
		printf("2) Cancel reservation\n");
		printf("3) Return to first step\n\n");
		
		printf("Input >> ");
		inputNum = get_input(input);

		switch(inputNum){
			case 1:
				edit_reservation();
				break;
				
			case 2:
				cancel_reservation();
				break;

			case 3:
				printf("Return to first step..\n");
				init_input(resvp);
				init_resv_v(resv_v);
				sleep(1);
				return;

			default:
				printf("Wrong input. Return to first step..\n")	;
				init_input(resvp);
				init_resv_v(resv_v);
				sleep(1);
				return;
		}
	}

	return;
}

void add_branch(void){

}

void edit_branch(void){
	

}

void delete_branch(void){

}


void edit_reservation(void){
	int resv_idx;
	int inputNum;
	int tmpbr;
	char tmpbuf[DIRNAME_SIZE];
	system("clear");
	pr_us_resv_info2(resv_v);

	/************   ID  **************/
	printf("input #index to edit reservation >> ");
	resv_idx = get_input(input);

	if(resv_idx == 0 || resv_idx >= dcnt){
		printf("Wrong input. Return to First step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}

	strcpy(tmpresv.ID, resv_v[resv_idx].ID);
	/*********************************/




	/************   #branch  **************/
	system("clear");
	pr_us_br_info();
	printf("input branch number >> ");
	inputNum = get_input(input);

	if(inputNum == 0){
		printf("return to First step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}
	memset(tmpbuf, '\0', DIRNAME_SIZE);
	sprintf(tmpbuf, "./%d", inputNum);
	if(access(tmpbuf, F_OK) < 0){
		printf("Wrong input. Return to First step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}
	tmpresv.branch = inputNum;								
	tmpbr = inputNum;
	/*****************************************/
	



	/***********   #room ************/
	system("clear");
	pr_us_rm_info(inputNum);
	printf("input room number >> ");
	inputNum = get_input(input);
	if(inputNum == 0){
		printf("return to First step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}
	memset(tmpbuf, '\0', DIRNAME_SIZE);
	sprintf(tmpbuf, "./%d/%d-%d", tmpbr, tmpbr, inputNum);
	if(access(tmpbuf, F_OK) < 0){
		printf("Wrong input. Return to First step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}
	tmpresv.room = inputNum;								
	/*********************************/




	/***********   date	 ************/
	system("clear");
	time_t t = time(NULL);
	struct tm lt;
	localtime_r(&t, &lt);
	char date[7];
	char* argv[ARGMAX];
	int argc;
	memset(date, '\0', 7);
	sprintf(date, "%02d%02d%02d", lt.tm_year+1900-2000, lt.tm_mon+1, lt.tm_mday);
	
	pr_us_resv_info1(tmpresv.branch, tmpresv.room);
	printf("input reservation date >> ");
	fgets(input, sizeof(input), stdin);
	input[strlen(input)-1] = '\0';
	argc = split(input, " ", argv);

	strcpy(tmpresv.date, argv[0]);								
	if(strcmp(date, tmpresv.date) >= 0){
		printf("input reservation date is invalid.\n");
		printf("return to First step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}
	/*********************************/





	/***********   time	 ************/
	printf("input reservation start time >> ");
	tmpresv.time = inputNum = get_input(input);				
	if(tmpresv.time < 8 || tmpresv.time >= 22){
		printf("valid reservation time : 08:00 ~ 22:00\n");
		printf("Return to first step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}
	/*********************************/




	/***********   resv_time	 ************/
	printf("input reservation period >> ");
	tmpresv.resv_time = inputNum = get_input(input);			
	if(tmpresv.time + tmpresv.resv_time > 22){
		printf("valid reservation time : 08:00 ~ 22:00\n");
		printf("Return to first step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;

	}
	/****************************************/





	/***********   resv_num	 ************/
	printf("input person number >> ");
	tmpresv.resv_num = inputNum = get_input(input);			
	if(tmpresv.resv_num > resv_v[resv_idx].resv_num){
		printf("Maximum number of room %d : %d\n", resv_v[resv_idx].room, resv_v[resv_idx].resv_num);
		printf("Return to first step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}
	/*************************************/


	edt_roomfile("add", resv_idx);
	edt_resvfile("add", resv_idx);
	
	init_resv_v(resv_v);
	init_input(&resv);
	sleep(1);
	return;
}

void cancel_reservation(void){	
	int inputNum;
	pr_us_resv_info2(resv_v);

	printf("input #index to cancel reservation >> ");		
	inputNum = get_input(input);

	if(inputNum == 0){
		printf("Wrong input. Return to first step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}
	if(inputNum < 0 || inputNum >= dcnt){
		printf("Wrong input. Return to first step..\n");
		init_resv_v(resv_v);
		init_input(&resv);
		sleep(1);
		return;
	}

	edt_roomfile("delete", inputNum);
	edt_resvfile("delete", inputNum);

	init_resv_v(resv_v);
	init_input(&resv);
	sleep(1);
	return;
}

void edt_roomfile(char* opt, int idx){
	int argc;
	char* argv[ARGMAX];
	int fd;
	int length;
	int br = resv_v[idx].branch;
	int rm = resv_v[idx].room;
	char fname[FILENAME_SIZE];
	memset(fname, '\0', FILENAME_SIZE);
	sprintf(fname, "./%d/%d-%d", br, br, rm);
	char nullVal[sizeof(RESV)+7];
	memset(nullVal, '\0', sizeof(RESV)+7);

	if((fd = open(fname, O_RDWR)) < 0){
		fprintf(stderr, "open error in %s\n", fname);
		exit(1);
	}

	if(!strcmp(opt, "delete")){

		while(length = read(fd, buf, sizeof(RESV)+7) > 0){
			buf[strlen(buf)-1] = '\0';
			argc = split(buf, " ", argv);
			
			if(!strcmp(argv[0], resv_v[idx].ID) && !strcmp(argv[3], resv_v[idx].date) && atoi(argv[1]) == resv_v[idx].branch &&
					atoi(argv[2]) == resv_v[idx].room && atoi(argv[4]) == resv_v[idx].time){
				lseek(fd, -(sizeof(RESV)+7), SEEK_CUR);
				write(fd, nullVal, sizeof(RESV)+7);
				break;
			}
		}
	}
	else if(!strcmp(opt, "add")){
		char tmpbuf[PATH_SIZE/2];
		memset(tmpbuf, '\0', PATH_SIZE/2);
		sprintf(tmpbuf, "%s %d %d %s %d %d %d\n", tmpresv.ID, tmpresv.branch, tmpresv.room, tmpresv.date, tmpresv.time,
													tmpresv.resv_time, tmpresv.resv_num);
	
		/*********** delete original record ***********/
		while(length = read(fd, buf, sizeof(RESV)+7) > 0){
			int curpos;
			buf[strlen(buf)-1] = '\0';
			argc = split(buf, " ", argv);
			
			if(!strcmp(argv[0], resv_v[idx].ID) && !strcmp(argv[3], resv_v[idx].date) && atoi(argv[1]) == resv_v[idx].branch &&
					atoi(argv[2]) == resv_v[idx].room && atoi(argv[4]) == resv_v[idx].time){
				lseek(fd, -(sizeof(RESV)+7), SEEK_CUR);
				write(fd, nullVal, sizeof(RESV)+7);
				curpos = lseek(fd, 0, SEEK_CUR);
				while(length = read(fd, buf, sizeof(RESV)+7) > 0){
					




				}
				break;
			}
		}
		lseek(fd, 0, SEEK_SET);


		/************* add new record **************/
		while(length = read(fd, buf, sizeof(RESV)+7) > 0){
			buf[strlen(buf)-1] = '\0';
			argc = split(buf, " ", argv);

			if(strcmp(argv[3], tmpresv.date) == 0){
				if(tmpresv.time < atoi(argv[4]) && tmpresv.time + tmpresv.resv_time <= atoi(argv[4])){
					write(fd, tmpbuf, sizeof(RESV)+7);
					break;
				}
			}
			else if(strcmp(argv[3], tmpresv.date) > 0)
				continue;
			else{
				write(fd, tmpbuf, sizeof(RESV)+7);
				break;
			}
		}
	}
	else{
		fprintf(stdout, "wrong \"opt\" argument");
		exit(1);
	}

	close(fd);
	return;
}

void edt_resvfile(char* opt, int idx){
	int argc;
	char* argv[ARGMAX];
	int fd;
	char nullVal[sizeof(RESV)+7];
	memset(nullVal, '\0', sizeof(RESV)+7);

	if((fd = open(RESV_FILE, O_RDWR)) < 0){
		fprintf(stderr, "open error in %s\n", fname);
		exit(1);
	}

	if(!strcmp(opt, "delete")){
		while(length = read(fd, buf, sizeof(RESV)+7) > 0){
			buf[strlen(buf)-1] = '\0';
			argc = split(buf, " ", argv);
			
			if(!strcmp(argv[0], resv_v[idx].ID) && !strcmp(argv[3], resv_v[idx].date) && atoi(argv[1]) == resv_v[idx].branch &&
					atoi(argv[2]) == resv_v[idx].room && atoi(argv[4]) == resv_v[idx].time){
				lseek(fd, -(sizeof(RESV)+7), SEEK_CUR);
				write(fd, nullVal, sizeof(RESV)+7);
				break;
			}
		}

		printf("*** Reservation Canceled successfully ***\n");
	}
	else if(!strcmp(opt, "add")){
		char tmpbuf[PATH_SIZE/2];
		memset(tmpbuf, '\0', PATH_SIZE/2);
		sprintf(tmpbuf, "%s %d %d %s %d %d %d\n", tmpresv.ID, tmpresv.branch, tmpresv.room, tmpresv.date, tmpresv.time,
													tmpresv.resv_time, tmpresv.resv_num);
		
		/*********** delete original record ***********/
		while(length = read(fd, buf, sizeof(RESV)+7) > 0){
			buf[strlen(buf)-1] = '\0';
			argc = split(buf, " ", argv);
			
			if(!strcmp(argv[0], resv_v[idx].ID) && !strcmp(argv[3], resv_v[idx].date) && atoi(argv[1]) == resv_v[idx].branch &&
					atoi(argv[2]) == resv_v[idx].room && atoi(argv[4]) == resv_v[idx].time){
				lseek(fd, -(sizeof(RESV)+7), SEEK_CUR);
				write(fd, nullVal, sizeof(RESV)+7);
				break;
			}
		}
		lseek(fd, 0, SEEK_SET);


		/************* add new record **************/
		while(length = read(fd, buf, sizeof(RESV)+7) > 0){
			buf[strlen(buf)-1] = '\0';
			argc = split(buf, " ", argv);

			if(strcmp(argv[3], tmpresv.date) == 0){
				if(tmpresv.time < atoi(argv[4]) && tmpresv.time + tmpresv.resv_time <= atoi(argv[4])){
					write(fd, tmpbuf, sizeof(RESV)+7);
					break;
				}
			}
			else if(strcmp(argv[3], tmpresv.date) > 0)
				continue;
			else{
				write(fd, tmpbuf, sizeof(RESV)+7);
				break;
			}
		}
		printf("*** Reservation Edited successfully ***\n");
	}
	else{
		fprintf(stdout, "wrong \"opt\" argument");
		exit(1);
	}

	close(fd);
	return;
}


void pr_ad_select_mode(void){
	printf("-----Select Mode-----\n");
	printf("1. Admin mode	\n");
	printf("2. User mode	\n");
	printf("3. Exit			\n");
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
	printf("---------------------\n");
	printf("Input your ID\n");
	printf("---------------------\n\n");
	return;
}	

void pr_us_br_info(void){
	printf("---------------------\n");
	printf(" *Branch status\n");
	
	char dirname[DIRNAME_SIZE];
	for(int i=1; i<7; i++){
		memset(dirname, '\0', DIRNAME_SIZE);
		sprintf(dirname, "%d", i);
		if(access(dirname, F_OK) == 0)
				printf("#%s branch\n", dirname);
	}
	printf("---------------------\n");
	return;
}

void pr_us_rm_info(int brnum){
	printf("---------------------\n");
	printf(" *Room status	\n");
	
	char dirname[DIRNAME_SIZE];
	char fname[FILENAME_SIZE];
	memset(dirname, '\0', DIRNAME_SIZE);
	sprintf(dirname, "./%d", brnum);

	chdir(dirname);
	for(int j=1; j<6; j++){
		memset(fname, '\0', FILENAME_SIZE);
		sprintf(fname, "./%d-%d", brnum, j);
		if(access(fname, F_OK) == 0)
			printf("#%s room\n", fname);
	}
	
	printf("---------------------\n");
	chdir("../");
	return;
}

void pr_us_resv_info1(int brnum, int rmnum){
	int argc;
	char* argv[ARGMAX];

	int fd;
	int length;
	char fname[PATH_MAX];
	memset(fname, '\0', PATH_MAX);
	sprintf(fname, "./%d/%d-%d", brnum, brnum, rmnum);

	if((fd = open(fname, O_RDONLY)) < 0){
		fprintf(stderr, "open error in %s\n", RESV_FILE);
		exit(1);
	}

	printf("-----------------------------\n");
	printf("|	Date		Time		|\n");
	while((length = read(fd, buf, sizeof(RESV)+7)) > 0){
		buf[strlen(buf)-1] = '\0';
		argc = split(buf, " ", argv);
		printf("|	%s		%02d:00 ~ %02d:00|\n", argv[3], atoi(argv[4]), atoi(argv[4])+atoi(argv[5]));
	}
	printf("-----------------------------\n");
	close(fd);
	return;
}

int pr_us_resv_info2(RESV* resv_v){
	int argc;
	char* argv[ARGMAX];

	if(dcnt == 1){
		printf("There is no reservation\n");
		return 1;
	}
	else{
		printf("-------------------------------------------------------------\n");
		printf(" *reservation status										\n");
		printf("index	#branch	 #room	date	time	reservation time	\n");
		
		for(int i=1; i<dcnt; i++){
			printf("%d	%d	%d	%s	%02d:00 ~ %02d:00	%d hour(s)\n", 
					i, resv_v[i].branch, resv_v[i].room, resv_v[i].date, resv_v[i].time, 
					resv_v[i].time + resv_v[i].resv_time, resv_v[i].resv_time);
		}
		printf("-------------------------------------------------------------\n");
	}
	return 0;
}


void init_resv_v(RESV* resv_v){
	for(int i=1; i<dcnt; i++)
		init_input(&resv_v[i]);

	dcnt = 1;
	return;
}

void init_input(RESV* resvp){
	memset(resvp->ID, 0, 11);
	resvp->branch = 0;
	resvp->room = 0;
	memset(resvp->date, 0, 6);
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

int get_input(char* input){
	int inputNum;
	int argc;
	char* argv[ARGMAX];
	memset(input, '\0', 20);

	fgets(input, sizeof(input), stdin);
	input[strlen(input)-1] = '\0';
	argc = split(input, " ", argv);
	for(int i=0; i<argc; i++)
		printf("argv[%d] : %s\n", i, argv[i]);
	inputNum = atoi(argv[0]);
	return inputNum;
}

