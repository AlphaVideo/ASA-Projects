#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// ~*~*~*~*~*~*~*~*~* Constants *~*~*~*~*~*~*~*~*~*~

//Source and target indexes
int source = 0; //Processor X
int target = 0; //Processor Y

#define NIL -1

// ~*~*~*~*~*~*~*~*~* Classes *~*~*~*~*~*~*~*~*~*~

class Graph
{
    public:

    int V;
    int E;
    int **nodes;

    Graph(int v, int e)
    {
        V = v;
        E = e;

        nodes = new int*[V];
        
        for(int i = 0; i < V; i++)
        {
            nodes[i] = new int[V];
            
            for(int j = 0; j < V; j++)
                nodes[i][j] = 0;
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

    int minimumCost = edmondsKarp(graph);
    printf("%d\n", minimumCost);
        
    return 0;
}



//Adds edge to the according vertex in the Graph class
void Graph::addEdge(int u, int v, int c)
{
    if(u != source && v != source)
        nodes[v][u] = c;

    nodes[u][v] = c;
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
    path[source].first = NIL;
    path[source].second = 0; //Path saves the parent that accessed them and the capacity used
    
    while(!q.empty())
    {
        int u = q.front();
        q.pop();

        for(int v = 0; v < g.V; v++)
        {
            int cap = g.nodes[u][v];

            //v must not be visited and edge must not be full
            if(cap == 0)
                continue;
            if(!visited[v]) 
            {
                if(v == target) //Exit condition
                {
                    path[v].first = u; //v was visited by u
                    path[v].second = cap; //Visit had this cap

                    return true;
                }
                else
                {
                    q.push(v);
                    visited[v] = true;
                    path[v].first = u; //v was visited by u
                    path[v].second = cap; //Visit had this cap
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
        int minimum = findPathMinimum(g, path);
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
            m = current.second;
        }
        else
        {
            m = min(current.second, m);
        }
    }

    return m;
}

/* Pushes the given flow throughout the path */
void Graph::pushPathFlow(pair<int,int> path[], int n)
{
    int child = V-1;
    for(pair<int, int> current = path[V-1]; current.first != NIL; current = path[current.first])
    {
        int parent = current.first;

        if(parent == NIL)
            parent = 0;

        nodes[parent][child] -= n; //Available capacity updated
        nodes[child][parent] += n; //Reflux updated

        child = current.first;
    }
}

