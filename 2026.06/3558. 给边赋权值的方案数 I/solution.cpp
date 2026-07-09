#include <bits/stdc++.h>  // 引入常用标准库：vector、queue、stack、iostream、algorithm 等都包含在里面。
using namespace std;       // 使用 std 命名空间，后面可以直接写 vector、queue、cout。

/*
 * 3558. 给边赋权值的方案数 I
 *
 * 本题给一棵无向树，节点编号从 1 到 n，其中 1 是根节点。
 * 每条边可以被赋值为 1 或 2。
 * 题目只关心：从根节点 1 到某个最深节点的路径。
 * 要求：这条路径上的边权和为奇数，问有多少种赋值方案。
 *
 * 关键变量：
 * edges     : 输入的无向边列表。
 * n         : 节点数量，因为树的边数是 n - 1，所以 n = edges.size() + 1。
 * graph     : 邻接表，用来存储树。
 * maxDepth  : 根节点 1 到最深节点的边数。
 * MOD       : 取模常数 1e9 + 7。
 *
 * 核心结论：
 * 如果根节点到最深节点的路径长度为 d，也就是路径上有 d 条边，
 * 那么每条边有两种赋值：1 或 2。
 *
 * 因为 2 是偶数，不改变路径和的奇偶性；
 * 因为 1 是奇数，会改变路径和的奇偶性；
 * 所以路径和为奇数 <=> 路径上赋值为 1 的边数为奇数。
 *
 * 长度为 d 的 1/2 序列中，奇数和方案数正好是总方案数的一半：
 * total = 2^d
 * odd   = 2^(d - 1)
 *
 * 所以本题真正要做的是：
 * 1. 求 maxDepth。
 * 2. 返回 2^(maxDepth - 1) % MOD。
 */

// ============================================================================
// 方法一：递归 DFS 求最大深度
// ============================================================================
// 这是最直接的提交写法。
// DFS 会从根节点 1 开始向下走，每走过一条边，depth 就加 1。
// 遍历过程中用 maxDepth 记录遇到过的最大 depth。
class SolutionDFS {  // 定义递归 DFS 版本的解法类。
private:             // private 区域中放类内部使用的常量和辅助函数。
    static constexpr long long MOD = 1000000007;  // 题目要求答案对 1e9+7 取模。

    int quickPow(long long base, int exp) {  // 快速幂函数：计算 base^exp % MOD。
        long long ans = 1;                   // ans 保存当前已经累乘出来的答案，初始为乘法单位元 1。

        while (exp > 0) {                    // 只要指数 exp 还没有被处理完，就继续循环。
            if (exp & 1) {                   // 如果 exp 的二进制最低位是 1，说明当前 base 需要乘进答案。
                ans = ans * base % MOD;      // 把当前 base 乘入 ans，并立刻取模防止溢出。
            }                                // if 结束。

            base = base * base % MOD;        // base 平方，相当于从 2^k 次方推进到 2^(k+1) 次方。
            exp >>= 1;                       // exp 右移一位，表示已经处理完当前最低位。
        }                                    // while 结束，所有二进制位都处理完。

        return static_cast<int>(ans);        // ans 已经取模，可以安全转成 int 返回。
    }                                        // quickPow 函数结束。

public:                                      // public 区域中放 LeetCode 会调用的主函数。
    int assignEdgeWeights(vector<vector<int>>& edges) {  // 主函数：输入边列表，返回赋权方案数。
        int n = static_cast<int>(edges.size()) + 1;       // 树有 n 个节点、n-1 条边，所以节点数 = 边数 + 1。
        vector<vector<int>> graph(n + 1);                 // 建立邻接表；节点从 1 开始，所以大小开 n+1。

        for (const auto& edge : edges) {                  // 遍历输入中的每一条无向边。
            int u = edge[0];                              // u 是这条边的一个端点。
            int v = edge[1];                              // v 是这条边的另一个端点。
            graph[u].push_back(v);                        // 无向边 u-v：把 v 加入 u 的邻居列表。
            graph[v].push_back(u);                        // 无向边 u-v：把 u 加入 v 的邻居列表。
        }                                                 // 建图结束。

        int maxDepth = 0;                                 // maxDepth 记录根节点 1 到最深节点的最大边数。

        auto dfs = [&](auto&& self, int node, int parent, int depth) -> void {  // 定义递归 lambda。
            maxDepth = max(maxDepth, depth);                                    // 用当前 depth 更新最大深度。

            for (int next : graph[node]) {                                      // 遍历当前节点 node 的所有邻居 next。
                if (next == parent) {                                           // 如果 next 是父节点，说明会走回头路。
                    continue;                                                   // 跳过父节点，避免在无向图中来回递归。
                }                                                               // if 结束。
                self(self, next, node, depth + 1);                              // 递归访问子节点 next，深度加 1。
            }                                                                   // 邻居遍历结束。
        };                                                                      // dfs lambda 定义结束。

        dfs(dfs, 1, 0, 0);                              // 从根节点 1 开始 DFS；父节点设为 0；根深度为 0。

        if (maxDepth == 0) {                            // 如果最大深度为 0，说明没有边，只有根节点。
            return 0;                                   // 没有边时路径和是 0，不是奇数，所以答案是 0。
        }                                               // if 结束。

        return quickPow(2, maxDepth - 1);               // 根据公式返回 2^(maxDepth-1) % MOD。
    }                                                   // assignEdgeWeights 函数结束。
};                                                      // SolutionDFS 类结束。

// ============================================================================
// 方法二：BFS 求最大深度
// ============================================================================
// BFS 是层序遍历。
// 队列 q 中存 pair<int, int>：
// first  = 当前节点 node；
// second = 当前节点到根节点 1 的边数 depth。
class SolutionBFS {                                     // 定义 BFS 版本的解法类。
private:                                                // private 区域。
    static constexpr long long MOD = 1000000007;        // 取模常数。

    int quickPow(long long base, int exp) {             // 快速幂函数，含义与 DFS 版本完全相同。
        long long ans = 1;                              // 初始化答案为 1。

        while (exp > 0) {                               // 当指数还没处理完时循环。
            if (exp & 1) {                              // 如果当前最低二进制位为 1。
                ans = ans * base % MOD;                 // 累乘当前 base。
            }                                           // if 结束。
            base = base * base % MOD;                   // base 自乘，进入下一位。
            exp >>= 1;                                  // 指数右移一位。
        }                                               // while 结束。

        return static_cast<int>(ans);                   // 返回 int 类型答案。
    }                                                   // quickPow 结束。

public:                                                 // public 区域。
    int assignEdgeWeights(vector<vector<int>>& edges) { // 主函数。
        int n = static_cast<int>(edges.size()) + 1;      // 计算节点数量。
        vector<vector<int>> graph(n + 1);                // 创建邻接表。

        for (const auto& edge : edges) {                 // 遍历每条边。
            int u = edge[0];                             // 取出端点 u。
            int v = edge[1];                             // 取出端点 v。
            graph[u].push_back(v);                       // 加入 u -> v。
            graph[v].push_back(u);                       // 加入 v -> u。
        }                                                // 建图完成。

        queue<pair<int, int>> q;                         // BFS 队列，保存 {节点编号, 深度}。
        vector<int> visited(n + 1, 0);                   // visited[x] 表示节点 x 是否已经入队访问过。

        q.push({1, 0});                                  // 根节点 1 入队，根节点深度为 0。
        visited[1] = 1;                                  // 标记根节点 1 已访问。

        int maxDepth = 0;                                // 初始化最大深度为 0。

        while (!q.empty()) {                             // 只要队列不为空，就继续 BFS。
            auto [node, depth] = q.front();              // 取出队首节点和它的深度。
            q.pop();                                     // 弹出队首，表示开始处理这个节点。

            maxDepth = max(maxDepth, depth);             // 用当前节点深度更新最大深度。

            for (int next : graph[node]) {               // 遍历当前节点的所有邻居。
                if (visited[next]) {                     // 如果邻居已经访问过。
                    continue;                            // 跳过，避免重复入队。
                }                                        // if 结束。

                visited[next] = 1;                       // 标记 next 已访问。
                q.push({next, depth + 1});               // next 入队，它的深度是当前 depth + 1。
            }                                            // 邻居遍历结束。
        }                                                // BFS 结束。

        if (maxDepth == 0) {                             // 如果没有边。
            return 0;                                    // 答案为 0。
        }                                                // if 结束。
        return quickPow(2, maxDepth - 1);                // 返回 2^(maxDepth-1)。
    }                                                    // assignEdgeWeights 结束。
};                                                       // SolutionBFS 类结束。

// ============================================================================
// 方法三：栈模拟 DFS
// ============================================================================
// 这个版本不用递归，而是自己维护 stack。
// parent[x] 记录 x 的父节点，避免走回头路。
// depth[x] 记录 x 到根节点 1 的边数。
class SolutionIterativeDFS {                             // 定义迭代 DFS 版本的解法类。
private:                                                 // private 区域。
    static constexpr long long MOD = 1000000007;         // 取模常数。

    int quickPow(long long base, int exp) {              // 快速幂函数。
        long long ans = 1;                               // 初始化答案为 1。

        while (exp > 0) {                                // 指数大于 0 时继续处理。
            if (exp & 1) {                               // 如果当前最低位是 1。
                ans = ans * base % MOD;                  // 把当前 base 乘入答案。
            }                                            // if 结束。
            base = base * base % MOD;                    // base 平方。
            exp >>= 1;                                   // 指数右移。
        }                                                // while 结束。

        return static_cast<int>(ans);                    // 返回答案。
    }                                                    // quickPow 结束。

public:                                                  // public 区域。
    int assignEdgeWeights(vector<vector<int>>& edges) {  // 主函数。
        int n = static_cast<int>(edges.size()) + 1;       // 计算节点数量。
        vector<vector<int>> graph(n + 1);                 // 创建邻接表。

        for (const auto& edge : edges) {                  // 遍历每条无向边。
            int u = edge[0];                              // 端点 u。
            int v = edge[1];                              // 端点 v。
            graph[u].push_back(v);                        // 存 u 的邻居 v。
            graph[v].push_back(u);                        // 存 v 的邻居 u。
        }                                                 // 建图完成。

        vector<int> parent(n + 1, 0);                     // parent[x] 表示 x 的父节点，初始为 0。
        vector<int> depth(n + 1, 0);                      // depth[x] 表示 x 到根节点 1 的边数，初始为 0。

        stack<int> st;                                    // 栈用于模拟 DFS。
        st.push(1);                                       // 根节点 1 入栈。

        parent[1] = -1;                                   // 根节点没有父节点，用 -1 表示。

        int maxDepth = 0;                                 // 初始化最大深度为 0。

        while (!st.empty()) {                             // 只要栈不为空，就继续 DFS。
            int node = st.top();                          // 取出栈顶节点。
            st.pop();                                     // 弹出栈顶节点，准备处理。

            maxDepth = max(maxDepth, depth[node]);        // 用当前节点的深度更新最大深度。

            for (int next : graph[node]) {                // 遍历当前节点的所有邻居。
                if (next == parent[node]) {               // 如果邻居是父节点。
                    continue;                             // 跳过父节点，避免走回头路。
                }                                         // if 结束。

                parent[next] = node;                      // 设置 next 的父节点为当前 node。
                depth[next] = depth[node] + 1;            // 设置 next 的深度为当前深度 + 1。
                st.push(next);                            // 把 next 入栈，等待后续处理。
            }                                             // 邻居遍历结束。
        }                                                 // 栈模拟 DFS 结束。

        if (maxDepth == 0) {                              // 如果树中没有边。
            return 0;                                     // 返回 0。
        }                                                 // if 结束。
        return quickPow(2, maxDepth - 1);                 // 返回公式答案。
    }                                                     // assignEdgeWeights 结束。
};                                                        // SolutionIterativeDFS 类结束。

/*
 * LeetCode 提交说明：
 * LeetCode 默认要求类名叫 Solution。
 * 这里让 Solution 继承 SolutionDFS，表示默认提交递归 DFS 版本。
 * 如果想提交 BFS，就改成：class Solution : public SolutionBFS {};
 * 如果想提交栈模拟 DFS，就改成：class Solution : public SolutionIterativeDFS {};
 */
class Solution : public SolutionDFS {};                   // LeetCode 实际提交入口类。

// main 函数只用于本地测试。
// LeetCode 提交时可以删除 main 函数，只保留 class Solution。
int main() {                                              // 程序入口。
    SolutionDFS dfsSolution;                              // 创建 DFS 版本对象。
    SolutionBFS bfsSolution;                              // 创建 BFS 版本对象。
    SolutionIterativeDFS iterativeSolution;                // 创建栈模拟 DFS 版本对象。

    vector<vector<int>> edges1 = {{1, 2}};                 // 示例 1：只有一条边，最大深度为 1。
    vector<vector<int>> edges2 = {{1, 2}, {1, 3}, {3, 4}, {3, 5}};  // 示例 2：最大深度为 2。
    vector<vector<int>> edges3 = {{1, 2}, {2, 3}, {3, 4}}; // 示例 3：链式树，最大深度为 3。

    cout << dfsSolution.assignEdgeWeights(edges1) << '\n';       // 输出 1，因为 2^(1-1)=1。
    cout << bfsSolution.assignEdgeWeights(edges2) << '\n';       // 输出 2，因为 2^(2-1)=2。
    cout << iterativeSolution.assignEdgeWeights(edges3) << '\n'; // 输出 4，因为 2^(3-1)=4。

    return 0;                                             // main 正常结束。
}                                                         // main 函数结束。
