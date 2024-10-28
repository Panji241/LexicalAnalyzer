#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *yytext = NULL;

int yylex(FILE *filePointer);

int main(void) {
    FILE *yyin = fopen("text.txt", "r+");
    if(yyin == NULL) {
        perror("File not found!");
        exit(-1);
    }

    yytext = (char *) calloc(526, sizeof(char));
    if(yytext == NULL) {
        perror("Memory not allocated!");
        exit(-1);
    }

    int token;
    while((token = yylex(yyin)) != 0) {
        if(token == 1) {
            printf("Identifier: %s\n", yytext);
        }
        else if(token == 2) {
            printf("Number: %s\n", yytext);
        }
        else {
            printf("Token: %d\n", token);
        }
    }

    free(yytext);
    fclose(yyin);
    return 0;
}

int yylex(FILE *filePointer) {
    char c;
    int textPointer = 0;
    int state = 0;
    while((c = fgetc(filePointer)) != EOF) {
        switch(state) {
            case 0: {
                if(isalpha(c)) {
                    state = 1;
                    yytext[textPointer++] = c;
                }
                else if(isdigit(c)) {
                    state = 2;
                    yytext[textPointer++] = c;
                }
                break;
            }
            case 1: {
                if(isalpha(c) || isdigit(c)) {
                    yytext[textPointer++] = c;
                }
                else {
                    yytext[textPointer] = '\0';
                    //ungetc(c, filePointer);
                    return 1;
                }
                break;
            }
            case 2: {
                if(isdigit(c)) {
                    yytext[textPointer++] = c;
                }
                else {
                    yytext[textPointer] = '\0';
                    ungetc(c, filePointer);
                    return 2;
                }
                break;
            }
            default: {
                return 0;
            }
        }
    }
}