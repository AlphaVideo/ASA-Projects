#ifndef GRAPH_H
#define GRAPH_H
#define INF -1

typedef struct node {
    int inDegree;
    int outDegree;
    int relaxed;
    int distance;
    struct node* *edges;
} node;

typedef struct graph {
    node **nodes;
} graph;

/* Structure functions */
graph* initGraph(int V);
node* newNode(int V);
void addEdge(graph* g, int u, int v);


#endif