#include <stdio.h>
#include <string.h>
#include <time.h>

enum State {
    Start = 'S',
    Empty = '.',
    Split = '^',
    Beam = '|',
    Visisted = 'x'
};

struct Cell {
    enum State state;
    long count;
};

void processFile(FILE *f, struct Cell grid[1024][256], int *row, int *col) {
    char buffer[1024];
    *row = 0;

    while (fgets(buffer, sizeof(buffer), f)) {
        *col = 0;

        for (int i = 0; buffer[i] != '\0'; i++) {
            char c = buffer[i];

            switch (c) {
            case Start:
                grid[*row][*col].count = 1;
            case Empty:
            case Split:
            case Beam:
                grid[*row][*col].state = (enum State)c;
                grid[*row][*col].count = 0;
                (*col)++;
                break;
            default:
                break;
            }
        }

        (*row)++;
    }
}

void part1(struct Cell grid[1024][256], int row, int col) {
    long result = 0;
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            enum State state = grid[i][j].state;
            switch (state) {
            case Start:
                grid[i + 1][j].state = Beam;
                break;
            case Empty:
                break;
            case Split:
                if (grid[i - 1][j].state == Beam) {
                    if (grid[i][j - 1].state != Beam) {
                        grid[i][j - 1].state = Beam;
                        if (grid[i + 1][j - 1].state == Empty) {
                            grid[i + 1][j - 1].state = Beam;
                        }
                    }
                    if (grid[i][j + 1].state != Beam) {
                        grid[i][j + 1].state = Beam;
                    }
                    result++;
                }
                break;
            case Beam:
                if (grid[i + 1][j].state == Empty) {
                    grid[i + 1][j].state = Beam;
                }
                break;
            case Visisted:
            }
        }
    }
    printf("p1 result: %ld\n", result);
}

void part2(struct Cell grid[1024][256], int row, int col) {
    for (int j = 0; j < col; j++) {
        if (grid[0][j].state == Start) {
            grid[0][j].count = 1;
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            long count = grid[i][j].count;
            if (count == 0)
                continue;

            enum State st = grid[i][j].state;

            switch (st) {

            case Empty:
            case Beam:
            case Start:
                if (i + 1 < row) {
                    grid[i + 1][j].count += count;
                }
                break;
            case Split:
                if (i > 0 && grid[i - 1][j].state == Beam) {

                    if (j > 0) {
                        grid[i][j - 1].count += count;
                        grid[i + 1][j - 1].count = grid[i][j - 1].count;
                    }

                    if (j + 1 < col) {
                        grid[i][j + 1].count += count;
                    }
                }
                break;

            default:
                break;
            }
        }
    }

    long total = 0;
    for (size_t i = 0; i < col; i++) {
        total += grid[row - 1][i].count;
    }

    printf("p2 result: %ld\n", total);
}

int main() {
    FILE *f = fopen("input.txt", "r");
    struct Cell grid[512][256];
    int row = 0, col = 0;

    processFile(f, grid, &row, &col);

    struct Cell grid2[512][256];
    memcpy(grid2, grid, sizeof(grid));

    part1(grid, row, col);

    clock_t start = clock();
    part2(grid, row, col);
    clock_t end = clock();
    printf("Time to run: %f\n", (double)(end - start) / CLOCKS_PER_SEC);
}