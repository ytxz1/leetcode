/*
 * LeetCode 1306. 跳跃游戏 III
 *
 * 题目描述：
 *   给定一个非负整数数组 arr，你最开始位于该数组的起始下标 start 处。
 *   当你位于下标 i 处时，你可以跳到 i + arr[i] 或者 i - arr[i]。
 *   请你判断自己是否能够跳到对应元素值为 0 的任一下标处。
 *   注意：不管什么情况下，你都无法跳到数组之外。
 *
 * ============================================================
 * 解题思路：
 * ============================================================
 *
 * 1. 问题本质：
 *    这是一个「图的可达性」问题。每个索引是图中的一个节点，
 *    从索引 i 可以跳到 i + arr[i] 和 i - arr[i] 两个节点。
 *    需要判断从 start 出发，能否到达任意一个值为 0 的节点。
 *
 * 2. 算法选择：BFS（广度优先搜索）
 *    - 也可以用 DFS，这里用 BFS 保持与跳跃游戏 IV 一致
 *    - 从 start 开始，逐层扩展所有可达的索引
 *    - 一旦发现某个索引的值为 0，返回 true
 *    - 队列为空仍未找到，返回 false
 *
 * 3. 关键点：
 *    - 用 visited 数组防止重复访问（避免死循环）
 *    - 每次跳转前检查是否越界
 *    - 只有两种跳转方式：i + arr[i] 和 i - arr[i]
 *
 * 4. 复杂度分析：
 *    - 时间复杂度：O(n)，每个索引最多被访问一次
 *    - 空间复杂度：O(n)，visited 数组和 BFS 队列各占 O(n)
 */

#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    bool canReach(vector<int>& arr, int start) {
        int n = arr.size();

        // 边界情况：如果起始位置就是 0，直接返回 true
        // 对应代码第 50 行
        if (arr[start] == 0) return true;

        /*
         * BFS 初始化
         *
         * 队列：存储待处理的索引，起始点为 start
         * visited：标记已访问的索引，防止重复访问造成死循环
         *
         * 对应代码第 57~59 行
         */
        queue<int> q;
        q.push(start);
        vector<bool> visited(n, false);
        visited[start] = true;

        /*
         * BFS 主循环
         *
         * 每次从队列中取出一个索引，尝试两种跳转：
         *   1. i + arr[i]（向右跳）
         *   2. i - arr[i]（向左跳）
         *
         * 如果跳转到的目标位置值为 0，返回 true
         * 否则将未访问过的目标位置加入队列
         *
         * 对应代码第 67~90 行
         */
        while (!q.empty()) {
            int curr = q.front();  // 取出当前索引
            q.pop();

            /*
             * 跳转方式 1：向右跳 → curr + arr[curr]
             * 条件：不越界（next < n）
             *
             * 对应代码第 76~83 行
             */
            int right = curr + arr[curr];
            if (right < n && !visited[right]) {
                // 到达值为 0 的位置，返回 true
                if (arr[right] == 0) return true;
                visited[right] = true;
                q.push(right);
            }

            /*
             * 跳转方式 2：向左跳 → curr - arr[curr]
             * 条件：不越界（next >= 0）
             *
             * 对应代码第 89~96 行
             */
            int left = curr - arr[curr];
            if (left >= 0 && !visited[left]) {
                // 到达值为 0 的位置，返回 true
                if (arr[left] == 0) return true;
                visited[left] = true;
                q.push(left);
            }
        }

        // 队列为空，所有可达位置都已访问，未找到值为 0 的位置
        return false;
    }
};
