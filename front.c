#include <stdio.h>
#include <ctype.h>
#include <string.h>

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define EOF_TOKEN -1
#define IF_ 15
#define ELSE_ 16
#define LBRACE 17
#define RBRACE 18
#define WHILE_ 30
#define FOR_ 31
#define DO_ 32
#define DO_WHILE_ 33

int main() {
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in\n");
    else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF_TOKEN);
    }
    return 0;
}

int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case '=':
            addChar();
            nextToken = ASSIGN_OP;
            break;
        case '{':
            addChar();
            nextToken = LBRACE;
            break;
        case '}':
            addChar();
            nextToken = RBRACE;
            break;
        default:
            addChar();
            nextToken = EOF_TOKEN;
            break;
    }
    return nextToken;
}

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    } else {
        printf("Error - lexeme is too long\n");
    }
}

void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        charClass = EOF_TOKEN;
    }
}

void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            if (strcmp(lexeme, "if") == 0) {
                nextToken = IF_;
            } else if (strcmp(lexeme, "else") == 0) {
                nextToken = ELSE_;
            } else if (strcmp(lexeme, "while") == 0) {
                nextToken = WHILE_;
            } else if (strcmp(lexeme, "for") == 0) {
                nextToken = FOR_;
            } else if (strcmp(lexeme, "do") == 0) {
                nextToken = DO_;
                getNonBlank();
                if (charClass == LETTER) {
                    lexLen = 0;
                    addChar();
                    getChar();
                    while (charClass == LETTER || charClass == DIGIT) {
                        addChar();
                        getChar();
                    }
                    if (strcmp(lexeme, "while") == 0) {
                        nextToken = DO_WHILE_;
                    }
                }
            } else {
                nextToken = IDENT;
            }
            break;
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        case EOF_TOKEN:
            nextToken = EOF_TOKEN;
            strcpy(lexeme, "EOF");
            break;
    }
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}
