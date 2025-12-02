

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    long start;
    long end;
} Range;

long part1(Range* range, int count) {
    long sum = 0;
    for (int i = 0; i < count; i++) {
        for (long j = range[i].start; j <= range[i].end; j++) {
            int nDigits = 0;
            long temp = j;
            while (temp > 0) {
                nDigits++;
                temp /= 10;
            }
            
            if (nDigits % 2 != 0) {
                continue;
            }
            int halfDigits = nDigits / 2;
            long divisor = 1;
            for (int k = 0; k < halfDigits; k++) {
                divisor *= 10;
            }
            
            long firstHalf = j / divisor;
            long secondHalf = j % divisor;
            
            if (firstHalf == secondHalf) {
                sum += j;
            }
        }
    }
    return sum;
}

long part2(Range* range, int count) {
    long sum = 0;
    for (int i = 0; i < count; i++) {        
        for (long j = range[i].start; j <= range[i].end; j++) {
            int nDigits = 0;
            long temp = j;
            while (temp > 0) {
                nDigits++;
                temp /= 10;
            }
            
            for (int seqLen = 1; seqLen <= nDigits / 2; seqLen++) {
                int numRepeats = nDigits / seqLen;
                if (nDigits % seqLen != 0 || numRepeats < 2) {
                    continue;
                }
                
                long divisorForExtraction = 1;
                for (int k = 0; k < (nDigits - seqLen); k++) {
                    divisorForExtraction *= 10;
                }
                long sequence = j / divisorForExtraction;
                
                long reconstructed = 0;
                long divisorForSeq = 1;
                for (int k = 0; k < seqLen; k++) {
                    divisorForSeq *= 10;
                }
                
                long multiplier = 1;
                for (int rep = 0; rep < numRepeats; rep++) {
                    reconstructed += sequence * multiplier;
                    multiplier *= divisorForSeq;
                }                
                if (reconstructed == j) {
                    sum += j;
                    break;
                }
            }
        
        }
    }
    return sum;
}


int parseFile(FILE* fptr, Range* ranges) {
    char buffer[4096];
    
    fgets(buffer, sizeof buffer, fptr);
    fclose(fptr);

    
    int count = 0;

    char* token = strtok(buffer, ",");
    while (token != NULL) {
        long start, end;
        if(sscanf(token, "%ld-%ld", &start, &end) == 2) {
            ranges[count].start = start;
            ranges[count].end = end;
            count++;
        }

        token = strtok(NULL, ",");
    }
    return count;
    
}
int main() {
    FILE* fptr = fopen("./input.txt", "r");
    Range ranges[1024];
    
    int count = parseFile(fptr, ranges);
    
    clock_t begin = clock();
    long result = part1(ranges, count);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent part1: %f\n", time_spent);
    begin = clock();
    result = part2(ranges, count);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent part2: %f\n", time_spent);
    printf("%ld\n", result);

    return EXIT_SUCCESS;
}