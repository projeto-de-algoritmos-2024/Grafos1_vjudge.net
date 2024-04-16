#include <stdio.h>
#include <stdlib.h>

#define INF 99999

typedef struct node
{
    int vertex;
    int weight;
    struct node *next;
} node;

typedef struct Graph
{
    int numVertices;
    node **adjLists;
} Graph;

typedef struct heapNode
{
    int vertex;
    int key;
} heapNode;

typedef struct minHeap
{
    int size;
    int capacity;
    int *pos;
    heapNode **array;
} minHeap;

node *createNode(int v, int w)
{
    node *newNode = malloc(sizeof(node));
    newNode->vertex = v;
    newNode->weight = w;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int vertices)
{
    int i;
    Graph *graph = malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(node *));
    for (i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;
    return graph;
}

void addEdge(Graph *graph, int src, int dest, int weight)
{
    node *newNode = createNode(dest, weight);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    newNode = createNode(src, weight);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

heapNode *newHeapNode(int v, int key)
{
    heapNode *heapNode = malloc(sizeof(heapNode));
    heapNode->vertex = v;
    heapNode->key = key;
    return heapNode;
}

minHeap *createMinHeap(int capacity)
{
    minHeap *minHeap = (struct minHeap *)malloc(sizeof(minHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (heapNode **)malloc(capacity * sizeof(heapNode *));
    return minHeap;
}

void swapHeapNode(heapNode **a, heapNode **b)
{
    heapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(minHeap *minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->key < minHeap->array[smallest]->key)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->key < minHeap->array[smallest]->key)
        smallest = right;
    if (smallest != idx)
    {
        heapNode *smallestNode = minHeap->array[smallest];
        heapNode *idxNode = minHeap->array[idx];
        minHeap->pos[smallestNode->vertex] = idx;
        minHeap->pos[idxNode->vertex] = smallest;
        swapHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(minHeap *minHeap)
{
    return minHeap->size == 0;
}

heapNode *extractMin(minHeap *minHeap)
{
    if (isEmpty(minHeap))
        return NULL;
    heapNode *root = minHeap->array[0];
    heapNode *lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->pos[root->vertex] = minHeap->size - 1;
    minHeap->pos[lastNode->vertex] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}

void decreaseKey(minHeap *minHeap, int v, int key)
{
    int i = minHeap->pos[v];
    minHeap->array[i]->key = key;
    while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key)
    {
        minHeap->pos[minHeap->array[i]->vertex] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->vertex] = i;
        swapHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(minHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return 1;
    return 0;
}

void dijkstra(Graph *graph, int src)
{
    int V = graph->numVertices;
    int *dist = (int *)malloc(V * sizeof(int));
    minHeap *minHeap = createMinHeap(V);
    int v, i, u;
    heapNode *minHeapNode;
    node *pCrawl;

    for (v = 0; v < V; ++v)
    {
        dist[v] = INF;
        minHeap->array[v] = newHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    minHeap->array[src] = newHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    minHeap->size = V;

    while (!isEmpty(minHeap))
    {
        minHeapNode = extractMin(minHeap);
        u = minHeapNode->vertex;
        pCrawl = graph->adjLists[u];

        while (pCrawl != NULL)
        {
            int v = pCrawl->vertex;

            if (isInMinHeap(minHeap, v) && dist[u] != INF && pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    int max_dist = 0;
    for (i = 1; i < V; ++i)
        if (dist[i] > max_dist)
            max_dist = dist[i];
    printf("%d\n", max_dist);

    free(dist);
}

int main(void) {
    int n, i, j;
    char weight[10];
    Graph *graph;
    scanf("%d", &n);
    graph = createGraph(n);
    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            scanf("%s", weight);
            if (weight[0] != 'x') {
                int w = atoi(weight);
                addEdge(graph, i, j, w);
                addEdge(graph, j, i, w);
            }
        }
    }
    dijkstra(graph, 0);
    return 0;
}