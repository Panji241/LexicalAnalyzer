#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int yylex(FILE *filePointer, char *string);

int main(void) {
    FILE *yyin = NULL;
    char *yytext = NULL;
    
    if((yyin = fopen("text.txt", "r+")) == NULL) {
        perror("File not found!");
        exit(-1);
    }

    if((yytext = (char *) calloc(526, sizeof(char))) == NULL) {
        perror("Memory not allocated!");
        exit(-1);
    }

    int token;
    while((token = yylex(yyin, yytext)) != 0) {
        if(token == 1) {
            printf("Identifier: %s\n", yytext);
        }
        else if(token == 2) {
            printf("Number: %s\n", yytext);
        }
        else {
            printf("Token: %d\n", token);
            break;
        }
    }

    free(yytext);
    fclose(yyin);
    return 0;
}

int yylex(FILE *filePointer, char *string) {
    int c;
    int textPointer = 0;
    int state = 0;
    
    while(1) {
        c = fgetc(filePointer);
        switch(state) {
            case 0: {
                if(isalpha(c)) {
                    state = 1;
                    string[textPointer++] = c;
                }
                else if(isdigit(c)) {
                    state = 2;
                    string[textPointer++] = c;
                }
                else {
                    return 0;
                }
            }
            break;
            case 1: {
                if(isalpha(c) || isdigit(c)) {
                    string[textPointer++] = c;
                }
                else {
                    string[textPointer] = '\0';
                    ungetc(c, filePointer);
                    return 1;
                }
            }
            break;
            case 2: {
                if(isdigit(c)) {
                    string[textPointer++] = c;
                }
                else {
                    string[textPointer] = '\0';
                    ungetc(c, filePointer);
                    return 2;
                }
            }
            break;
            default: {
                return 0;
            }
        }
    }
    //return 0;
}