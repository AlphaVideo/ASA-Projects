#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int id = 1;

/* Creates a graph struct with V vertexes */
graph* initGraph(int V)
{
    int i;
    V++; /* Starting index is seen as 1 */
    graph *new = malloc(sizeof(graph));
    new->nodes = malloc(sizeof(node*) * V);
    new->nodes[0] = NULL; 

    for(i = 1; i < V; i++)
        new->nodes[i] = newNode(V);

    return new;
}


/* Aux function that creates a "blank" node */
node* newNode(int V)
{
    int i;
    V++; /* Starting index is seen as 1 */
    node *new = malloc(sizeof(node));
    new->id = id++;
    new->outDegree = 0;
    new->inDegree = 0;
    new->visited = 0;
    new->relaxed = 0;
    new->distance = INF;
    new->edges = malloc(sizeof(node*) * V);

    for(i = 0; i < V; i++)
        new->edges[i] = NULL;

    return new;    
}


/* Adds edge information between two nodes.
Updates in and out degrees. */
void addEdge(graph* g, int u, int v)
{
    g->nodes[u]->outDegree++;
    g->nodes[v]->inDegree++;
    g->nodes[u]->edges[v] = g->nodes[v];
}

/* Free stack head, replaces with next in line */
stack *pop(stack* head)
{
    stack* temp = head->next;
    free(head->n);
    free(head);

    return temp;
}

/*Adds node to the LIFO */
stack *push(stack* head, node* n)
{
    stack* new = malloc(sizeof(stack));
    new->n = malloc(sizeof(node));
    new->n = n;
    new->next = head;

    return new;
}