//   U+2B1B        1110xxxx 10xxxxxx 10xxxxxx
//   0000 2B1B
//   0000 0010 1011 0001 1011 >> 11100010  10101100  10011011
//   E2 AC 9B

// U+2B1C         1110xxxx 10xxxxxx 10xxxxxx
// 0000 2B1C
// 0000 0010 1011 0001 1100 >> 11100010  10101100  10011100
// E2 AC 9C

#include <stdio.h>

int main() {
    char color[25] = {"⬛⬜⬛⬜⬛⬜⬛⬜"}; 
    *(color+24) = '\0';   
    for (int i = 0; i < 8; i++) {
        printf("\n");
        if (i%2 == 0){
            for (int j = 0; j < 24; j = j+3) {
                printf("%c%c%c", *(color + j), *(color + j + 1), *(color + j + 2));
            }
        }
        else {
            for (int j = 3; j <= 21; j = j+3) {
                printf("%c%c%c", *(color + j), *(color + j + 1), *(color + j + 2));
                if (j == 21) {
                    printf("⬛");
                }
            }
        }
    }
    return 0;
}