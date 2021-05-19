#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// ~*~*~*~*~*~*~*~*~* Constants *~*~*~*~*~*~*~*~*~*~

enum Direction {NONE, UV, VU};

//Source and target indexes
int source = 0; //Processor X
int target = 0; //Processor Y

#define NIL -1

// ~*~*~*~*~*~*~*~*~* Classes *~*~*~*~*~*~*~*~*~*~

class Edge
{
    public:

    int capacity;
    int flow;
    int u;
    int v;
    Direction d;
    

    Edge(int vertexU, int vertexV, int c)
    {
        flow = 0;
        u = vertexU;
        v = vertexV;
        capacity = c;
        d = NONE;
    }

    void pushFlow(int n, Direction dir);
    bool canPush(Direction dir);
    int getAvailableFlow(Direction dir);
    Direction getDirection(int start);
    int getOppositeEnd(int vertex);

};

class Node
{
    public:

    vector<int> edges; //Stores neighbour edge indexes

    Node()
    {
    }

};

class Graph
{
    public:

    int V;
    int E;
    int nextEdge;
    vector<Node> nodes;
    vector<Edge> edges;

    Graph(int v, int e)
    {
        V = v;
        E = e;
        nextEdge = 0;

        for(int i = 0; i < V; i++)
        {
            nodes.push_back(Node());
        }
    }

    void addEdge(int u, int v, int c);
    void pushPathFlow(pair<int,int> path[], int n);
};

// ~*~*~*~*~*~*~*~*~* Functions *~*~*~*~*~*~*~*~*~*~

bool BFS(Graph g, pair<int, int> path[]);
int edmondsKarp(Graph g);
int findPathMinimum(Graph g, pair<int, int> path[]);

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

    //Adds inter-task edges
    for(int i = 1; i < k+1; i++)
    {
        int u, v, c;
        scanf("%d %d %d", &u, &v, &c);
        graph.addEdge(u, v, c);
    }

    // for(Edge e : graph.edges)
    // {
    //     printf("Start: %d  End: %d   Capacity:%d   Flow:%d   Direction:%d\n", e.u, e.v, e.capacity, e.flow, e.d);
    // }

    // int path[graph.V];

    // if(BFS(graph, path))
    //     puts("Yay");
    // else
    //     puts("Nay");
    
    // for(int i: path)
    // {
    //     printf("%d\n", i);
    // }

    int minimumCost = edmondsKarp(graph);
    printf("%d\n", minimumCost);
        
    return 0;
}



//Adds edge to the according vertex in the Graph class
void Graph::addEdge(int u, int v, int c)
{
    Edge edge(u, v, c);
    edges.push_back(edge);
    nodes[u].edges.push_back(nextEdge);
    nodes[v].edges.push_back(nextEdge++);
}

//Adds flux to the edge, dependant on the direction
void Edge::pushFlow(int n, Direction dir)
{
    if(d == NONE) //Init
    {
        flow += n;
        d = dir;
    }
    else if(d == dir) //Same direction
    {
        flow += n;
    }
    else //Opposite directions
    {
        if(flow - n < 0) //Direction changes after flow update
        {
            d = dir;
            flow = abs(flow - n);
        }
        else //Direction remains the same
        {
            flow -= n;
        }
    }
}

//Returns true if edge not full
bool Edge::canPush(Direction dir)
{
    if(dir == d)
    {
        if(flow == capacity)
            return false;
        else
            return true;
    }
    else
        return true;
}

//Get the flow that you can push through the edge
int Edge::getAvailableFlow(Direction dir)
{
    if(dir == d)
        return capacity - flow;
    else if (dir != d)
        return capacity + flow;
    else //NONE == Not used yet
        return capacity;
}

//Returns the direction of the flow when given starting vertex
Direction Edge::getDirection(int start)
{
    if(start == u)
        return UV;
    else
        return VU;
}

/* Performs a BFS on the graph, returning true if path is found.
Path is updated by reference */
bool BFS(Graph g, pair<int, int> path[])
{
    queue<int> q;
    bool visited[g.V] = {false};
    
    //Init by pushing source
    q.push(source);
    visited[source] = true; //Each vertex is only visited once
    path[source].first = path[source].second = NIL; //Path saves the parent that accessed them and edge used
    
    while(!q.empty())
    {
        int u = q.front();
        q.pop();

        for(int e : g.nodes[u].edges)
        {
            int v = g.edges[e].getOppositeEnd(u);
            Direction dir = g.edges[e].getDirection(v);

            int available = g.edges[e].getAvailableFlow(dir);
            printf("U: %d  V: %d  Dir: %d  Available: %d\n", u, v, dir, available);

            //v must not be visited and edge must not be full
            if(!visited[v] && g.edges[e].canPush(dir)) 
            {
                if(v == target) //Exit condition
                {
                    path[v].first = u; //v was visited by u
                    path[v].second = e; //Visit used edge e

                    for(pair<int, int> current = path[g.V-1]; current.first != NIL; current = path[current.first])
                    {
                        printf(" Parent:%d  EdgeU:%d  EdgeV:%d  Capacity:%d  Flow:%d  Direction:%d  Available:%d  Dir:%d\n", current.first, 
                        g.edges[current.second].u, g.edges[current.second].v ,g.edges[current.second].capacity, g.edges[current.second].flow,
                        g.edges[current.second].d, g.edges[current.second].getAvailableFlow(dir), dir);
                    }
                    printf("\n");

                    return true;
                }
                else
                {
                    q.push(v);
                    visited[v] = true;
                    path[v].first = u; //v was visited by u
                    path[v].second = e; //Visit used edge e
                }
            }
        }
    }

    return false;
}

/* Applies Edmonds Karp algorithm to push flux.
Returns total amount pushed */
int edmondsKarp(Graph g)
{
    int maxFlow = 0;

    /* Path is a sequence of parent indexes
    path[u] = v means v is u's parent */
    pair<int, int> path[g.V]; 

    while(BFS(g, path))
    {

        // for(Edge e : g.edges)
        // {
        //     printf("Start: %d  End: %d   Capacity:%d   Flow:%d   Direction:%d\n", e.u, e.v, e.capacity, e.flow, e.d);
        // }

        int minimum = findPathMinimum(g, path);
        printf("Pushing: %d\n", minimum);
        g.pushPathFlow(path, minimum);
        maxFlow += minimum;

    }

    return maxFlow;
}

/* Returns the least amount of flow you can push through the path */
int findPathMinimum(Graph g, pair<int, int> path[])
{
    int m = NIL;

    //Iterates through each pair
    for(pair<int, int> current = path[g.V-1]; current.first != NIL; current = path[current.first])
    {
        if(m == NIL)
        {
            Direction dir = g.edges[current.second].getDirection(path[current.first].first); //Parent to son direction
            m = g.edges[current.second].getAvailableFlow(dir);
        }
        else
        {
            Direction dir = g.edges[current.second].getDirection(path[current.first].first);
            m = min(g.edges[current.second].getAvailableFlow(dir), m);
        }
    }

    return m;
}

/* Pushes the given flow throughout the path */
void Graph::pushPathFlow(pair<int,int> path[], int n)
{
    for(pair<int, int> current = path[V-1]; current.first != NIL; current = path[current.first])
    {
        Direction dir = edges[current.second].getDirection(path[current.first].first);
        edges[current.second].pushFlow(n, dir);
    }
}

/* Returns the other end of the edge given one end*/
int Edge::getOppositeEnd(int vertex)
{
    if(vertex == u)
    {
        return v;
    }
    else
        return u;
}

