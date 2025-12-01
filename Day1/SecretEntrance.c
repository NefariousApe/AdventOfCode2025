#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int part1(FILE* fptr) {
    char buffer[256];

    int spot = 50;
    int password = 0;
    while (fgets(buffer, sizeof(buffer), fptr) != NULL) {
        char direction = buffer[0];
        int number = atoi(buffer + 1);

        if (direction == 'R') {
            spot += number;
        } else {
            spot -= number;
        }

        spot %= 100;
        if (spot == 0) {
            password += 1;
        }
    }
    return password;
}

int part2(FILE* fptr) {
    int spot = 50;
    int password = 0;
    char buffer[256];   

    while (fgets(buffer, sizeof(buffer), fptr) != NULL) {        
        char direction = buffer[0];
        int number = atoi(buffer + 1);

        if (direction == 'L') {
            password += number / 100;
            if (spot != 0 && (spot - (number % 100) <= 0)) {
                password++;
            }
            spot = (spot - number) % 100;
            if (spot < 0) {
                spot += 100;
            }
        } else {
            password += (spot + number) / 100;
            spot = (spot + number) % 100;
        }
    }
    return password;
}

int main() {
    FILE* fptr = fopen("./SecretEntranceInput.txt", "r");
    int password = part1(fptr);
    printf("%d\n", password);
    fptr = fopen("./SecretEntranceInput.txt", "r");
    clock_t begin = clock();
    password = part2(fptr);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time_spent);
    printf("%d\n", password);
    fclose(fptr);
    return EXIT_SUCCESS;
}



