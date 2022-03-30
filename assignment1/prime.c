#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

void sorting(int* arr, int size);
int rm_duplicate(int* arr, int* uniqueArr, int size);
int get_GCD(int a, int b);
void count_Prime(int min, int max);

int* PrimeTable;
int main(){
	struct timeval startTime, endTime;
	double diffTime;

	int inputNum;
	int Count=0;
	char ch;
	int InRange = 0;

	while(1){
		printf(">> Input the number of numbers to process: ");
		scanf("%d", &inputNum);
		
		if(inputNum < 2 || inputNum > 30){ // check input number range : 2~30
			while((ch = getchar()) != '\n'){}
			printf("Input count range should be  \"1 < [INPUT] < 31\"\n");
			continue;
		}
		
		int* numList = (int*)malloc(sizeof(int)*inputNum);
		int* uniqueArr = (int*)malloc(sizeof(int)*inputNum); // for removing duplicated #
		printf(">> Input the numbers to be processed:\n");
		
		for(int i=0; i<inputNum; i++){
			Count += scanf("%d", &numList[i]);
		}

		ch = getchar();
		if(ch == ' '){ // if more input than inputNum
			printf("Input count Excess!\n");
			Count = 0;
			while((ch = getchar()) != '\n'){}
			continue;
		}

		if(Count != inputNum){ // if input is not a integer
			while((ch = getchar()) != '\n'){}
			Count = 0;
			fprintf(stderr, "Input error\n");
			continue;
		}

		for(int i=0; i<inputNum; i++){
			if(numList[i] > 100000 || numList[i] < 1){
				InRange = 1;
				break;
			}	
		}

		if(InRange){ // Range should be 1 ~ 100,000
			printf("Range : 1 ~ 100,000\n");
			InRange = 0;
			Count = 0;
			for(int i=0; i<inputNum; i++)
				numList[i] = 0;
			continue;
		}
		
		gettimeofday(&startTime, NULL);
		sorting(numList, inputNum);
		int uniqArr_size = rm_duplicate(numList, uniqueArr, inputNum);

		/*** Calculating GCD ***/
		int GCD = uniqueArr[0];
		for(int i=1; i<uniqArr_size-1; i++){
			GCD = get_GCD(GCD, uniqueArr[i]);
		}		
		printf("GCD of input number is %d\n\n", GCD);
		/***********************/

		
		/*** Counting Prime Number ***/
		PrimeTable = (int*)malloc(sizeof(int)*uniqueArr[uniqArr_size]);
		for(int i=0; i<uniqArr_size-1; i++)
			count_Prime(uniqueArr[i], uniqueArr[i+1]);
		
		free(PrimeTable);
		/*****************************/


		/*** Checking Execution time ***/
		gettimeofday(&endTime, NULL);
		diffTime = (endTime.tv_sec - startTime.tv_sec) + ((endTime.tv_usec - startTime.tv_usec) / 1000000.0);
		printf("Total execution time using C is %f seconds!\n", diffTime);
		/*******************************/

		break;
	}
	exit(1);
}



void sorting(int* arr, int size){
	int tmp;
	for(int i=size-1; i>0; i--){
		for(int j=0; j<i; j++){
			if(arr[j] > arr[j+1]){
				tmp = arr[j+1];
				arr[j+1] = arr[j];
				arr[j] = tmp;
			}
		}
	}
	return;
}

int rm_duplicate(int* arr, int* uniqueArr, int size){
	int k=0, t=0;
	for(int i=0; i<size; i++){
		for(int j=i+1; j<size; j++){
			if(arr[i] == arr[j]){
				k=1;
				break;
			}
		}
		if(k==0){
			uniqueArr[t++] = arr[i];
			k=0;
		}
		k=0;
	}
	return t;
}

int get_GCD(int a, int b){ // a < b
	int n;
	while(a!=0){
		n = b%a;
		b = a;
		a = n;
	}
	return b;
}



void count_Prime(int min, int max){
	int cnt = 0;

	for(int i=2; i<=max; i++){
		PrimeTable[i] = 1;
	}

	for(int i=2; i<=max; i++){
		if(PrimeTable[i] == 0)
			continue;
		for(int j=2*i; j<=max; j+=i)
			PrimeTable[j] = 0;
	}

	for(int i=min; i<=max; i++){
		if(PrimeTable[i] != 0)
			cnt++;
	}

	printf("Number of prime numbers between %d, %d: %d\n", min, max, cnt);
	return;
}

