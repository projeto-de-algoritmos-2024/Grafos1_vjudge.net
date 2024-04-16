#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int vertex;
    struct node *next;
} node;

typedef struct Graph
{
    int numVertices;
    int *visited;
    int *giovanni_numbers;
    node **adjLists;
} Graph;

node *createNode(int v)
{
    node *newNode = malloc(sizeof(node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int vertices)
{
    Graph *graph = malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(node *));
    graph->visited = malloc(vertices * sizeof(int));
    graph->giovanni_numbers = malloc(vertices * sizeof(int));
    for (int i = 0; i < vertices; i++)
    {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
        graph->giovanni_numbers[i] = 1000000;
    }
    graph->giovanni_numbers[0] = 0;
    return graph;
}

void addEdge(Graph *graph, int src, int dest)
{
    node *newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void BFS_DFS(Graph *graph, int startVertex)
{
    node *adjList = graph->adjLists[startVertex];
    node *temp = adjList;
    graph->visited[startVertex] = 1;
    while (temp != NULL)
    {
        int connectedVertex = temp->vertex;
        if (graph->visited[connectedVertex] == 0)
        {
            if (graph->giovanni_numbers[startVertex] + 1 < graph->giovanni_numbers[connectedVertex])
            {
                graph->giovanni_numbers[connectedVertex] = graph->giovanni_numbers[startVertex] + 1;
            }
            BFS_DFS(graph, connectedVertex);
        }
        temp = temp->next;
    }
}

int main()
{
    int T;
    scanf("%d", &T);
    while (T--)
    {
        int P, D;
        scanf("%d %d", &P, &D);
        Graph *graph = createGraph(P);
        for (int i = 0; i < D; i++)
        {
            int x, y;
            scanf("%d %d", &x, &y);
            addEdge(graph, x, y);
        }
        BFS(graph, 0);
        for (int i = 1; i < P; i++)
        {
            printf("%d\n", graph->giovanni_numbers[i]);
        }
        if (T)
        {
            printf("\n");
        }
    }
    return 0;
}
