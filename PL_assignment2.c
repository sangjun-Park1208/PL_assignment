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

double expr();
double term();
double factor();
double number();
double digit();
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
char cur_lexeme[BUF_MAX];
char nextChar;
int nextToken;
int token;
int charClass;
int lexLen;
int leftParenCnt;
int rightParenCnt;
int isFirst = 1;

int main(){
    while(1){
		printf(">> ");
		isFirst = 1;
		double result = expr();
		printf("left : %d, right : %d\n", leftParenCnt, rightParenCnt);
		if(leftParenCnt != rightParenCnt){
			printf("Syntax error!!(in main)\n");
			exit(1);
		}
		printf("%f\n", result);
		while(getchar() != '\n')
			continue;
	}
	exit(0);
}

// <expr> -> <term> {+ <term> | - <term>}
double expr(void){
//	printf("Enter <expr>\n");
	double left = term();
	double rslt = left;
	double right;
	int token;
    while(nextToken == ADD_OP || nextToken == SUB_OP){
		token = nextToken;
		lex();
		right = term();
		if(token == ADD_OP){
			rslt = add(left, right);
			printf("rslt : %f\n", rslt);
			left = rslt;
		}
		else if(token == SUB_OP){
			rslt = sub(left, right);
			printf("rslt : %f\n", rslt);
			left = rslt;
		}
    }
//	printf("Exit <expr>\n");
    return rslt;
}

// <term> -> <factor> {* <factor> | / <factor>}
double term(void){
//	printf("Enter <term>\n");

	double left = factor();
	double rslt = left;
	double right;
	int token;
    while(nextToken == MULT_OP || nextToken == DIV_OP){
		token = nextToken;
		lex();
		right = factor();
		if(token == MULT_OP){
			rslt = mult(left, right);
			printf("rslt : %f\n", rslt);
			left = rslt;
		}
		else if(token == DIV_OP){
			rslt = divd(left, right);
			printf("rslt : %f\n", rslt);
			left = rslt;
		}
    }
//	printf("Exit <term>\n");
    return rslt;
}

// <factor> -> [-] ( <number> | (<expr>) )
double factor(void){
//	printf("Enter <factor>\n");
    if(isFirst == 1){
		isFirst = 0;
		getChar();
		lex();
		if(nextToken != LEFT_PAREN){
			return number();
		}
		else{
			double exprVal = expr();
			if(nextToken == RIGHT_PAREN){
				lex();
				return exprVal;
			}
			else{
				printf("Syntax error!!(in factor -> left paren1)\n");
				exit(1);
			}
		}
	}
//	if(nextToken == SUB_OP)
//	lex();
    
    if(nextToken == LEFT_PAREN){
        lex();
        double exprVal = expr();
        if(nextToken == RIGHT_PAREN){
            lex();
		}
        else{
            printf("Syntax error!!(in factor -> left paren2)\n");
            exit(1);
        }
		return exprVal;
    }
    else if(nextToken == INT_LIT){
        return number();
    }
    else{
        printf("Syntax error!!(in factor)\n");
        exit(1);
    }
}

// <number> -> <digit> {<digit>}
double number(){
    return digit();
}

// <digit> → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
double digit(void){
    lex();
	return atof(cur_lexeme);
}

void lex(void){
    lexLen = 0;
    getNonBlank();
    switch (charClass){
		case LETTER:
			printf("Syntax error!!(int lex())\n");
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
			memset(cur_lexeme, '\0', BUF_MAX);
			strcpy(cur_lexeme, lexeme);
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
