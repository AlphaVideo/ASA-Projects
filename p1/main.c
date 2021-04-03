#include <stdlib.h>
#include <stdio.h>

/* Structures */

typedef struct node {
    int id;
    int inDegree;
    int outDegree;
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


/* Global Variables */

#define INF -1
int V; /* Number of vertexes */
int E; /* Number of edges */
int nSources = 0; /* Output 1 */
int longest = 0; /* Output 2 */
unsigned int ID = 0;
queue *tail = NULL;


/* Main Functions */

queue* setSources(graph* g, queue* sources);
int* topologicalSort(graph *g, queue* sources);
int getLongest(graph *g, int *topSorted);


/* Structure functions */

graph* initGraph(int vertex);
node* newNode(int vertex);
void addEdge(graph* g, int u, int v);

queue *enqueue(queue* head, node* n);
queue *dequeue(queue* head);
node *getQueueNode(queue* head);
void destroyGraph(graph *g);


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

    destroyGraph(Graph);
    free(topSorted);

    return 0;
}

/* Searches the whole graph for nodes with inDegree = 0.
Sets distance to 0 and increments global counter. */
queue* setSources(graph* g, queue* sources)
{
    int i;
    for(i = 0; i < V; i++)
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
        sorted[id] = v->id;
        id++; 

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

/* Creates a graph struct with V vertexes */
graph* initGraph(int vertex)
{
    int i;
    graph *new = malloc(sizeof(graph));
    new->nodes = malloc(sizeof(node*) * vertex);

    for(i = 0; i < vertex; i++)
        new->nodes[i] = newNode(vertex);

    return new;
}


/* Aux function that creates a "blank" node */
node* newNode(int vertex)
{
    node *new = malloc(sizeof(node));
    new->id = ID++;
    new->outDegree = 0;
    new->inDegree = 0;
    new->distance = INF;
    new->edges = malloc(sizeof(node*)*vertex);

    return new;    
}


/* Adds edge information between two nodes.
Updates in and out degrees. */
void addEdge(graph* g, int u, int v)
{
    /* Real indexes are one less */
    u = u - 1;
    v = v - 1;

    g->nodes[u]->outDegree++;
    g->nodes[v]->inDegree++;
    g->nodes[u]->edges[g->nodes[u]->outDegree - 1] = g->nodes[v];
}


/*Adds a node to the FIFO*/
queue *enqueue(queue* head, node* n)
{   
    queue* new = (queue*) malloc(sizeof(struct queue));
    new->n = n;
    
    if(head == NULL) /* Init */
    {
        new->next = NULL;
        new->previous = NULL;

        head = new;
        tail = head;
    }
    else if (head == tail) /* Head is updated, tail will be different from head */
    {
        new->next = NULL;
        new->previous = head;
        tail = new;

        head->next = tail;
    }
    else /* Update through tail for speeeed */
    {
        queue *prev = tail->previous;
        
        tail = new;
        prev->next = tail;
    }

    return head;
}


/* Free queue at the front, updates tail */
queue *dequeue(queue* head)
{
    if(head == NULL)
        return NULL;
    else if(tail == head)
    {
        free(head);
        tail = NULL;
        return NULL;
    }
    else
    {
        queue* next = head->next;
        next->previous = NULL;
        head = next;
        return head;
    }
}

/* Returns front node */
node *getQueueNode(queue* head)
{
    node *u;
    if(head != NULL)
    {
        u = head->n;
        return u;
    }
    else
        return NULL;
}

/* Frees remaining memory */
void destroyGraph(graph *g)
{
    int i;
    for(i = 0; i < V; i++)
    {
        free(g->nodes[i]->edges);
        free(g->nodes[i]);
    }
    free(g->nodes);
    free(g);
}