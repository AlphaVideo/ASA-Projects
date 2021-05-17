#include <iostream>
#include <vector>

using namespace std;

// ~*~*~*~*~*~*~*~*~* Classes *~*~*~*~*~*~*~*~*~*~

class Edge
{
    public:

    int capacity;
    int flow;
    int startVertex;
    int endVertex;

    Edge(int u, int v, int c)
    {
        flow = 0;
        startVertex = u;
        endVertex = v;
        capacity = c;
    }

};

class Node
{
    public:

    vector<Edge> edges; //Stores child edges

    Node()
    {
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

    void addEdge(int u, int v, int c);
};

//Source and target indexes
int source = 0; //Processor X
int target = 0; //Processor Y

int main()
{
    int n, k, v, e; //n-> number of tasks, k-> matrix inputs

    //Graph parameters
    scanf("%d %d", &n, &k);
    e = 2*n + k; //Each task has 2 edges + k additional edges
    v = n+2; //Number of tasks + source + sink
    target = v-1;
    Graph graph(v, e);

    //Adds task edges to the processors
    for(int i = 1; i < n+1; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        graph.addEdge(source, i, x); //Processor X has an edge to task i with weight x;
        graph.addEdge(i, target, y); //Task i has an edge to processor Y with weight y;
    }

    for(int i = 1; i < k+1; i++)
    {
        int u, v, c;
        scanf("%d %d %d", &u, &v, &c);
        graph.addEdge(u, v, c);
    }

    // for(int i = 0; i < graph.V; i++)
    // {
    //     for(Edge e : graph.nodes[i].edges)
    //     {
    //         printf("Start: %d  End: %d   Capacity:%d\n", e.startVertex, e.endVertex, e.capacity);
    //     }
    // }

    return 0;
}

//Adds edge to the according vertex in the Graph class
void Graph::addEdge(int u, int v, int c)
{
    Edge edge(u, v, c);
    nodes[u].edges.push_back(edge);
}