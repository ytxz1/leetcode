/*
 * LeetCode 1345. 跳跃游戏 IV
 *
 * 题目描述：
 *   给你一个整数数组 arr，你一开始在数组的第一个元素处（下标为 0）。
 *   每一步，你可以从下标 i 跳到下标：
 *     1. i + 1（需满足 i + 1 < arr.length）
 *     2. i - 1（需满足 i - 1 >= 0）
 *     3. j（需满足 arr[i] == arr[j] 且 i != j）
 *   返回到达数组最后一个元素的下标处所需的最少操作次数。
 *
 * ============================================================
 * 解题思路：
 * ============================================================
 *
 * 1. 问题本质：
 *    这是一个「图的最短路径」问题。每个索引是图中的一个节点，
 *    三种跳跃方式定义了节点之间的边。要求从节点 0 到节点 n-1 的最短路径。
 *
 * 2. 算法选择：BFS（广度优先搜索）
 *    - BFS 天然按「层」遍历，每一层代表一步跳跃
 *    - 第一次到达目标节点时的层数就是最少跳跃次数
 *    - 不需要像 Dijkstra 那样处理权重，因为每步代价相同（都是 1）
 *
 * 3. 关键优化：
 *    - 预处理：用哈希表记录每个值对应的所有索引，跳转时 O(1) 查找
 *    - 剪枝：处理完某个值的「相同值跳转」后，立即删除该值的映射
 *      因为这些节点已经全部入队或已访问，再次处理毫无意义
 *
 * 4. 复杂度分析：
 *    - 时间复杂度：O(n)
 *      每个节点最多入队一次，哈希表中每个值最多被遍历一次
 *    - 空间复杂度：O(n)
 *      哈希表存储所有索引，visited 数组和 BFS 队列各占 O(n)
 */

#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

class Solution {
public:
    int minJumps(vector<int>& arr) {
        int n = arr.size();

        // 边界情况：只有一个元素，不需要跳跃
        if (n == 1) return 0;

        /*
         * 第一步：构建值 → 索引列表 的哈希表
         *
         * 例如 arr = [7, 7, 7, 7, 7, 11, 7]
         * 则 valueToIndices = {
         *     7  → [0, 1, 2, 3, 4, 6],
         *     11 → [5]
         * }
         *
         * 作用：当我们在索引 i 时，可以瞬间找到所有 arr[j] == arr[i] 的位置 j
         */
        unordered_map<int, vector<int>> valueToIndices;
        for (int i = 0; i < n; i++) {
            valueToIndices[arr[i]].push_back(i);
        }

        /*
         * 第二步：BFS 初始化
         *
         * 队列：存储待处理的索引，起始点为 0
         * visited：标记已访问的索引，防止重复访问造成死循环
         * steps：记录当前跳跃步数
         */
        queue<int> q;
        q.push(0);
        vector<bool> visited(n, false);
        visited[0] = true;
        int steps = 0;

        /*
         * 第三步：BFS 主循环
         *
         * 每次外层循环处理「一整层」节点（即步数相同的节点）
         * 内层循环遍历当前层的所有节点，扩展出下一层的节点
         */
        while (!q.empty()) {
            int size = q.size();  // 当前层的节点数量

            for (int i = 0; i < size; i++) {
                int curr = q.front();
                q.pop();

                // 到达终点，返回当前步数
                if (curr == n - 1) {
                    return steps;
                }

                /*
                 * 跳跃方式 1：向右跳一格 → curr + 1
                 * 条件：不越界（curr + 1 < n）且未访问过
                 */
                if (curr + 1 < n && !visited[curr + 1]) {
                    visited[curr + 1] = true;
                    q.push(curr + 1);
                }

                /*
                 * 跳跃方式 2：向左跳一格 → curr - 1
                 * 条件：不越界（curr - 1 >= 0）且未访问过
                 */
                if (curr - 1 >= 0 && !visited[curr - 1]) {
                    visited[curr - 1] = true;
                    q.push(curr - 1);
                }

                /*
                 * 跳跃方式 3：跳到所有值相同的索引位置
                 *
                 * 从哈希表中取出 arr[curr] 对应的所有索引，逐个尝试跳转
                 * 跳转完成后，删除该值的映射（关键优化！）
                 *
                 * 为什么删除？
                 *   - 假设值 7 出现在索引 [0,1,2,3,4,6]
                 *   - 当我们从索引 0 处理「值 7」时，会把 1,2,3,4,6 全部入队
                 *   - 之后当 BFS 到达索引 1 时，如果再处理「值 7」，入队的节点早已被访问
                 *   - 所以每个值只需处理一次，之后删除即可
                 */
                if (valueToIndices.count(arr[curr])) {
                    for (int next : valueToIndices[arr[curr]]) {
                        if (!visited[next]) {
                            visited[next] = true;
                            q.push(next);
                        }
                    }
                    valueToIndices.erase(arr[curr]);  // 删除已处理的值，防止重复遍历
                }
            }

            steps++;  // 当前层处理完毕，步数 +1
        }

        // 理论上不会走到这里，因为题目保证一定有解
        return -1;
    }
};
