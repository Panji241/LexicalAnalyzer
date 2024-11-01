#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

union semantic_info {
    char* string;
    int i;
};

int yylex(FILE *filePointer, char *string);

int main(void) {
    FILE* yyin = NULL;
    char* yytext = NULL;
    
    fopen_s(&yyin, "Text.txt", "r");
    if (yyin == NULL) {
		printf("Error: File not found!\n");
        exit(-1);
    }

    if ((yytext = (char*)calloc(128, sizeof(char))) == NULL) {
        printf("Error: Memory not allocated!\n");
        exit(-1);
    }

    int token;
    while ((token = yylex(yyin, yytext)) != 0) {
        if (token == 1) {
            printf("Identifier: %s\n", yytext);
        }
        else if (token == 2) {
            printf("Number: %s\n", yytext);
        }
        else {
            printf("Token: %d\n", token);
            break;
        }
    }

    free(yytext);
    if (yyin != NULL) fclose(yyin);
    return 0;
}

int yylex(FILE *filePointer, char *string) {
    int c, textPointer = 0, state = 0;

    while(1) {
        c = fgetc(filePointer);
        switch (state) {
            case 0: {
                if (isalpha(c)) {
                    state = 1;
                    string[textPointer++] = c;
                }
                else if (isdigit(c)) {
                    state = 2;
                    string[textPointer++] = c;
                }
                else if (c == EOF){
                    return 0;
                }
            }
            break;
            case 1: {
                if (isalpha(c)) {
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
                if (isdigit(c)) {
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
}