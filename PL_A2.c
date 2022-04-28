#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUF_MAX 1024
#define DIGIT_MAX 20

struct token* token_ptr[BUF_MAX];
int tknCnt;

enum TOKEN_TYPE{NUMBER, ADD, SUB, MULT, DIV, LPAREN, RPAREN};
struct token {
	double value;
	enum TOKEN_TYPE;
};

void add_token(double value, enum TOKEN_TYPE type);
void tokenizer(char* input);
double add_op(double left, double right);
double sub_op(double left, double right);
double mult_op(double left, double right);
double div_op(double left, double right);
double unary_minus(double left, double right);

int main(){
	char input[BUF_MAX];
	while(1){
		printf(">> ");
		fgets(input, sizeof(input), stdin);
		input[strlen(input)-1] = '\0';
		tokenizer(input);


	}
}

void add_token(double value, enum TOKEN_TYPE type){
	struct token* ptr = (struct token*)malloc(sizeof(struct token));
	ptr->value = value;
	ptr->type = type;
	arr[tknCnt++] = ptr;
	return;
}

void tokenizer(char* input){
	char buf[DIGIT_MAX] = {'\0', };
	int j;
	for(int i=0; i<strlen(input); i++){
		switch(input[i]){
			case '(': 
				add_token('(', LPAREN); 
				break;
			case ')':
				add_token(')', RPAREN);
				break;
			case '+':
				add_token('+', ADD);
				break;
			case '-':
				add_token('-', SUB);
				break;
			case '*':
				add_token('*', MULT);
				break;
			case '/':
				add_token('-', DIV);
				break;
			case '.':	case '0':	case '1':	case '2':	case '3':	case '4':	
			case '5':	case '6':	case '7':	case '8':	case '9':
				j=0;
				while(isdigit(input[i]) || input[i] == '.')
					buf[j++] = input[i++];
				buf[j] = '\0';
				i--;
				add_token(atof(buf), NUMBER);
				break;
			default:
				break;

		}
	}
}

double add_op(double left, double right){
	return left + right;
}

double sub_op(double left, double right){
	return left - right;
}

double mult_op(double left, double right){
	return left * right;
}

double div_op(double left, double right){
	return left / right;
}

double unary_minus(double value){
	return -value;
}

//	<expr> -> <term> {+ <term> | - <term>}
void expr(double* rslt){
	if(token_ptr->type)
}

















