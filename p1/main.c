#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/* Global Variables */
int V; /* Number of vertexes */
int E; /* Number of edges */

int main()
{
    scanf("%d %d", &V, &E);
    graph *Graph = initGraph(V, E);
    


    return 0;
}