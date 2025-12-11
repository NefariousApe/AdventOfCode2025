
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long x;
    long y;
} Point;

int main() {
    FILE *f = fopen("input.txt", "r");
    size_t capacity = 1024;
    size_t count = 0;

    Point *pts = malloc(sizeof(Point) * capacity);
    if (!pts)
        return 1;

    long x, y;

    // Load all points
    while (fscanf(f, "%ld,%ld", &x, &y) == 2) {
        if (count == capacity) {
            capacity *= 2;
            pts = realloc(pts, sizeof(Point) * capacity);
            if (!pts)
                return 1;
        }
        pts[count++] = (Point){x, y};
    }
    fclose(f);

    uint64_t maxArea = 0;

    // Check all pairs of points
    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            long dx = pts[i].x - pts[j].x + 1;
            long dy = pts[i].y - pts[j].y + 1;

            if (dx < 0)
                dx = -dx;
            if (dy < 0)
                dy = -dy;

            uint64_t area = (uint64_t)dx * (uint64_t)dy;
            if (area > maxArea) {
                maxArea = area;
            }
        }
    }

    printf("Largest rectangle area = %lu\n", maxArea);
    free(pts);
}