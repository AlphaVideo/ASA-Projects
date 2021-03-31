#ifndef GRAPH_H
#define GRAPH_H
#define INF -1

typedef struct node {
    int id;
    int inDegree;
    int outDegree;
    int visited;
    int relaxed;
    int distance;
    struct node* *edges;
} node;

typedef struct graph {
    node **nodes;
} graph;

typedef struct stack {
    node *n;
    struct stack *next;
} stack;

/* Structure functions */
graph* initGraph(int V);
node* newNode(int V);
void addEdge(graph* g, int u, int v);
stack *pop(stack* head);
stack *push(stack* head, node* node);


#endif