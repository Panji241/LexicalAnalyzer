#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef union {
    char *string;
    int i;
}SEMANTIC_INFO;

int yylex();

FILE* yyin = NULL;
char* yytext = NULL;
SEMANTIC_INFO yyval;

int main(void) {
    if ((fopen_s(&yyin, "text.txt", "r")) != NULL) {
		printf("Error: File not found!\n");
        return -1;
    }

    if ((yytext = (char*)calloc(128, sizeof(char))) == NULL) {
        printf("Error: Memory not allocated!\n");
        return -1;
    }

    int token;
    while ((token = yylex()) != 0) {
        if (token == 1) {
			//yyval.string = yytext;
			yyval.string = (char *) calloc(strlen(yytext) + 1, sizeof(char));
			strcpy_s(yyval.string, sizeof(yyval.string), yytext);
            printf("Identifier: %s\n", yyval.string);
        }
        else if (token == 2) {
			yyval.i = atoi(yytext);
            printf("Number: %d\n", yyval.i);
        }
        else {
            printf("Token: %d\n", token);
        }
    }

    free(yytext);
    fclose(yyin);
    return 0;
}

int yylex() {
    int c, textPointer = 0, state = 0;

    while(1) {
        //Fixed the issue with while returning -1 non stop
        c = fgetc(yyin);
        switch (state) {
            case 0: {
                if (isalpha(c)) {
                    state = 1;
                    yytext[textPointer++] = c;
                }
                else if (isdigit(c)) {
                    state = 2;
                    yytext[textPointer++] = c;
                }
                else if (c == EOF){
					return 0; // Checking for c is EOF so it will stop the loop instead of cotinuing to spam -1
                }
            }
            break;
            case 1: {
                if (isalpha(c)) {
                    yytext[textPointer++] = c;
                }
                else {
                    yytext[textPointer] = '\0';
					ungetc(c, yyin); // Used ungetc to return the character back from EOF
                    return 1;
                }
            }
            break;
            case 2: {
                if (isdigit(c)) {
                    yytext[textPointer++] = c;
                }
                else {
                    yytext[textPointer] = '\0';
                    ungetc(c, yyin); // Used ungetc to return the character back from EOF
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