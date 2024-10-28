#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    char c = 'd';
    
    if(isalpha(c)) {
        printf("Character is a letter\n");
    }
    else {
        printf("Character is not a letter\n");
    }

    return 0;
}