#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/* Global Variables */

int V; /* Number of vertexes */
int E; /* Number of edges */
int nSources = 0;

/* Functions */

queue* setSources(graph* g, queue* sources);
void topologicalSort(graph *g, queue* sources);


int main()
{
    int i;
    scanf("%d %d", &V, &E);
    graph *Graph = initGraph(V);
    queue *sources = NULL;

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
    sources = setSources(Graph, sources);
    /*topologicalSort(Graph, sources);*/

    while(sources != NULL)
    {
        node* v;
        v = getQueueNode(sources);
        sources = dequeue(sources);
        if(v != NULL)
            printf("%d\n", v->id);

    }

    return 0;
}

/* Searches the whole graph for nodes with inDegree = 0.
Sets distance to 0 and increments global counter. */
queue* setSources(graph* g, queue* sources)
{
    int i;
    for(i = 1; i <= V; i++)
    {
        if(g->nodes[i]->inDegree == 0)
        {
            g->nodes[i]->distance = 0;
            nSources++;
            sources = enqueue(sources, g->nodes[i]);
        }
    }

    return sources;
}


/* Returns a stack with the topological order of the given graph. */
void topologicalSort(graph *g, queue* sources)
{
    int i;

    /* The processing queue will start out with the sources */
    queue *toProcess = sources;

    while (sources != NULL)
    {
        printf("CHEGUEI %d", sources->n->id);
    }
    


    /* While stack isn't empty */
    while(toProcess != NULL)
    {
        node *v = dequeue(toProcess);
        printf("%d\n", v->id);


        /* Handles all adjacent nodes later by putting them in toProcess */
        for(i = 0 ; i < v->outDegree; i++)
        {
            if(--v->edges[i]->inDegree == 0)
                toProcess = enqueue(toProcess, v->edges[i]);
        }
    }
}