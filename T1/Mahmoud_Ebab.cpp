#include <iostream>
#include <vector>
#include <queue>

using namespace std;

long long max_bipartite_edges(int n, const vector<vector<int>>& adj) {
    if (n == 1) return 0; 

    vector<int> color(n + 1, -1); 
    queue<int> q; 
    vector<int> count(2, 0); 

    q.push(1);
    color[1] = 0; 
    count[0]++; 

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        int nodeColor = color[node];

        for (int neighbor : adj[node]) {
            if (color[neighbor] == -1) { 
                color[neighbor] = 1 - nodeColor;
                q.push(neighbor);
                count[color[neighbor]]++; 
            } else if (color[neighbor] == nodeColor) {
                cerr << "Error: Graph is not bipartite. This should be impossible for a tree." << endl;
                return -1;
            }
        }
    }

    long long countA = count[0];
    long long countB = count[1];
    long long max_additional_edges = countA * countB - (n - 1);

    return max_additional_edges;
}

int main() {
    int n;
    cin >> n;

    vector<vector<int>> adj(n + 1); 

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    cout << max_bipartite_edges(n, adj) << endl;

    return 0;
}
