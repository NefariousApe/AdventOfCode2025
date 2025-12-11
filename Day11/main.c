#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char name[4];
    char connections[32][4];
    int graphIndex;
    int numConnections;
} Node;

typedef struct {
    Node connections[650];
    int numNodes;
} Graph;

void addNode(Graph *g, Node *n) {
    g->connections[g->numNodes] = *n;
    n->graphIndex = g->numNodes;
    g->numNodes++;
}

void initNode(Node *n) {
    n->numConnections = 0;
    memset(n->name, 0, sizeof(n->name));
}

void initGraph(Graph *g) { g->numNodes = 0; }

void addConnection(Node *n, char connection[4]) {
    strncpy(n->connections[n->numConnections], connection, 3);
    n->connections[n->numConnections][3] = '\0';
    n->numConnections++;
}

void printNode(Node *n) {
    printf("%s: ", n->name);
    for (int i = 0; i < n->numConnections; i++) {
        printf("%s ", n->connections[i]);
    }
    printf("\n");
}

void printGraph(Graph *g) {
    for (int i = 0; i < g->numNodes; i++) {
        printNode(&g->connections[i]);
    }
}

bool contains(char *word, int wordLength, char target) {
    for (int i = 0; i < wordLength; i++) {
        if (word[i] == target) {
            return true;
        }
    }
    return false;
}

int findNodeIndex(Graph *g, char name[4]) {
    for (int i = 0; i < g->numNodes; i++) {
        if (strcmp(g->connections[i].name, name) == 0)
            return i;
    }
    return -1;
}

long dfs(Graph *g, int idx, int targetIdx, long memo[650][2][2], bool hasX,
         bool hasY, int xIdx, int yIdx, bool requireNodes) {
    if (idx == xIdx)
        hasX = true;
    if (idx == yIdx)
        hasY = true;

    if (memo[idx][hasX][hasY] != -1)
        return memo[idx][hasX][hasY];

    if (idx == targetIdx) {
        if (requireNodes)
            return memo[idx][hasX][hasY] = (hasX && hasY ? 1L : 0L);
        else
            return memo[idx][hasX][hasY] = 1L;
    }

    Node *node = &g->connections[idx];
    long total = 0;

    for (int i = 0; i < node->numConnections; i++) {
        int next = findNodeIndex(g, node->connections[i]);
        if (next == -1)
            continue;

        total +=
            dfs(g, next, targetIdx, memo, hasX, hasY, xIdx, yIdx, requireNodes);
    }

    memo[idx][hasX][hasY] = total;
    return total;
}

long findNumPaths(Graph *g, char startName[4], char targetName[4]) {
    int startIdx = findNodeIndex(g, startName);
    int targetIdx = findNodeIndex(g, targetName);

    if (startIdx == -1 || targetIdx == -1)
        return 0L;

    long memo[650][2][2];
    for (int i = 0; i < 650; i++)
        for (int a = 0; a < 2; a++)
            for (int b = 0; b < 2; b++)
                memo[i][a][b] = -1;

    return dfs(g, startIdx, targetIdx, memo, true, true, -1, -1, false);
}

long findNumPathsThroughTwoNodes(Graph *g, char startName[4],
                                 char targetName[4], char xName[4],
                                 char yName[4]) {
    int startIdx = findNodeIndex(g, startName);
    int targetIdx = findNodeIndex(g, targetName);
    int xIdx = findNodeIndex(g, xName);
    int yIdx = findNodeIndex(g, yName);

    if (startIdx == -1 || targetIdx == -1 || xIdx == -1 || yIdx == -1)
        return 0L;

    long memo[650][2][2];
    for (int i = 0; i < 650; i++)
        for (int a = 0; a < 2; a++)
            for (int b = 0; b < 2; b++)
                memo[i][a][b] = -1;

    return dfs(g, startIdx, targetIdx, memo, false, false, xIdx, yIdx, true);
}

int main() {
    FILE *f = fopen("input.txt", "r");

    Graph g;
    Node n;
    char name[128];

    initGraph(&g);
    initNode(&n);
    int startIndex = 0;

    while (1) {
        if (fscanf(f, "%127s", name) != 1)
            break;

        if (contains(name, strlen(name), ':')) {
            if (n.numConnections != 0 || n.name[0] != '\0') {
                addNode(&g, &n);
            }

            initNode(&n);

            strncpy(n.name, name, 3);
            n.name[3] = '\0';

        } else {
            addConnection(&n, name);
        }
    }

    if (n.numConnections != 0 || n.name[0] != '\0') {
        addNode(&g, &n);
    }

    initNode(&n);
    strncpy(n.name, "out", 3);
    addNode(&g, &n);

    int numPaths = findNumPaths(&g, "you", "out");
    printf("p1 %d\n", numPaths);

    long part2 = 0;

    part2 += findNumPaths(&g, "svr", "dac") * findNumPaths(&g, "dac", "fft") *
             findNumPaths(&g, "fft", "out");

    part2 += findNumPaths(&g, "svr", "fft") * findNumPaths(&g, "fft", "dac") *
             findNumPaths(&g, "dac", "out");
    // numPaths = findNumPathsThroughTwoNodes(&g, "svr", "out", "dac", "fft");
    printf("p2 %ld\n", part2); // 2100348264 too low
}
