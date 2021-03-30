#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

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

node* newNode(int V)
{
    node *new = malloc(sizeof(node));
    new->outDegree = 0;
    new->inDegree = 0;
    new->relaxed = 0;
    new->distance = INF;
    new->edges = malloc(sizeof(node*) * V);

    return new;    
}

void addEdge(graph* g, int u, int v)
{
    g->nodes[u]->outDegree++;
    g->nodes[v]->inDegree++;
    g->nodes[u]->edges[v] = g->nodes[v];
}