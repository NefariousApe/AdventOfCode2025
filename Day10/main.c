#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int v[16];
} P2State;

typedef struct {
    P2State *arr;
    int capacity, back, front;

} P2Queue;

typedef struct {
    int idx[16]; // which positions to increment
    int count;   // how many indices
} P2Op;

typedef struct {
    int *arr;
    int capacity, back, front;
} Queue;

void p2QueueInit(P2Queue *q, int capacity) {
    q->arr = malloc(sizeof(P2State) * capacity);
    q->front = 0;
    q->back = 0;
    q->capacity = capacity;
}

bool p2Empty(P2Queue *q) { return q->front == q->back; }

bool p2IsFull(P2Queue *q) { return q->back == q->capacity; }

void p2GrowArray(P2Queue *q) {
    int newCap = q->capacity * 2;
    q->arr = realloc(q->arr, newCap * sizeof(int));
    q->capacity = newCap;
}

void p2Enqueue(P2Queue *q, P2State s) {
    if (p2IsFull(q)) {
        p2GrowArray(q);
    }
    q->arr[q->back++] = s;
}

P2State p2Pop(P2Queue *q) { return q->arr[q->front++]; }

void queueInit(Queue *q) {
    q->front = 0;
    q->back = 0;
    q->capacity = 100;
    q->arr = malloc(q->capacity * sizeof(int));
}

bool isEmpty(Queue *q) { return q->front == q->back; }

bool isFull(Queue *q) { return q->back == q->capacity; }

void growArray(Queue *q) {
    int newCap = q->capacity * 2;
    q->arr = realloc(q->arr, newCap * sizeof(int));
    q->capacity = newCap;
}

void enqueue(Queue *q, int value) {
    if (isFull(q)) {
        growArray(q);
    }
    q->arr[q->back++] = value;
}

int pop(Queue *q) {
    if (isEmpty(q)) {
        return -1;
    }

    int value = q->arr[q->front++];
    return value;
}

int findDepth(Queue *q, uint32_t *ops, int opSize, bool *visited) {
    int depth = 0;

    while (!isEmpty(q)) {

        int cutOff = q->back - q->front;

        for (int i = 0; i < cutOff; i++) {

            int state = pop(q);

            if (state == 0)
                return depth;

            for (int j = 0; j < opSize; j++) {

                uint32_t next = state ^ ops[j];

                if (!visited[next]) {
                    visited[next] = true;
                    enqueue(q, next);
                }
            }
        }

        depth++;
    }

    return -1; // no solution
}

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        printf("Failed to open input file.\n");
        return 1;
    }

    char x[128];
    char voltage[128];
    uint32_t opsP1[128];

    int totalP1 = 0;
    int totalP2 = 0;

    clock_t start = clock();

    while (1) {

        if (fscanf(f, "[%127[^]]]", x) != 1)
            break;

        int m = strlen(x);

        // P1 initial state as bitmask
        uint32_t initialState = 0;
        for (int i = 0; i < m; i++) {
            initialState <<= 1;
            initialState |= (x[i] == '#') ? 1 : 0;
        }

        int n = 0;
        P2Op opsP2[128]; // store P2 ops as index-lists
        memset(opsP2, 0, sizeof(opsP2));

        while (1) {

            char buf[128];
            if (fscanf(f, " (%127[^)])", buf) != 1)
                break;

            fgetc(f); // eat ')'

            uint32_t mask = 0;

            char *p = buf;
            char *endptr;

            while (*p != '\0') {

                long idx = strtol(p, &endptr, 10);
                if (p == endptr)
                    break;

                if (idx >= 0 && idx < m) {

                    // P1 op mask
                    mask |= (1u << (m - 1 - idx));

                    // P2 op: this op increments "idx"
                    opsP2[n].idx[opsP2[n].count++] = idx;
                }

                p = endptr;
                if (*p == ',')
                    p++;
                while (*p == ' ')
                    p++;
            }

            opsP1[n++] = mask;
        }

        // Now read "{a,b,c,...}"
        fgets(voltage, sizeof(voltage), f);

        int target[16];
        int targetLen = 0;

        char *p = voltage;
        if (*p == '{')
            p++;

        while (*p && *p != '}') {

            long val = strtol(p, &p, 10);
            target[targetLen++] = val;

            while (*p == ' ' || *p == ',')
                p++;
        }

        bool visited[1 << m];
        memset(visited, 0, sizeof(visited));

        Queue q;
        queueInit(&q);

        visited[initialState] = true;
        enqueue(&q, initialState);

        int depth = findDepth(&q, opsP1, n, visited);
        totalP1 += depth;

        // int presses = findDepthP2(opsP2, n, target, targetLen);
        // totalP2 += presses;
    }

    clock_t end = clock();

    printf("total time: %f\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("p1 total: %d\n", totalP1);
    printf("p2 total: %d\n", totalP2);

    return 0;
}
