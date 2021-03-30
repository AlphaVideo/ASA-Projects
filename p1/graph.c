#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

graph* initGraph(int V, int E)
{
    int i;
    graph *new = malloc(sizeof(node*) * V);

    for(i = 0; i < V; i++)
    {
        new->nodes[i].outDegree = 0;
        new->nodes[i].inDegree = 0;
        new->nodes[i].relaxed = 0;
        new->nodes[i].distance = INF;    
        new->nodes[i].edges = malloc(sizeof(node*) * E);
    }

    return new;
}