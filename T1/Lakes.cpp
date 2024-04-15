#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

inline int key(int x, int y, int m) {
    return x * m + y;
}

int dfs(int node, unordered_map<int, vector<int>>& adjList, unordered_map<int, bool>& visited, const unordered_map<int, int>& depths) {
    if (visited[node]) return 0; 
    visited[node] = true; 
    int volume = depths.at(node); 
    for (int neighbor : adjList[node]) {
        volume += dfs(neighbor, adjList, visited, depths); 
    }
    return volume;
}

int main() {
    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;
        unordered_map<int, int> depths; 
        unordered_map<int, vector<int>> adjList; 
        unordered_map<int, bool> visited; 

        vector<vector<int>> grid(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cin >> grid[i][j];
                if (grid[i][j] > 0) {
                    int node = key(i, j, m);
                    depths[node] = grid[i][j];
                    visited[node] = false; 
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] > 0) {
                    int node = key(i, j, m);
                    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; 
                    for (auto& dir : directions) {
                        int ni = i + dir.first, nj = j + dir.second;
                        if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] > 0) {
                            int neighbor = key(ni, nj, m);
                            adjList[node].push_back(neighbor);
                        }
                    }
                }
            }
        }

        int largestVolume = 0;

        for (auto& pair : depths) {
            if (!visited[pair.first]) { 
                largestVolume = max(largestVolume, dfs(pair.first, adjList, visited, depths));
            }
        }

        cout << largestVolume << endl;
    }

    return 0;
}
