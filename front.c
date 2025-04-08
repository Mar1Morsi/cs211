/* front.c - a lexical analyzer for evaluating basic arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global declarations */
/* Variable declarations */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);

/* Character classification */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token identifiers */
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

/******************************************************/
/* Main driver */
int main() {
    /* Opens the source file and start processing it */
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

/******************************************************/
/* lookup - a function to lookup operators and parentheses and return the token */
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

/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    } else {
        printf("Error - lexeme is too long\n");
    }
}

/******************************************************/
/* getChar - a function that retrieves the next character of input and determine its character class */
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

/******************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/******************************************************/
/* lex - the lexical analyzer function for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Parse identifiers and reserved words */
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

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Handle Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        /* End of file */
        case EOF_TOKEN:
            nextToken = EOF_TOKEN;
            strcpy(lexeme, "EOF");
            break;
    }

    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}
