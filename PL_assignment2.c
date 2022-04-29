#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

#define INT_LIT 10
#define CHAR_LIT 11
#define ADD_OP 21 // '+'
#define SUB_OP 22 // '-'
#define MULT_OP 23 // '*'
#define DIV_OP 24 // '/'
#define LEFT_PAREN 25 // '('
#define RIGHT_PAREN 26 // ')'
#define BUF_MAX 1024

/*
<expr> → <term> {+ <term> | - <term>}
<term> → <factor> {* <factor> | / <factor>}
<factor> → [ - ] ( <number> | (<expr>) )
<number> → <digit> {<digit>}
<digit> → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
*/

void expr();
void term();
void factor();
void number();
void digit();
void lex();
void addChar();
void getChar();
int lookup(char ch);
void getNonBlank();
double add(double left, double right);
double sub(double left, double right);
double mult(double left, double right);
double divd(double left, double right);

struct token* tokenList[1000];
char lexeme[BUF_MAX];
char nextChar;
int nextToken;
int charClass;
int lexLen;
int leftParenCnt;
int rightParenCnt;

struct token{
	double value;
	int tokenType;
};

int main(){
    while(1){
		printf(">> ");
		getChar(); 
		lex();
		expr();
		if(leftParenCnt != rightParenCnt){
			printf("Syntax error!!\n");
			exit(1);
		}
	}
	exit(0);
}

// <expr> -> <term> {+ <term> | - <term>}
double expr(void){
    term();
    while(nextToken == ADD_OP || nextToken == SUB_OP){
        if(nextToken == ADD_OP){
			
		}
		else if(nextToken == SUB_OP){

		}
		lex();
        term();
    }
    return;
}

// <term> -> <factor> {* <factor> | / <factor>}
double term(void){
    factor();
    while(nextToken == MULT_OP || nextToken == DIV_OP){
        lex();
        factor();
    }
    return;
}

// <factor> -> [-] ( <number> | (<expr>) )
double factor(void){
    if(nextToken == SUB_OP)
        lex();
    
    if(nextToken == LEFT_PAREN){
        lex();
        expr();
        if(nextToken == RIGHT_PAREN){
            lex();
		}
        else{
            printf("Syntax error!!\n");
            exit(1);
        }
    }
    else if(nextToken == INT_LIT){
        number();
    }
    else{
        printf("Syntax error!!\n");
        exit(1);
    }
    return;
}

// <number> -> <digit> {<digit>}
double number(){
    return digit();
//    while(nextToken == INT_LIT)
//          digit();
//    return;
}

// <digit> → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
double digit(void){
    lex();
	int i=0;
	int check_ifDigit = 0;
	do{
		if(!isdigit(lexeme[i++])){
			check_ifDigit = 1;
			break;
		}
	}while(lexeme[i] != '\n');

	if(check_ifDigit == 0){ // if lexeme is digit
		return atof(lexeme);
	}
	else{ // if lexeme is not a digit

	}

    return;
}

void lex(void){
    lexLen = 0;
    getNonBlank();
    switch (charClass){
		case LETTER:
			printf("Syntax error!!\n");
			exit(1);
			break;

        case DIGIT:
            addChar();
            getChar();
            while(charClass == DIGIT){
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
            
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = '\0';
            break;

        default:
            break;
    }
	printf("lexeme : %s\n", lexeme);
    return;
}

void addChar(){
    if(lexLen <= BUF_MAX){
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    }
    else{
        printf("lexeme is too long\n");
        exit(1);
    }
}

void getChar(){
    if((nextChar = getc(stdin)) != '\n'){
        if(isalpha(nextChar))
            charClass = LETTER;
		else if(isdigit(nextChar))
			charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else
        charClass = EOF;
	return;
}

void getNonBlank(){
	while(isspace(nextChar) && nextChar != '\n'){
        getChar();
    }
    return;
}

int lookup(char ch){
    switch (ch){
	    case '(':	addChar();	nextToken = LEFT_PAREN;		leftParenCnt++;		break;
	    case ')':	addChar();	nextToken = RIGHT_PAREN;	rightParenCnt++;	break;
	    case '+':   addChar();	nextToken = ADD_OP;     	break;
	    case '-':	addChar();	nextToken = SUB_OP;			break;
	    case '*':	addChar();	nextToken = MULT_OP;		break;
		case '/':	addChar();  nextToken = DIV_OP;			break;
	    default:	addChar();	nextToken = EOF;        	break;
    }
    return nextToken;
}


double add(double left, double right){
	return left + right;
}

double sub(double left, double right){
	return left - right;
}
double mult(double left, double right){
	return left * right;
}
double divd(double left, double right){
	return left / right;
}
