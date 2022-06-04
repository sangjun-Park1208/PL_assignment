#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define RESV_FILE "./reservation.txt"
#define ADMIN_FILE "./admin.txt"

int split(char* string, char* seperator, char* argv[]);
void print_select_mode(void);
void print_branch_information(void);
void admin_mode(void);
void user_mode(void);
void add_branch(void);
void edit_branch(void);
void remove_branch(void);
void add_room(int brNum);
void edit_room(int brNum);
void remove_room(int brNum);
int get_input(void);
int isAlpha(int c);
int isDigit(int c);
void new_reservation(char* id);
void origin_reservation(char* id);
void add_reservation(char* id, int brNum, int rmNum);
void print_reservation_info(int brNum, int rmNum);
void print_user_reservation_info(char* id);
void edit_reservation(char* id);
void editing_resv(int idx, char* id, int brNum, int rmNum);



typedef struct _RESV{
	char ID[11];
	char date[7];
	int time;
	int usetime;
	int branch;
	int room;
	int person;
}RESV;

int max_person[6][5];
char br_isopen[6];
char br_adminopen[6];
char rm_isopen[6][5];


int main(){
	int input, fd, argc, brcnt;
	char* argv[10];
	char mpbuf[16];
	char rsbuf[34];

	if(access(ADMIN_FILE, F_OK) < 0){
		if((fd = open(ADMIN_FILE, O_RDWR | O_CREAT, 0644)) < 0){
			fprintf(stderr, "open error in admin.txt\n");
			exit(1);
		}
		
		sprintf(mpbuf, "%02d %02d %02d %02d %02d\n", 0, 0, 0, 0, 0);
		for(int i=0; i<6; i++)
			write(fd, mpbuf, sizeof(mpbuf));
		
	
		close(fd);
	
	}

	if(access(RESV_FILE, F_OK) < 0){
		if((fd = open(RESV_FILE, O_RDWR | O_CREAT, 0644)) < 0){
			fprintf(stderr, "open error in reservation.txt\n");
			exit(1);
		}
		close(fd);
	}



	while(1){
		if((fd = open(ADMIN_FILE, O_RDWR | O_CREAT, 0644)) < 0){
			 fprintf(stderr, "open admin.txt error\n");
			 exit(1);
		}

		for(int i=0; i<6; i++){
			read(fd, mpbuf, sizeof(mpbuf));
			mpbuf[strlen(mpbuf)-1] = '\0';
			argc = split(mpbuf, " ", argv);
			for(int j=0; j<5; j++){
				max_person[i][j] = atoi(argv[j]);
				if(max_person[i][j] != 0)
					rm_isopen[i][j] = 'o';
				else
					rm_isopen[i][j] = 'x';

			}

		}

		for(int i=0; i<6; i++){
			brcnt = 0;
			for(int j=0; j<5; j++){
				if(rm_isopen[i][j] == 'o'){
					br_isopen[i] = 'o';
					break;
				}
				brcnt++;
			}
			if(br_adminopen[i] == 'o')
				br_isopen[i] = 'o';
			else if(brcnt == 5 && br_adminopen[i] != 'o')
				br_isopen[i] = 'x';
		}


		close(fd);


		system("clear");
		print_select_mode();
		printf("	---------------------------\n\n");
		printf("		 >> ");
		input = get_input();
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
	printf("	[Study Cafe Reservation System]\n\n");
	printf("		Select Mode\n\n");
	printf("		1. Admin mode\n");
	printf("		2. User mode\n");
	printf("		3. Exit\n\n");
	return;
}

void print_branch_information(void){
	printf("		Branch list\n\n");
	for(int i=0; i<6; i++){
		if(br_isopen[i] == 'o')
			printf("		#%d (open)\n", i+1);
		else
			printf("		#%d (closed)\n", i+1);
	}
	return;
}

void print_room_information(int branchNum){
	printf("		Branch [%d] - Room list\n", branchNum);
	printf(" 		#Room			Capacity\n");
	for(int i=0; i<5; i++){
		if(rm_isopen[branchNum-1][i] == 'o'){
			printf("		#%d (open)		%d\n", i+1, max_person[branchNum-1][i]);
		}
		else{
			printf("		#%d (closed)\n", i+1);
		}
	}
	return;
}

void admin_mode(void){
	int input;
	while(1){
		system("clear");
		print_branch_information();
		printf("\n		--------------------------\n");
		printf("		1) Add branch\n");
		printf("		2) Edit branch\n");
		printf("		3) Remove branch\n");
		printf("		4) Back to menu\n");
		printf("\n		--------------------------\n\n");

		printf("		 >> ");

		input = get_input();

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
	int fd, input;
	if((fd = open(ADMIN_FILE, O_RDWR | O_CREAT, 0644)) < 0){
		fprintf(stderr, "open error in admin.txt\n");
		exit(1);
	}
	
	while(1){
		system("clear");
		print_branch_information();
		printf("\n		(input \"0\" back to menu)\n");
		printf("		 input #branch to open >> ");

		input = get_input();
	
		if(input == 0){
			printf("Back to menu..\n");
			sleep(1);
			break;
		}
		if(input < 1 || input > 6){
			printf("input (1~6)\n");
			sleep(1);
			continue;
		}
		else{
			if(br_isopen[input-1] == 'o'){
				printf("Already opened branch..\n");
				sleep(1);
				continue;
			}
			else{
				br_adminopen[input-1] = 'o';
				printf("Successfully open branch #%d\n", input);
				sleep(1);
				break;
			}
		}	
	}

	close(fd);

	return;
}

void edit_branch(void){
	int fd, input1, input2;
	if((fd = open(ADMIN_FILE, O_RDWR | O_CREAT, 0644)) < 0){
		fprintf(stderr, "open error in admin.txt\n");
		exit(1);
	}

	while(1){
		system("clear");
		print_branch_information();
		printf("\n	----------------------------------------------------\n");
		printf("\n		(input \"0\" back to menu)\n");
		printf("\n		 input #branch to edit >> ");
		input1 = get_input();
	
		if(input1 == 0){
			printf("Back to menu..\n");
			close(fd);
			sleep(1);
			break;
		}
		if(input1 < 1 || input1 > 6){
			printf("input (1~6)\n");
			sleep(1);
			continue;
		}
		else{
			if(br_isopen[input1-1] != 'o'){
				printf("#%d branch is closed. Open branch first.\n", input1);
				sleep(1);
				continue;
			}
			else{
				while(1){
					system("clear");
					print_room_information(input1);
					printf("\n");
					printf("		1) Add study room\n");
					printf("		2) Edit study room capacity\n");
					printf("		3) Remove study room\n");
					printf("		4) Back to menu\n");
					printf("\n----------------------------------------------\n\n");
					
					printf("		 >> ");

					input2 = get_input();
	
					if(input2 == 1){
						add_room(input1);
						close(fd);
						return;
					}
					else if(input2 == 2){
						edit_room(input1);
						close(fd);
						return;
					}
					else if(input2 == 3){
						remove_room(input1);
						close(fd);
						return;
					}
					else if(input2 == 4){
						printf("Back to menu..\n");
						close(fd);
						sleep(1);
						return;
					}
					else{
						printf("Wrong input. Select (1~4)\n");
						sleep(1);
						continue;
					}
				}
			}
		}
	}

	return;
}


void add_room(int brNum){
	int fd, input1, input2, argc;
	int ibuf[5];
	char* argv[10];
	char admbuf[16];
	if((fd = open(ADMIN_FILE, O_RDWR)) < 0){
		fprintf(stderr, "open error in admin.txt");
		exit(1);
	}

	while(1){
		system("clear");
		print_room_information(brNum);
		printf("\n");
		printf("		 input #room to add >> ");
		input1 = get_input();
		
		if(input1 < 1 || input1 > 5){
			printf("Wrong input. Select(1~5)\n");
			sleep(1);
			continue;
		}

		if(rm_isopen[brNum-1][input1-1] == 'o'){
			printf("#%d room is already opened. Select other number\n", input1);
			sleep(1);
			continue;
		}

		printf("		 input Max Capacity of #%d room >> ", input1);
		input2 = get_input();

		if(input2 <= 0){
			printf("Wrong input. Capacity should be positive.\n");
			sleep(1);
			continue;
		}


		lseek(fd, (brNum-1)*sizeof(admbuf), SEEK_SET);		
		read(fd, admbuf, sizeof(admbuf));
		admbuf[strlen(admbuf)-1] = '\0';
		argc = split(admbuf, " ", argv);
		
		for(int i=0; i<5; i++){
			ibuf[i] = atoi(argv[i]);
		}

		ibuf[input1-1] = input2;
		
		sprintf(admbuf, "%02d %02d %02d %02d %02d\n", ibuf[0], ibuf[1], ibuf[2], ibuf[3], ibuf[4]);

		lseek(fd, (brNum-1)*sizeof(admbuf), SEEK_SET);		
		write(fd, admbuf, sizeof(admbuf));

		printf("***  #%d branch - #%d room has opened. ***\n", brNum, input1);
		printf("Back to menu..\n");
		sleep(1);
		break;
	}


	close(fd);
	return;
}

void edit_room(int brNum){
	int fd, input1, input2, argc;
	int ibuf[5];
	char* argv[10];
	char admbuf[16];
	if((fd = open(ADMIN_FILE, O_RDWR)) < 0){
		fprintf(stderr, "open error in admin.txt");
		exit(1);
	}

	while(1){
		system("clear");
		print_room_information(brNum);
		printf("\n");
		printf("		 input #room to edit >> ");
		input1 = get_input();
		
		if(input1 < 1 || input1 > 5){
			printf("Wrong input. Select(1~5)\n");
			sleep(1);
			continue;
		}

		if(rm_isopen[brNum-1][input1-1] != 'o'){
			printf("#%d room is closed. First open the room.\n", input1);
			sleep(1);
			continue;
		}
		
		printf("		 input Capacity for #%d room >> ", input1);
		input2 = get_input();

		if(input2 <= 0){
			printf("Wrong input. Capacity should be positive.\n");
			sleep(1);
			continue;
		}

		lseek(fd, (brNum-1)*sizeof(admbuf), SEEK_SET);		
		read(fd, admbuf, sizeof(admbuf));
		admbuf[strlen(admbuf)-1] = '\0';
		argc = split(admbuf, " ", argv);
		
		for(int i=0; i<5; i++){
			ibuf[i] = atoi(argv[i]);
		}

		ibuf[input1-1] = input2;
		
		sprintf(admbuf, "%02d %02d %02d %02d %02d\n", ibuf[0], ibuf[1], ibuf[2], ibuf[3], ibuf[4]);

		lseek(fd, (brNum-1)*sizeof(admbuf), SEEK_SET);		
		write(fd, admbuf, sizeof(admbuf));

		printf("***  #%d branch - #%d room's capacity : %d ***\n", brNum, input1, input2);
		printf("Back to menu..\n");
		sleep(1);
		break;
	}
	close(fd);
	return;
}

void remove_room(int brNum){
	time_t t;
	struct tm lt;
	int fd1, fd2, input1, input2, argc, isEmpty;
	int ibuf[5];
	char* argv[10];
	char admbuf[16];
	char rsbuf[34];
	char datenow[7];
	char timenow[5];
	if((fd1 = open(ADMIN_FILE, O_RDWR)) < 0){
		fprintf(stderr, "open error in admin.txt");
		exit(1);
	}

	while(1){
		t = time(NULL);
		localtime_r(&t, &lt);
		sprintf(datenow, "%02d%02d%02d", lt.tm_year+1900-2000, lt.tm_mon+1, lt.tm_mday);
		sprintf(timenow, "%02d%02d", lt.tm_hour, lt.tm_min);

		system("clear");
		print_room_information(brNum);
		printf("\n");
		printf("		(input \"0\" back to menu)\n");
		printf("		 input #room to remove >> ");
		input1 = get_input();
		
		if(input1 == 0){
			printf("Back to menu..\n");
			sleep(1);
			break;
		}
		if(input1 < 1 || input1 > 5){
			printf("Wrong input. Select(1~5)\n");
			sleep(1);
			continue;
		}

		if(rm_isopen[brNum-1][input1-1] == 'x'){
			printf("#%d room is closed. Try opened room.\n", input1);
			sleep(1);
			continue;
		}

		if((fd2 = open(RESV_FILE, O_RDWR)) < 0){
			fprintf(stderr ,"open error in reservation.txt\n");
			exit(1);
		}

		isEmpty = 1;
		while(read(fd2, rsbuf, sizeof(rsbuf)) > 0){
			rsbuf[strlen(rsbuf)-1] = '\0';
			argc = split(rsbuf, " ", argv);

			if(strcmp(datenow, argv[1]) < 0){
				if(brNum == atoi(argv[4]) && input1 == atoi(argv[5])){
					isEmpty = 0;
					break;
				}
			}
			else if(strcmp(datenow, argv[1]) == 0 && lt.tm_hour < atoi(argv[2])){
				if(brNum == atoi(argv[4]) && input1 == atoi(argv[5])){
					isEmpty = 0;
					break;
				}
			}

		}	
		close(fd2);

		if(isEmpty == 1){
			lseek(fd1, (brNum-1)*sizeof(admbuf), SEEK_SET);		
			read(fd1, admbuf, sizeof(admbuf));
			admbuf[strlen(admbuf)-1] = '\0';
			argc = split(admbuf, " ", argv);
			
			for(int i=0; i<5; i++){
				ibuf[i] = atoi(argv[i]);
			}

			ibuf[input1-1] = 0;
			sprintf(admbuf, "%02d %02d %02d %02d %02d\n", ibuf[0], ibuf[1], ibuf[2], ibuf[3], ibuf[4]);
			
			lseek(fd1, (brNum-1)*sizeof(admbuf), SEEK_SET);		
			write(fd1, admbuf, sizeof(admbuf));
	
			printf("***  #%d branch - #%d room is deleted ***\n", brNum, input1);
			printf("Back to menu..\n");
			sleep(1);
			break;	
		}
		else{
			printf("This room already reserved..\n");
			printf("If you want to remove this room, cancel reservation first.\n");
			sleep(3);
			continue;
		}
	}

	close(fd1);
	return;
}


void remove_branch(void){
	time_t t;
	struct tm lt;
	int fd1, fd2, input, argc, isEmpty, brNum;
	char* argv[10];
	char admbuf[16];
	char rsbuf[34];
	char datenow[7];
	char timenow[5];

	if((fd1 = open(ADMIN_FILE, O_RDWR)) < 0){
		fprintf(stderr, "open error in admin.txt");
		exit(1);
	}
	
	while(1){
		t = time(NULL);
		localtime_r(&t, &lt);
		sprintf(datenow, "%02d%02d%02d", lt.tm_year+1900-2000, lt.tm_mon+1, lt.tm_mday);
		system("clear");
		print_branch_information();
		printf("	\n------------------------------------------\n");
		printf("\n");
		printf("		(input \"0\" back to menu)\n");
		printf("		 input #branch to remove >> ");
		brNum = input = get_input();

		if(input == 0){
			printf("Back to menu..\n");
			sleep(1);
			break;
		}
		
		if(input < 1 || input > 6){
			printf("Wrong input. Select(1~6)\n");
			sleep(1);
			continue;
		}

		if(br_isopen[input-1] == 'x'){
			printf("#%d branch is closed. Try opened branch.\n", brNum);
			sleep(1);
			continue;
		}


		isEmpty = 1;
		if((fd2 = open(RESV_FILE, O_RDWR)) < 0){
			fprintf(stderr, "open error in reservation.txt\n");
			exit(1);
		}
		while(read(fd2, rsbuf, sizeof(rsbuf)) > 0){
			rsbuf[strlen(rsbuf)-1] = '\0';
			argc = split(rsbuf, " ", argv);


			if(strcmp(datenow, argv[1]) < 0){
				if(brNum == atoi(argv[4])){
					isEmpty = 0;
					break;
				}
			}
			else if(strcmp(datenow, argv[1]) == 0 && lt.tm_hour < atoi(argv[2])){
				if(brNum == atoi(argv[4])){
					isEmpty = 0;
					break;
				}
			}



		}	
		close(fd2);

		if(isEmpty == 1){
			lseek(fd1, (brNum-1)*sizeof(admbuf), SEEK_SET);		
			read(fd1, admbuf, sizeof(admbuf));
			admbuf[strlen(admbuf)-1] = '\0';
			argc = split(admbuf, " ", argv);

			sprintf(admbuf, "%02d %02d %02d %02d %02d\n", 0, 0, 0, 0, 0);
			
			lseek(fd1, (brNum-1)*sizeof(admbuf), SEEK_SET);		
			write(fd1, admbuf, sizeof(admbuf));
	
			printf("***  #%d branch is deleted ***\n", brNum);
			printf("Back to menu..\n");
			sleep(1);
			break;	
		}
		else{
			printf("Invalid remove: Some reservation remaining..\n");
			printf("Back to menu..\n");
			sleep(1);
			break;
		}
	}
	close(fd1);
	return;
}

int isAlpha(int c){
	if((c>='a' && c<='z') || (c>='A' && c<='Z'))
		return 1;
	else
		return 0;
}

int isDigit(int c){
	if(c>='0' && c<='9')
		return 1;
	else
		return 0;
}


void user_mode(void){
	char id[30];
	int alpha, digit, fd, isFirst, argc, check;
	char* argv[10];
	char rsbuf[34];
	char datenow[7];

	time_t t;
	struct tm lt;

	while(1){
		t = time(NULL);
		localtime_r(&t, &lt);
		sprintf(datenow, "%02d%02d%02d", lt.tm_year+1900-2000, lt.tm_mon+1, lt.tm_mday);

		system("clear");
		printf("Input your id(length 5~10) : ");
		fgets(id, sizeof(id), stdin);
		if(strlen(id)-1 > 10 || strlen(id)-1 < 5){
			printf("ID length error!\n\n");
			printf("ID length should be 5 ~ 10\n");
			sleep(2);
			continue;
		}
		id[strlen(id)-1] = '\0';
		
		alpha = digit = 0;
		for(int i=0; i<strlen(id); i++){
			alpha += isAlpha(id[i]);
			digit += isDigit(id[i]);
		}

		if(alpha == 0 || digit == 0){
			printf("ID format error!\n");
			printf("ID must contain at least one character & one alphabet\n");
			sleep(2);
			continue;
		}

		if((fd = open(RESV_FILE, O_RDWR)) < 0){
			fprintf(stderr, "open error in reservation.txt\n");
			exit(1);
		}

		isFirst = 1;
		while(read(fd, rsbuf, sizeof(rsbuf)) > 0){
			rsbuf[strlen(rsbuf)-1] = '\0';
			argc = split(rsbuf, " ", argv);

			if(!strcmp(id, argv[0])){
				isFirst = 0;
				break;
			}
		}


		lseek(fd, 0, SEEK_SET);
		if(isFirst == 1){
			new_reservation(id);
			break;
		}
		else{
			check = 0;
			while(read(fd, rsbuf, sizeof(rsbuf)) > 0){
				rsbuf[strlen(rsbuf)-1] = '\0';
				argc = split(rsbuf, " ", argv);

				if(strcmp(datenow, argv[1]) < 0){
					check = 1;
					break;
				}
			}
			if(check == 1){
				origin_reservation(id);
			}
			else{
				new_reservation(id);
			}

			break;
		}
		close(fd);
	}
	return;
}

void new_reservation(char* id){
	int input1, input2;

	while(1){
		system("clear");
		print_branch_information();
		printf("\n		------------------------------------------------\n");
		printf("\n(input \"0\" back to menu)\n");
		printf("		 input #branch to reserve >> ");
		input1 = get_input();

		if(input1 == 0){
			printf("Back to menu..\n");
			sleep(1);
			return;
		}

		if(input1 < 1 || input1 > 6){
			printf("Wrong input. Select(1~6)\n");
			sleep(1);
			continue;
		}
		
		if(br_isopen[input1-1] == 'x'){
			printf("#%d branch is closed. Try opened branch\n", input1);
			sleep(1);
			continue;
		}

		while(1){
			system("clear");
			print_room_information(input1);
			printf("\n	------------------------------------------------\n");
			printf("\n(input \"0\" back to menu)\n");
			printf("		 input #room to reserve >> ");
			input2 = get_input();
	
			if(input2 == 0){
				printf("Back to menu..\n");
				sleep(1);
				return;
			}
	
			if(input2 < 1 || input2 > 5){
				printf("Wrong input. Select(1~5)\n");
				sleep(1);
				continue;
			}
			
			if(rm_isopen[input1-1][input2-1] == 'x'){
				printf("#%d room is closed. Try opened room\n", input2);
				sleep(1);
				continue;
			}

			add_reservation(id, input1, input2);
			break;
		}
		break;
	}
	return;
}

void origin_reservation(char* id){
	int input;

	while(1){
		system("clear");
		print_user_reservation_info(id);
		printf("		1) Add reservation\n");
		printf("		2) Edit reservation\n");
		printf("		3) Back to menu\n");
		printf("\n	--------------------------------\n");
		printf("		 >> ");
		input = get_input();

		if(input == 1){
			new_reservation(id);
			break;
		}
		else if(input == 2){
			edit_reservation(id);
			break;
		}
		else if(input == 3){
			printf("Back to menu..\n");
			sleep(1);
			return;
		}
		else{
			printf("Wrong input. Select(1~3)\n");
			sleep(1);
			continue;
		}
	}
	return;
}

void edit_reservation(char* id){
	int fd, idx, input1, input2, rrn;
	
	if((fd = open(RESV_FILE, O_RDWR)) < 0){
		fprintf(stderr, "open error in reservation.txt\n");
		exit(1);
	}

	while(1){
		system("clear");
		print_user_reservation_info(id);
		printf(" input #Index to edit reservation >> ");
		idx = get_input();
		
		if(idx < 1){
			printf("Wrong input. Index must bigger than 0\n");
			sleep(1);
			continue;
		}
		
		while(1){
			system("clear");
			print_branch_information();
			printf("\n	-----------------------------------------\n");
			printf("\n		(input \"0\" back to menu)\n");
			printf("		 input #branch to edit reservation >> ");
			input1 = get_input();

			if(input1 == 0){
				printf("Back to menu..\n");
				close(fd);
				sleep(1);
				return;
			}

			if(input1 < 1 || input1 > 6){
				printf("Wrong input. Select (1~6)\n");
				sleep(1);
				continue;
			}
			if(br_isopen[input1-1] != 'o'){
				printf("It is closed. Try other room.\n");
				sleep(1);
				continue;
			}


			while(1){
				system("clear");
				print_room_information(input1);
				printf("\n	-----------------------------------------\n");
				printf("\n		(input \"0\" back to menu)\n");
				printf("		 input #room to edit reservation >> ");
				input2 = get_input();
				
				if(input2 == 0){
					printf("Back to menu..\n");
					close(fd);
					sleep(1);
					return;
				}		
				if(input2 < 1 || input2 > 5){
					printf("Wrong input. Select (1~5)\n");
					sleep(1);
					continue;
				}
				if(rm_isopen[input1-1][input2-1] != 'o'){
					printf("It is closed. Try other room.\n");
					sleep(1);
					continue;
				}

				editing_resv(idx, id, input1, input2);
				break;
			}
			break;
		}
		break;
	}
	close(fd);
	return;
}

void editing_resv(int idx, char* id, int brNum, int rmNum){
	int fd, input2, input3, input4, argc, chktime, cnt, chk, rrn;
	char* argv[10];
	char input1[7];
	char rsbuf[34];
	char tpbuf[34];
	char datenow[7];
	time_t t;
	struct tm lt;

	while(1){	
		t = time(NULL);
		localtime_r(&t, &lt);
		sprintf(datenow, "%02d%02d%02d", lt.tm_year+1900-2000, lt.tm_mon+1, lt.tm_mday);
		system("clear");
		print_reservation_info(brNum, rmNum);
		printf("	\n---------------------------------------------\n");
		printf("\n		(input \"0\" back to menu)\n");
		printf("		 input reservation date(YYMMDD) >> ");

		fgets(input1, sizeof(input1), stdin);
		if(strlen(input1) != 6){
			printf("Invalid reservation date. Format : YYMMDD\n");
			sleep(1);
			continue;
		}

		while(getchar()!='\n');

		printf("		 input reservation starting time >> ");

	 	input2 = get_input();

		if(input2 < 8 || input2 >= 22){
			printf("Invalid reservation time. Between 08:00 ~ 22:00\n");
			sleep(5);
			continue;
		}

		printf("		 input using time >> ");
		input3 = get_input();

		if(input3 < 1 || input3 > 14){
			printf("Invalid Using time. Between 08:00 ~ 22:00\n");
			sleep(1);
			continue;
		}
		if(input2+input3 > 22){
			printf("Invalid reservation time. Between 08:00 ~ 22:00\n");
			sleep(1);
			continue;
		}

		printf("		 input #person to use >> ");
		input4 = get_input();

		if(input4 < 1){
			printf("Invalid person number. User count should be more than 1.\n");
			sleep(1);
			continue;
		}

		RESV resv;
		strcpy(resv.ID, id);
		strcpy(resv.date, input1);
		resv.time = input2;
		resv.usetime = input3;
		resv.branch = brNum;
		resv.room = rmNum;
		resv.person = input4;

		if((fd = open(RESV_FILE, O_RDWR)) < 0){
			fprintf(stderr, "open error in reservation.txt\n");
			exit(1);
		}
		chktime = 1;
		while(read(fd, rsbuf, sizeof(rsbuf)) > 0){
			rsbuf[strlen(rsbuf)-1] = '\0';
			argc = split(rsbuf, " ", argv);

			if(!strcmp(resv.ID, argv[0]) && !strcmp(resv.date, argv[1]) && resv.time == atoi(argv[2]) &&
					resv.usetime == atoi(argv[3]) && resv.branch == atoi(argv[4]) &&
					resv.room == atoi(argv[5]) && resv.person == atoi(argv[6]))
				continue;


			
			if(strcmp(resv.date, argv[1]) == 0){
				if(resv.time+resv.usetime <= atoi(argv[2]) || atoi(argv[2])+atoi(argv[3]) <= resv.time){}
				else{
					chktime = 0;
					break;
				}
			}

		}

		if(chktime == 0){
			printf("Invalid reservation(Duplicate time).. Retry reservation\n");
			close(fd);
			sleep(1);
			continue;
		}
		else{
			sprintf(rsbuf, "%10s %6s %02d %02d %02d %02d %02d\n",
					resv.ID, resv.date, resv.time, resv.usetime, resv.branch, resv.room, resv.person);


			lseek(fd, 0, SEEK_SET);
			chk = 0;
			rrn = 0;
			while(read(fd, tpbuf, sizeof(tpbuf)) > 0){
				tpbuf[strlen(tpbuf)-1] = '\0';
				argc = split(tpbuf, " ", argv);

				if(strcmp(datenow, argv[1]) <= 0){
					if(!strcmp(id, argv[0])){
						chk++;
						if(chk == idx)
							break;
					}
				}
				rrn++;
			}
			lseek(fd, (rrn)*sizeof(tpbuf), SEEK_SET);
			write(fd, rsbuf, sizeof(rsbuf));

			printf("*** Successfully edited ***\n");
			printf("Back to menu..\n");
			close(fd);
			sleep(2);
			break;
		}

	}

	return;
}



void print_user_reservation_info(char* id){
	time_t t;
	struct tm lt;
	int fd, argc, idx, rrn;
	char* argv[10];
	char rsbuf[34];
	char datenow[7];
	char timenow[5];
	t = time(NULL);
	localtime_r(&t, &lt);
	sprintf(datenow, "%02d%02d%02d", lt.tm_year+1900-2000, lt.tm_mon+1, lt.tm_mday);

	if((fd = open(RESV_FILE, O_RDONLY)) < 0){
		fprintf(stderr, "open error on reservation.txt\n");
		exit(1);
	}

	printf("		Reservation info - [%s]\n\n", id);
	printf("Index #Branch #Room #Person  Date		Time	UsingTime\n");
	idx = 1;
	while(read(fd, rsbuf, sizeof(rsbuf)) > 0){
		rsbuf[strlen(rsbuf)-1] = '\0';
		argc = split(rsbuf, " ", argv);

		if(strcmp(datenow, argv[1]) <= 0){
			if(!strcmp(id, argv[0])){
				printf(" %4d %6d %4d %6d    %6s	%02d:00 ~ %02d:00 %9d\n",
						idx, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[1],
						atoi(argv[2]), atoi(argv[2])+atoi(argv[3]), atoi(argv[3]));
				idx++;
			}
		}

	}
	close(fd);
	printf("------------------------------------------------------------------\n\n");
	return;
}



void add_reservation(char* id, int brNum, int rmNum){
	int fd, input2, input3, input4, argc, chktime;
	char* argv[10];
	char input1[7];
	char rsbuf[34];
	char datenow[7];
	time_t t;
	struct tm lt;

	while(1){
		t = time(NULL);
		localtime_r(&t, &lt);
		sprintf(datenow, "%02d%02d%02d", lt.tm_year+1900-2000, lt.tm_mon+1, lt.tm_mday);
		system("clear");
		print_reservation_info(brNum, rmNum);
		printf("	\n-------------------------------------------\n");
		printf("\n		(input \"0\" back to menu)\n");
		printf("		 input reservation date(YYMMDD) >> ");

		fgets(input1, sizeof(input1), stdin);

		if(atoi(input1) == 0){
			printf("Back to menu..\n");
			sleep(1);
			return;
		}

		if(strlen(input1) != 6){
			printf("Invalid reservation date. Format : YYMMDD\n");
			sleep(1);
			continue;
		}

		if(strcmp(datenow, input1) == 0){
			printf("Invalid input : Same day reservation in not allowed.\n");
			sleep(1);
			continue;
		}

		if(strcmp(datenow, input1) > 0){
			printf("Today : %s\nYou cannot reserve past time(%s)\n", datenow, input1);
			sleep(2);
			continue;
		}

		while(getchar()!='\n');

		printf("		 input reservation starting time >> ");

		input2 = get_input();

		if(input2 < 8 || input2 >= 22){
			printf("Invalid reservation time. Between 08:00 ~ 22:00\n");
			sleep(5);
			continue;
		}

		printf("		 input using time >> ");
		input3 = get_input();

		if(input3 < 1 || input3 > 14){
			printf("Invalid Using time. Between 08:00 ~ 22:00\n");
			sleep(1);
			continue;
		}
		if(input2+input3 > 22){
			printf("Invalid reservation time. Between 08:00 ~ 22:00\n");
			sleep(1);
			continue;
		}

		printf("		 input #person to use >> ");
		input4 = get_input();

		if(input4 < 1){
			printf("Invalid person number. User count should be more than 1.\n");
			sleep(1);
			continue;
		}

		RESV resv;
		strcpy(resv.ID, id);
		strcpy(resv.date, input1);
		resv.time = input2;
		resv.usetime = input3;
		resv.branch = brNum;
		resv.room = rmNum;
		resv.person = input4;

		if((fd = open(RESV_FILE, O_RDWR)) < 0){
			fprintf(stderr, "open error in reservation.txt\n");
			exit(1);
		}
		chktime = 1;
		while(read(fd, rsbuf, sizeof(rsbuf)) > 0){
			rsbuf[strlen(rsbuf)-1] = '\0';
			argc = split(rsbuf, " ", argv);

			if(strcmp(resv.date, argv[1]) < 0){}
			else if(strcmp(resv.date, argv[1]) == 0 && resv.time < atoi(argv[2])){
				if(resv.time+resv.usetime <= atoi(argv[2]) || atoi(argv[2])+atoi(argv[3]) <= resv.time){}
				else{
					chktime = 0;
					break;
				}
			}
		}

		if(chktime == 0){
			printf("Invalid reservation(Duplicate time).. Retry reservation\n");
			close(fd);
			sleep(1);
			continue;
		}
		else{
			sprintf(rsbuf, "%10s %6s %02d %02d %02d %02d %02d\n",
					resv.ID, resv.date, resv.time, resv.usetime, resv.branch, resv.room, resv.person);
			write(fd, rsbuf, sizeof(rsbuf));
			printf("*** Successfully reserved ***\n");
			printf("Back to menu..\n");
			close(fd);
			sleep(2);
			break;
		}

	}

	return;
}

void print_reservation_info(int brNum, int rmNum){
	time_t t;
	struct tm lt;
	int fd, argc;
	char* argv[10];
	char rsbuf[34];
	char datenow[7];
	char timenow[5];
	t = time(NULL);
	localtime_r(&t, &lt);
	sprintf(datenow, "%02d%02d%02d", lt.tm_year+1900-2000, lt.tm_mon+1, lt.tm_mday);

	printf("	[%d] branch - [%d] room reservation list\n\n", brNum, rmNum);
	printf("	  date			 time\n");

	if((fd = open(RESV_FILE, O_RDONLY)) < 0){
		fprintf(stderr, "open error in reservation.txt\n");
		exit(1);
	}
	while(read(fd, rsbuf, sizeof(rsbuf)) > 0){
		rsbuf[strlen(rsbuf)-1] = '\0';
		argc = split(rsbuf, " ", argv);

		if(strcmp(datenow, argv[1]) < 0){
			if(brNum == atoi(argv[4]) && rmNum == atoi(argv[5])){
				printf("	%6s		%02d:00 ~ %02d:00\n", 
						argv[1], atoi(argv[2]), atoi(argv[2])+atoi(argv[3]));
			}
		}
		else if(strcmp(datenow, argv[1]) == 0){
			if(brNum == atoi(argv[4]) && rmNum == atoi(argv[5])){
				printf("%6s		%02d:00 ~ %02d:00\n", 
						argv[1], atoi(argv[2]), atoi(argv[2])+atoi(argv[3]));
			}
		}		
	}
	printf("\n---------------------------------------------------\n\n");
	close(fd);
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

int get_input(void){
	char buf[10];
	int input;

	fgets(buf, sizeof(buf), stdin);
	buf[strlen(buf)-1] = '\0';
	input = atoi(buf);

	return input;
}
