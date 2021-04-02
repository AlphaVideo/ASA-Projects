#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/* Global Variables */

int V; /* Number of vertexes */
int E; /* Number of edges */
int nSources = 0; /* Output 1 */
int longest = 0; /* Output 2 */

/* Functions */

queue* setSources(graph* g, queue* sources);
int* topologicalSort(graph *g, queue* sources);
int getLongest(graph *g, int *topSorted);


int main()
{
    int i;
    scanf("%d %d", &V, &E);
    graph *Graph = initGraph(V);
    queue *sources = NULL;
    int *topSorted = NULL;

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
    topSorted = topologicalSort(Graph, sources);

    longest = getLongest(Graph, topSorted);

    printf("%d %d\n", nSources, longest);

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
        else
            g->nodes[i]->distance = INF;
    }

    return sources;
}


/* Returns an index array with the topological order of the given graph. 
Utilizes Kahn's algorithm. */
int* topologicalSort(graph *g, queue* sources)
{
    int i, id = 0;
    int *sorted = malloc(sizeof(int)*V);

    /* The processing queue will start out with the sources */
    queue *toProcess = sources;

    /* While stack isn't empty */
    while(toProcess != NULL)
    {
        node *v = getQueueNode(toProcess);
        toProcess = dequeue(toProcess);
        sorted[id++] = v->id; 

        /* Handles all adjacent nodes later by putting them in toProcess */
        for(i = 0 ; i < v->outDegree; i++) /* On max, does E iterations */
        {
            /* Decrements child vertexes. If one now has inDegree 0, it's processed */
            if(--v->edges[i]->inDegree == 0) 
                toProcess = enqueue(toProcess, v->edges[i]);
        }
    }

    return sorted;
}

/* Applies DAG relaxation algorithm to find longest path.
All edges have weight = 1 */
int getLongest(graph *g, int *topSorted)
{
    int i, j, max = 0;

    /* In topological order */
    for(i = 0; i < V; i++)
    {
        int id = topSorted[i];
        node *s = g->nodes[id];

        for(j = 0; j < s->outDegree; j++)
        {
            /* Is the current path to child longer? If yes, update it */
            if(s->edges[j]->distance < s->distance + 1)
            {
                s->edges[j]->distance = s->distance + 1;
                if(s->edges[j]->distance > max)
                    max = s->edges[j]->distance;
            }
        }
    }

    /* Source domino falling counts towards longest path */
    return max+1;
}