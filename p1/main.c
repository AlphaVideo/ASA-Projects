#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/* Global Variables */

int V; /* Number of vertexes */
int E; /* Number of edges */
int sources = 0;

/* Functions */

void setSources(graph* Graph);
stack* topologicalSort(graph *g, stack* stk);


int main()
{
    int i;
    stack *topologicalStack = malloc(sizeof(stack));
    topologicalStack->n = NULL;
    topologicalStack->next = NULL;

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

    topologicalStack = topologicalSort(Graph, topologicalStack);

    for(i = 1; i <= V+1; i++)
    {
        printf("%d", topologicalStack->n->id);
        topologicalStack = pop(topologicalStack);
    }


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


/* Returns a stack with the topological order of the given graph. */
stack* topologicalSort(graph *g, stack* stk)
{
    int i, j;
    stack *toProcess= malloc(sizeof(stack) * V);
    toProcess->n = NULL;
    toProcess->next = NULL;

    for(i = 1; i <= V; i++)
    {
        /* Apply algorithm to every source */
        if(g->nodes[i]->inDegree == 0)
        {
            toProcess = push(toProcess, g->nodes[i]); /* Source goes first */

            /* While stack isn't empty */
            while(toProcess->n != NULL)
            {
                node *v = toProcess->n;
                toProcess = pop(toProcess);

                /* Remove from processing stack and skip if already visited */
                if(v->visited)
                    continue;

                v->visited = 1;
                stk = push(stk, v); /* Pushes into the sorted stack */

                /* Handles all adjacent nodes later by putting them in toProcess */
                for(j = 1; j <= V; j++)
                {
                    if(v->edges[i] != NULL && !v->edges[i]->visited)
                        toProcess = push(toProcess, v->edges[i]);
                }
            }
        }
    }

    return stk;
}