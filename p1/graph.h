#ifndef GRAPH_H
#define GRAPH_H
#define INF -1

typedef struct node {
    int id;
    int inDegree;
    int outDegree;
    int relaxed;
    int distance;
    struct node **edges;
} node;

typedef struct graph {
    node **nodes;
} graph;

typedef struct queue {
    node *n;
    struct queue *next;
    struct queue *previous;
} queue;

/* Structure functions */
graph* initGraph(int V);
node* newNode(int V);
void addEdge(graph* g, int u, int v);

queue *enqueue(queue* head, node* n);
queue *dequeue(queue* head);
node *getQueueNode(queue* head);


#endif