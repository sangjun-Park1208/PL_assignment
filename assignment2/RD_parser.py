import sys

EOF = -1
LETTER = 0
DIGIT = 1
UNKNOWN = 99

INT_LIT = 10
ADD_OP = 21 # '+'
SUB_OP = 22 # '-'
MULT_OP = 23 # '*'
DIV_OP = 24 # '/'
LEFT_PAREN = 25 # '('
RIGHT_PAREN = 26 # ')'

BUF_MAX = 1024

lexeme = [0 for i in range(BUF_MAX)]
cur_lexeme = [0 for i in range(BUF_MAX)]
nextChar = 0
nextToken = token = 0
charClass = 0
lexLen = 0
leftParenCnt = rightParenCnt = 0
checkCharCnt = 0

def expr():
    global nextToken
    left = term()
    rslt = left
    while nextToken == ADD_OP  or nextToken == SUB_OP:
        token = nextToken
        lex()
        right = term()
        if token == ADD_OP:
            rslt = add(left, right)
            left = rslt
        elif token == SUB_OP:
            rslt = sub(left, right)
            left = rslt
    return rslt    
    
def term():
    global nextToken
    left  = factor()
    rslt = left
    while nextToken == MULT_OP or nextToken == DIV_OP:
        token = nextToken
        lex()
        right = factor()
        if token == MULT_OP:
            rslt = mult(left, right)
            left = rslt
        elif token == DIV_OP:
            rslt = divd(left, right)
            left = rslt
    return rslt

def factor():
    global nextToken
    ifUnary = 0
    if nextToken == SUB_OP:
        ifUnary = 1
        lex()
        
    if nextToken == LEFT_PAREN:
        lex()
        exprVal = expr()
        if nextToken == RIGHT_PAREN:
            lex()
        else:
            print("Syntax error!!")
            exit()
        
        if ifUnary == 1:
            return -exprVal
        else:
            return exprVal   
    elif nextToken == INT_LIT:
        if ifUnary == 1:
            return -number()
        else:
            return number()
    else:
        print("Syntax error!!")
        exit()
        
def number():
    return digit()

def digit():
    global cur_lexeme, lexeme
    cur_lexeme.clear()
    i=0
    while lexeme[i] != '\0':
        cur_lexeme.append(lexeme[i])
        i += 1

    lex()
    return float("".join(cur_lexeme))

def lex():
    global lexLen, nextToken, lexeme, charClass, nextChar
    lexLen = 0
    getNonBlank()
    if charClass == LETTER:
        print("Syntax error!!")
        exit()
    elif charClass == DIGIT:
        addChar()
        getChar()
        getNonBlank()
        while charClass == DIGIT:
            addChar()
            getChar()
            getNonBlank()
        nextToken = INT_LIT
    elif charClass == UNKNOWN:
        lookup(nextChar)
        getChar()
    elif charClass == EOF:
        nextToken = EOF
        lexeme = 'EOF'
    
def addChar():
    global lexLen
    if lexLen <= BUF_MAX: 
        lexeme[lexLen] = nextChar
        lexLen += 1
        lexeme[lexLen] = '\0'
        
    else:
        print("lexeme is too long");
        exit()
    

def getChar():
    global input
    global checkCharCnt, charClass, nextChar
    nextChar = input[checkCharCnt]
    checkCharCnt += 1
    
    if nextChar != '\n':
        if nextChar.isalpha():
            charClass = LETTER
        elif nextChar.isdigit():
            charClass = DIGIT
        else:
            charClass = UNKNOWN
    else:
        
        charClass = EOF
    
def getNonBlank():
    global nextChar
    while (nextChar == ' ' or nextChar == '\t' ) and nextChar != '\n':
        getChar()
    
def lookup(ch):
    global nextToken, leftParenCnt, rightParenCnt
    if ch == '(':
        addChar();  nextToken = LEFT_PAREN; leftParenCnt += 1;
    elif ch == ')':
        addChar();  nextToken = RIGHT_PAREN; rightParenCnt += 1;
    elif ch == '+':
        addChar();  nextToken = ADD_OP;
    elif ch == '-':
        addChar();  nextToken = SUB_OP;
    elif ch == '*':
        addChar();  nextToken = MULT_OP;
    elif ch == '/':
        addChar();  nextToken = DIV_OP;
    else:
        addChar();  nextToken = EOF;

    return nextToken

def add(left, right):
    return left + right

def sub(left, right):
    return left - right

def mult(left, right):
    return left * right

def divd(left, right):
    return left / right
    
    
def main():
    global leftParenCnt, rightParenCnt, input, lexeme, cur_lexeme, nextChar, nextToken, token, charClass, lexLen, checkCharCnt

    while True:
        lexeme = [0 for i in range(BUF_MAX)]
        cur_lexeme = [0 for i in range(BUF_MAX)]
        nextChar = 0
        nextToken = token = 0
        charClass = 0
        lexLen = 0
        leftParenCnt = rightParenCnt = 0
        checkCharCnt = 0
        print('>>', end=' ', flush=True)
        input = list(sys.stdin.readline())
        if(input[0] == '\n'):
            continue
        getChar()
        lex()
        result = expr()
        if leftParenCnt != rightParenCnt:
            print("Syntax error!!");
            exit()
        if(result - int(result) == 0):
            print(int(result))
        else:
            print(result)    
        

if __name__ == '__main__':
    main()
    