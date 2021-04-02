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
    V++; /* Starting index is seen as 1 */
    node *new = malloc(sizeof(node));
    new->id = id++;
    new->outDegree = 0;
    new->inDegree = 0;
    new->visited = 0;
    new->relaxed = 0;
    new->distance = INF;
    new->edges = NULL;

    return new;    
}


/* Adds edge information between two nodes.
Updates in and out degrees. */
void addEdge(graph* g, int u, int v)
{
    g->nodes[u]->outDegree++;
    g->nodes[v]->inDegree++;

    if(g->nodes[u]->edges == NULL)
    {
        g->nodes[u]->edges = malloc(sizeof(node*) * g->nodes[u]->outDegree);
        g->nodes[u]->edges[0] = g->nodes[v];
    }
    else
    {
        g->nodes[u]->edges = realloc(g->nodes[u]->edges, g->nodes[u]->outDegree);
        g->nodes[u]->edges[g->nodes[u]->outDegree - 1] = g->nodes[v];
    }

    
}

/* Free stack head, replaces with next in line */
stack *pop(stack* head)
{
    stack* temp = head->next;
    free(head);

    return temp;
}

/*Adds node to the LIFO */
stack *push(stack* head, node* n)
{
    stack* new = malloc(sizeof(stack));
    new->n = n;
    new->next = head;

    return new;
}

/*Adds a node to the FIFO*/
queue *enqueue(queue* head, node* n)
{
    queue* new = (queue*) malloc(sizeof(struct node));
    new->n = n;
    new->next = head;
    return new;
}

/*Returns node at the front*/
queue *dequeue(queue* head)
{

    queue* temp = NULL, *prev;

    for (temp = head, prev = NULL; temp != NULL; temp = temp->next)
    {
        if (temp == head)
        {
            head = temp->next;
        }
        else
        {
            prev->next = temp->next;
        }
        
    }

    return head;
    
}