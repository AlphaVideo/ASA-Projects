#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/* Global Variables */

int V; /* Number of vertexes */
int E; /* Number of edges */
int sources = 0;

/* Functions */

void setSources(graph* Graph);


int main()
{
    int i;

    scanf("%d %d", &V, &E);
    graph *Graph = initGraph(V);

    /* Input parsing */
    for(i = 0; i < E; i++)
    {
        int u, v;
        scanf("%d %d", &u, &v);

        if(u > V || v > V || v == 0 || u == 0)
        {
            fprintf(stderr, "Invalid domino.\n");
            exit(-1);
        }
        addEdge(Graph, u, v);
    }

    /* Nodes with inDegree = 0 are sources and will be counted for the output */
    setSources(Graph);

    return 0;
}

/* Searches the whole graph for nodes with inDegree = 0.
Sets distance to 0 and increments global counter. */
void setSources(graph* g)
{
    int i;
    for(i = 1; i <= V; i++)
    {
        if(g->nodes[i]->inDegree == 0)
        {
            g->nodes[i]->distance = 0;
            sources++;
        }
    }
}