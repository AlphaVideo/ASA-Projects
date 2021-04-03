#include <iostream>
#include <queue>
#include <vector>
#include <cstdio>

using namespace std;

#define INF -1


// Classes --------------------------------------

class Node
{
    public:

    int inDegree;
    int outDegree;
    int distance;
    vector<int> edges; //Stores child indexes

    Node()
    {
        inDegree = 0;
        outDegree = 0;
        distance = INF;
    }

};

class Graph
{
    public:

    int V;
    int E;
    vector<Node> nodes;

    Graph(int v, int e)
    {
        V = v;
        E = e;

        for(int i = 0; i < V; i++)
        {
            nodes.push_back(Node());
        }
    }

    void addEdge(int u, int v);
    queue<int> setSources();
};

/* Global variables */

int nSources = 0; /* Output 1 */
int longest = 0; /* Output 2 */

/* Functions */

vector<int> topologicalSort(Graph graph, queue<int> toProcess);
int getLongest(Graph graph, vector<int> topSorted);

int main()
{
    int v, e;
    queue<int> sources;
    vector<int> topSorted;

    scanf("%d %d", &v, &e);

    Graph graph(v, e);

    /* Input parsing */
    for(int i = 0; i < graph.E; i++)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        graph.addEdge(u-1, v-1); //Real indexes are one less
    }

    //Nodes with inDegree = 0 are sources and will be counted for the output
    sources = graph.setSources();

    //Topological sort uses a queue with the sources as it's starting point
    topSorted = topologicalSort(graph, sources);

    longest = getLongest(graph, topSorted);

    printf("%d %d\n", nSources, longest);

    return 0;
}

/* Updates out and in degrees, connects parent to child */
void Graph::addEdge(int u, int v)
{
    nodes[u].outDegree++;
    nodes[v].inDegree++;
    nodes[u].edges.push_back(v);
}

/* Returns a queue with all nodes with inDegree = 0 */
queue<int> Graph::setSources()
{
    queue<int> sources;
    for(int i = 0; i < V; i++)
    {
        if(nodes[i].inDegree == 0)
        {
            nodes[i].distance = 0;
            nSources++;
            sources.push(i);
        }
    }
    return sources;
}

/* Returns an array with graph's indexes in topological order
Utilizes Kahn's algorithm */
vector<int> topologicalSort(Graph graph, queue<int> toProcess)
{
    vector<int> sorted;

    /* While queue isn't empty */
    while(!toProcess.empty())
    {
        int id = toProcess.front();

        sorted.push_back(id);
        toProcess.pop();

        /* Handles all adjacent nodes, max E iterations */
        for (int child : graph.nodes[id].edges)
        {
            //Decrements child vertexes' inDegree. If 0 afterwards, it's processed
            if(--graph.nodes[child].inDegree == 0)
                toProcess.push(child);
        }
    }

    return sorted;
}

/* Applies DAG relaxation algorithm to find longest path.
All edges have weight = 1 */
int getLongest(Graph graph, vector<int> topSorted)
{
    int max = 0;

    for(int i : topSorted)
    {
        for(int child : graph.nodes[i].edges)
        {
            /* Is the current path to child longer? If yes, update it */
            if(graph.nodes[child].distance < graph.nodes[i].distance + 1)
            {
                graph.nodes[child].distance = graph.nodes[i].distance + 1;
                if(graph.nodes[child].distance > max)
                    max = graph.nodes[child].distance;
            }
        }
    }

    /* Source domino falling counts towards longest path */
    return max+1;
}
