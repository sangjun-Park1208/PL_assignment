#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

void sorting(int* arr, int size);
int get_GCD(int* arr);
void print_Prime(int min, int max);
int rm_duplicate(int* arr, int* uniqueArr, int size);

int main(){
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
		int* uniqueArr = (int*)malloc(sizeof(int)*inputNum);
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
		
		sorting(numList, inputNum);
		int uniqArr_size = rm_duplicate(numList, uniqueArr, inputNum);
		for(int i=0; i<inputNum; i++){
			printf("%d ", numList[i]);
		}
		printf("\n");

		for(int i=0; i<uniqArr_size; i++){
			printf("%d ", uniqueArr[i]);
		}
		printf("\n");

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

int get_GCD(int* arr){
	
}



void print_Prime(int min, int max){

}

