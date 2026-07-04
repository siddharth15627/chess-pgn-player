//   U+2B1B        1110xxxx 10xxxxxx 10xxxxxx
//   0000 2B1B
//   0000 0010 1011 0001 1011 >> 11100010  10101100  10011011
//   E2 AC 9B

// U+2B1C         1110xxxx 10xxxxxx 10xxxxxx
// 0000 2B1C
// 0000 0010 1011 0001 1100 >> 11100010  10101100  10011100
// E2 AC 9C

#include <stdio.h>

#define SIZE 8

int main() {
    char wcell[] = "⬜";
    char bcell[] = "⬛";
    char *cell = NULL;

    for (int i = 0; i < SIZE; i++) {
        cell = (i%2 == 0) ? wcell : bcell;
        for (int j = 0; j < SIZE; j++) {
            printf("%s", cell);
            cell = (cell == wcell) ? bcell : wcell;
        }
        printf("\n");
    }

    return 0;
}