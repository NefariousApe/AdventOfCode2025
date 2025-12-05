#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int part1(FILE *fptr) {
    char buffer[256];
    int sum = 0;
    while(fgets(buffer, sizeof buffer, fptr) != NULL) {
        int i = 0;
        char digits[2];
        while(buffer[i] != '\n' && buffer[i] != '\0') {      
            if (buffer[i] > digits[0] && buffer[i+1] != '\n' && buffer[i+1] != '\0') {
                digits[0] = buffer[i];
                digits[1] = 0;               
            } else if (buffer[i] > digits[1]) {
                digits[1] = buffer[i];
            }
            i += 1;
        }
        sum += atoi(digits);
        digits[0] = 0, digits[1] = 0;
        
    }
    return sum;
}

long part2(FILE *fptr) {
    char buffer[256];
    
    long sum = 0;
    while(fgets(buffer, sizeof buffer, fptr) != NULL) {
        int i = 0;
        char digits[12];
        long length = strlen(buffer);
        while(buffer[i] != '\n' && buffer[i] != '\0') {
            for (size_t j = 0; j < 12; j++) {               
                if (buffer[i] > digits[j] && (length - i - 1) > (12 - j - 1)){
                    digits[j] = buffer[i];
                    for(size_t k = j+1; k < 12; k++) {
                        digits[k] = 0;
                    }
                    break;
                }
            }
            i += 1;
        }
        sum += atol(digits);
        for (size_t j = 0; j < 12; j++) {
            digits[j] = 0;
        }        
    }
    return sum;
}


int main() {

    FILE* fptr = fopen("./input.txt", "r");
    int result = part1(fptr);
    printf("Part 1: %d\n", result);
    fptr = fopen("./input.txt", "r");
    long result2 = part2(fptr);
    printf("Part d: %ld\n", result2);

    return EXIT_SUCCESS;
}