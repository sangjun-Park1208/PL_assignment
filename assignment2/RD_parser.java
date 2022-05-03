import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class RD_parser {
    static final int LETTER = 0;
    static final int DIGIT = 1;
    static final int UNKNOWN = 99;
    static final int EOF = -1;

    static final int INT_LIT = 10;
    static final int ADD_OP = 21; // '+'
    static final int SUB_OP  = 22; // '-'
    static final int MULT_OP = 23; // '*'
    static final int DIV_OP = 24; // '/'
    static final int LEFT_PAREN = 25; // '('
    static final int RIGHT_PAREN = 26; // ')'
    static final int BUF_MAX = 1024;

    List<String> lexeme = new ArrayList<>();

    char nextChar = 0;
    int nextToken = 0;
    int charClass = 0;
    int lexLen = 0;
    int leftParenCnt = 0;
    int rightParenCnt = 0;
    int checkCharCnt = 0;
    String input;

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while(true){
            RD_parser rd_parser = new RD_parser();
            System.out.print(">> ");
            String temp = sc.nextLine();
            rd_parser.input = temp + "\n";
            rd_parser.getChar();
            if(rd_parser.nextChar == '\n')
                continue;
            rd_parser.lex();
            double result = rd_parser.expr();
            if(rd_parser.leftParenCnt != rd_parser.rightParenCnt){
                System.out.println("Syntax error!!");
                System.exit(1);
            }
            if(result - (int)result == 0){
                System.out.println((int)result);
            }
            else {
                System.out.println(result);
            }
        }

    }
    double expr(){
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
                left = rslt;
            }
            else if(token == SUB_OP){
                rslt = sub(left, right);
                left = rslt;
            }
        }
        return rslt;
    }

    double term(){
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
                left = rslt;
            }
            else if(token == DIV_OP){
                rslt = divd(left, right);
                left = rslt;
            }
        }
        return rslt;
    }

    double factor(){
        int ifUnary = 0;
        if(nextToken == SUB_OP){
            ifUnary = 1;
            lex();
        }
        if(nextToken == LEFT_PAREN){
            lex();
            double exprVal = expr();
            if(nextToken == RIGHT_PAREN){
                lex();
            }
            else{
                System.out.println("Syntax error!!");
                System.exit(1);
            }
            if(ifUnary == 1)
                return -exprVal;
            else
                return exprVal;
        }
        else if(nextToken == INT_LIT){
            if(ifUnary == 1)
                return -number();
            else
                return number();
        }
        else {
            System.out.println("Syntax error!!");
            System.exit(1);
        }
        return 0;
    }

    double number(){
        return digit();
    }

    double digit(){
        List<String> cur_lexeme = new ArrayList<>();
        cur_lexeme.addAll(lexeme);
        lex();
        return Float.parseFloat(String.join("", cur_lexeme));
    }

    void lex(){
        lexLen = 0;
        lexeme = new ArrayList<>();
        getNonBlank();
        switch (charClass){
            case LETTER:
                System.out.println("Syntax error!!");
                System.exit(1);
            case DIGIT:
                addChar();
                getChar();
                getNonBlank();
                while (charClass == DIGIT){
                    addChar();
                    getChar();
                    getNonBlank();
                }
                nextToken = INT_LIT;
                break;
            case UNKNOWN:
                lookup(nextChar);
                getChar();
                break;
            case EOF:
                nextToken = EOF;
                lexeme = new ArrayList<>();
                lexeme.add("EOF"+'\0');
                break;
            default:
                break;
        }
    }

    void addChar(){
        if(lexLen <= BUF_MAX){
            lexeme.add(lexLen++, nextChar+"");
            lexeme.add(lexLen, '\0'+"");
        }
        else{
            System.out.println("lexeme is too long");
            System.exit(1);
        }
    }

    void getChar(){
         nextChar = input.charAt(checkCharCnt++);
         if(nextChar != '\n'){
             if(Character.isLetter(nextChar))
                 charClass = LETTER;
             else if (Character.isDigit(nextChar))
                 charClass = DIGIT;
             else
                 charClass = UNKNOWN;
         }
         else
             charClass = EOF;
    }

    int lookup(char ch){
        switch (ch){
            case '(':	addChar();	nextToken = LEFT_PAREN;		leftParenCnt++;		break;
            case ')':	addChar();	nextToken = RIGHT_PAREN;	rightParenCnt++;	break;
            case '+':   addChar();	nextToken = ADD_OP;     	break;
            case '-':	addChar();	nextToken = SUB_OP;			break;
            case '*':	addChar();	nextToken = MULT_OP;		break;
            case '/':	addChar();  nextToken = DIV_OP;			break;
            case '\n':  addChar();  nextToken = EOF;            break;
            default:
                System.out.println("Syntax error!!");
                System.exit(1);
                break;
        }
        return nextToken;
    }

    void getNonBlank(){
        while((Character.isSpaceChar(nextChar) || nextChar == '\t') && nextChar != '\n')
            getChar();
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
}
