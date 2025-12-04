#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rowCount = 0;

int countNeighbors(char** grid, int row, int col) {
    int neighbors = 0;                
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {            
            if (i == 0 && j == 0 || i + row < 0 || i + row >= rowCount || j + col < 0 || j + col >= strlen(grid[row])) {
                continue;
            }
            if (grid[row+i][col+j] == '@') {
                neighbors += 1;
            }
        }
    }    
    return neighbors;    
}

int part1(char** grid) {
    char buffer[256];
    int sum = 0;

    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < strlen(grid[row]); col++) {
            if (grid[row][col] != '@') {
                continue;
            }
            int neighbors = countNeighbors(grid, row, col);
            if (neighbors < 4) {
                sum += 1;
            }
        }
    }

    return sum;
}

int part2(char** grid) {
    int sum = 0;
    int row = 0;
    int col = 0;
    while (row < rowCount) {
        int rowLen = strlen(grid[row]);
        while (col < rowLen) {
            if (grid[row][col] != '@') {
                col++;
                continue;
            }            
            int neighbors = countNeighbors(grid, row, col);            
            if (neighbors < 4) {                              
                grid[row][col] = '.';
                if (row > 0) {row--;}
                if (col > 0) {col--; }         
                sum ++;
                continue;
            }           
            col++;
        }
        row++;
        col=0;
    }
    return sum;        
}
        
    


char** processFile(FILE* f) {
    int capacity = 8;
    char **grid = malloc(capacity * sizeof(char*));

    char buffer[256];

    while (fgets(buffer, 256, f)) {
        
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (rowCount == capacity) {
            capacity *= 2;
            grid = realloc(grid, capacity * sizeof(char*));
        }

        grid[rowCount] = malloc(strlen(buffer) + 1);
        strcpy(grid[rowCount], buffer);

        rowCount++;
    }

    fclose(f);

    

    return grid;
}



int main() {
    FILE* f = fopen("./input.txt", "r");
    char** file = processFile(f);
    
    int p1 = part1(file);
    printf("%d\n", p1);

    int p2 = part2(file);
    printf("%d\n", p2);
    
}