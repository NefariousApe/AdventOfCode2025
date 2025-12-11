#include <stdio.h>
#include <string.h>

typedef struct {
    char name[4];
    char connections[32][4];
    int numConnections;
} Node;

typedef struct {
    Node connections[650];
    int numNodes;
} Graph;

void addNode(Graph g, Node n) {
    g.connections[g.numNodes] = n;
    g.numNodes++;
}

void initNode(Node n) { n.numConnections = 0; }

void initGraph(Graph g) { g.numNodes = 0; }

void addConnection(Node n, char connection[4]) {
    *n.connections[n.numConnections] = *connection;
    n.numConnections++;
}

void printNode(Node n) {
    printf("%s: ", n.name);
    for (size_t i = 0; i < n.numConnections; i++) {
        printf("%s ", n.connections[i]);
    }
}

void printGraph(Graph g) {
    for (size_t i = 0; i < g.numNodes; i++) {
        printNode(g.connections[i]);
    }
}

bool contains(char *word, int wordLength, char target) {
    for (size_t i = 0; i < wordLength; i++) {
        if (word[i] == target) {
            return true;
        }
    }
    return false;
}

int main() {
    FILE *f = fopen("input.txt", "r");

    Graph g;
    Node n;
    char name[4], connections[128];
    initGraph(g);
    initNode(n);

    while (1) {
        if (fscanf(f, "%s", name) != 1)
            break;
        if (contains(name, strlen(name), ':')) {
            if (n.numConnections != 0) {
                addNode(g, n);
            }
            initNode(n);
            memcpy(n.name, &name, 3 * sizeof(char));
        } else {
            addConnection(n, name);
        }
    }
    printGraph(g);
}