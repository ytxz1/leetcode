#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // 方法一：并查集。
    //
    // LeetCode 默认会调用 countCompleteComponents，
    // 所以这里把并查集作为主方法保留。
    int countCompleteComponents(int n, vector<vector<int>>& edges) {
        // parent[x] 表示节点 x 在并查集中的父节点。
        // 如果 parent[x] == x，说明 x 当前就是所在集合的代表节点，也叫根节点。
        vector<int> parent(n);

        // componentSize[root] 只在 root 是根节点时有意义：
        // 它表示这个连通分量中一共有多少个节点。
        vector<int> componentSize(n, 1);

        // 初始化并查集：
        // 一开始没有处理任何边，每个节点都是一个单独的连通分量，
        // 所以每个节点的父节点都是自己。
        for (int node = 0; node < n; ++node) {
            parent[node] = node;
        }

        // findRoot(node) 用来找到 node 所在连通分量的根节点。
        //
        // 这里使用路径压缩：
        // 如果 node 的父节点不是自己，就递归找到真正的根，
        // 并把 node 直接挂到根节点下面。
        function<int(int)> findRoot = [&](int node) -> int {
            if (parent[node] != node) {
                parent[node] = findRoot(parent[node]);
            }
            return parent[node];
        };

        // 先遍历所有边，把有边相连的节点合并到同一个连通分量里。
        for (const auto& edge : edges) {
            int a = edge[0];
            int b = edge[1];

            int rootA = findRoot(a);
            int rootB = findRoot(b);

            // 如果 rootA != rootB，说明 a 和 b 原本属于两个不同的连通分量。
            // 因为它们之间现在有一条边，所以这两个连通分量应该合并。
            if (rootA != rootB) {
                // 按连通分量大小合并：
                // 让较小的集合挂到较大的集合下面，可以让并查集树更矮。
                if (componentSize[rootA] < componentSize[rootB]) {
                    swap(rootA, rootB);
                }

                parent[rootB] = rootA;
                componentSize[rootA] += componentSize[rootB];
            }
        }

        // edgeCount[root] 表示根节点 root 对应的连通分量中有多少条边。
        //
        // 第二次遍历所有边，找到每条边最终属于哪个根节点，再计数。
        vector<int> edgeCount(n, 0);
        for (const auto& edge : edges) {
            int a = edge[0];
            int root = findRoot(a);
            ++edgeCount[root];
        }

        int answer = 0;

        for (int node = 0; node < n; ++node) {
            // 只在根节点处判断一个连通分量。
            if (findRoot(node) != node) {
                continue;
            }

            int nodes = componentSize[node];
            int edgesInComponent = edgeCount[node];
            int expectedEdges = nodes * (nodes - 1) / 2;

            if (edgesInComponent == expectedEdges) {
                ++answer;
            }
        }

        return answer;
    }

    // 方法二：BFS。
    //
    // 思路：
    // 1. 先用 edges 建邻接表 graph。
    // 2. 从每个未访问节点出发，用队列 BFS 找到一个完整连通分量。
    // 3. BFS 过程中统计：
    //    - nodes：当前连通分量中的节点数。
    //    - degreeSum：当前连通分量中所有节点的度数之和。
    // 4. 因为无向图里每条边会贡献 2 次度数，
    //    所以 edgesInComponent = degreeSum / 2。
    // 5. 判断 edgesInComponent 是否等于 nodes * (nodes - 1) / 2。
    int countCompleteComponentsBFS(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph = buildGraph(n, edges);
        vector<bool> visited(n, false);
        int answer = 0;

        for (int start = 0; start < n; ++start) {
            // 已经访问过的节点，说明它属于之前某个连通分量，不需要重复处理。
            if (visited[start]) {
                continue;
            }

            int nodes = 0;
            int degreeSum = 0;

            queue<int> nodeQueue;
            nodeQueue.push(start);
            visited[start] = true;

            while (!nodeQueue.empty()) {
                int current = nodeQueue.front();
                nodeQueue.pop();

                // current 是当前连通分量中的一个节点。
                ++nodes;

                // graph[current].size() 就是 current 的度数。
                // 把每个节点的度数加起来，最后除以 2 才是边数。
                degreeSum += (int)graph[current].size();

                for (int next : graph[current]) {
                    if (visited[next]) {
                        continue;
                    }

                    visited[next] = true;
                    nodeQueue.push(next);
                }
            }

            int edgesInComponent = degreeSum / 2;
            int expectedEdges = nodes * (nodes - 1) / 2;

            if (edgesInComponent == expectedEdges) {
                ++answer;
            }
        }

        return answer;
    }

    // 方法三：DFS。
    //
    // 这里使用栈写迭代 DFS，而不是递归 DFS。
    // 好处是当图很深时，不容易因为递归层数太深导致栈溢出。
    //
    // DFS 和 BFS 的统计逻辑完全一样：
    // 都是在遍历一个连通分量时统计 nodes 和 degreeSum。
    int countCompleteComponentsDFS(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph = buildGraph(n, edges);
        vector<bool> visited(n, false);
        int answer = 0;

        for (int start = 0; start < n; ++start) {
            if (visited[start]) {
                continue;
            }

            int nodes = 0;
            int degreeSum = 0;

            stack<int> nodeStack;
            nodeStack.push(start);
            visited[start] = true;

            while (!nodeStack.empty()) {
                int current = nodeStack.top();
                nodeStack.pop();

                ++nodes;
                degreeSum += (int)graph[current].size();

                for (int next : graph[current]) {
                    if (visited[next]) {
                        continue;
                    }

                    visited[next] = true;
                    nodeStack.push(next);
                }
            }

            int edgesInComponent = degreeSum / 2;
            int expectedEdges = nodes * (nodes - 1) / 2;

            if (edgesInComponent == expectedEdges) {
                ++answer;
            }
        }

        return answer;
    }

private:
    // buildGraph 用 edges 建出无向图的邻接表。
    //
    // graph[x] 中保存所有和节点 x 直接相连的邻居节点。
    // 例如有边 [0, 2]，就要同时加入：
    // graph[0].push_back(2)
    // graph[2].push_back(0)
    vector<vector<int>> buildGraph(int n, const vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);

        for (const auto& edge : edges) {
            int a = edge[0];
            int b = edge[1];

            graph[a].push_back(b);
            graph[b].push_back(a);
        }

        return graph;
    }
};
