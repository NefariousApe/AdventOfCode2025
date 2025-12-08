#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_POINTS 1000

struct Point {
    int x, y, z;
};

struct Edge {
    int i, j;
    float dist;
};

struct Point points[NUM_POINTS];
struct Edge *edges;

int parent[NUM_POINTS];
int size_arr[NUM_POINTS];

// union find

int find(int x) {
    if (parent[x] == x)
        return x;
    parent[x] = find(parent[x]);
    return parent[x];
}

int unite(int a, int b) {
    int ra = find(a);
    int rb = find(b);

    if (ra == rb)
        return 0; // already same component (cycle)

    // union by size
    if (size_arr[ra] < size_arr[rb]) {
        int tmp = ra;
        ra = rb;
        rb = tmp;
    }

    parent[rb] = ra;
    size_arr[ra] += size_arr[rb];
    return 1; // successful merge
}

float distance(struct Point p1, struct Point p2) {
    float dx = (float)p1.x - (float)p2.x;
    float dy = (float)p1.y - (float)p2.y;
    float dz = (float)p1.z - (float)p2.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

int edge_cmp(const void *a, const void *b) {
    float d1 = ((const struct Edge *)a)->dist;
    float d2 = ((const struct Edge *)b)->dist;
    if (d1 < d2)
        return -1;
    if (d1 > d2)
        return 1;
    return 0;
}

// sort int descending
int cmp_int(const void *a, const void *b) {
    return (*(const int *)b - *(const int *)a);
}

int main(void) {

    FILE *f = fopen("input.txt", "r");
    if (!f) {
        printf("Missing input file.\n");
        return 1;
    }

    for (int i = 0; i < NUM_POINTS; i++) {
        if (fscanf(f, "%d,%d,%d", &points[i].x, &points[i].y, &points[i].z) !=
            3) {
            printf("ERROR: input has fewer than 1000 valid lines.\n");
            return 1;
        }
    }
    fclose(f);

    // Create edges
    int total_edges = NUM_POINTS * (NUM_POINTS - 1) / 2;
    edges = malloc(sizeof(struct Edge) * total_edges);

    // Build all edges (i < j)
    int idx = 0;
    for (int i = 0; i < NUM_POINTS; i++) {
        for (int j = i + 1; j < NUM_POINTS; j++) {
            edges[idx].i = i;
            edges[idx].j = j;
            edges[idx].dist = distance(points[i], points[j]);
            idx++;
        }
    }

    // Sort edges by distance ASC
    qsort(edges, total_edges, sizeof(struct Edge), edge_cmp);

    // Init union-find
    for (int i = 0; i < NUM_POINTS; i++) {
        parent[i] = i;
        size_arr[i] = 1;
    }

    // p1
    // Connect the first 1000 edges (not merges)
    for (int k = 0; k < 1000; k++) {
        unite(edges[k].i, edges[k].j);
    }

    // Compute sizes of all components
    int comp_size[NUM_POINTS] = {0};

    for (int i = 0; i < NUM_POINTS; i++) {
        int r = find(i);
        comp_size[r]++;
    }

    // Collect non-zero sizes
    int sizes[NUM_POINTS];
    int count = 0;

    for (int i = 0; i < NUM_POINTS; i++) {
        if (comp_size[i] > 0)
            sizes[count++] = comp_size[i];
    }

    // Sort descending
    qsort(sizes, count, sizeof(int), cmp_int);

    int32_t part1 = sizes[0] * sizes[1] * sizes[2];

    printf("p1 result %d\n", part1);

    // p2

    int edge_pos = 1000;    // continue after Part 1 edges
    int components = count; // number of components after first 1000 edges
    int last_i = -1, last_j = -1;

    while (components > 1 && edge_pos < total_edges) {

        int a = edges[edge_pos].i;
        int b = edges[edge_pos].j;
        edge_pos++;

        int ra = find(a);
        int rb = find(b);

        if (ra != rb) {
            unite(ra, rb);
            components--;

            last_i = a;
            last_j = b;
        }
    }

    if (components != 1) {
        printf("ERROR: did not reach full connectivity.\n");
        return 1;
    }

    int part2 = points[last_i].x * points[last_j].x;

    printf("p2 result %d\n", part2);

    free(edges);
    return 0;
}
