#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int countIslands(vector<vector<int>>& grid, int k) {
        int m = (int)grid.size();
        int n = (int)grid[0].size();

        // visited[row][col] 表示格子 (row, col) 是否已经被访问过。
        // 只要一个陆地格子属于某个岛屿，它就会在遍历该岛屿时被标记为 true。
        vector<vector<bool>> visited(m, vector<bool>(n, false));

        // 四个方向：下、上、右、左。
        // 从一个陆地格子出发，只能通过这四个方向连接到同一个岛屿的其它陆地。
        vector<int> directions = {1, 0, -1, 0, 1};

        // answer 统计“岛屿总价值可以被 k 整除”的岛屿数量。
        int answer = 0;

        for (int startRow = 0; startRow < m; ++startRow) {
            for (int startCol = 0; startCol < n; ++startCol) {
                // 只有“没有访问过的正数格子”才是一个新岛屿的起点。
                // grid[startRow][startCol] == 0 表示水，不属于任何岛屿。
                if (grid[startRow][startCol] == 0 || visited[startRow][startCol]) {
                    continue;
                }

                // islandSum 是当前这个岛屿的总价值。
                // 题目中 m * n <= 1e5，grid[i][j] <= 1e6，
                // 总和最坏可到 1e11，因此必须用 long long。
                long long islandSum = 0;

                // stackCells 用来做迭代 DFS，避免递归深度过大导致栈溢出。
                stack<pair<int, int>> stackCells;
                stackCells.push({startRow, startCol});
                visited[startRow][startCol] = true;

                while (!stackCells.empty()) {
                    auto [row, col] = stackCells.top();
                    stackCells.pop();

                    // 当前格子是陆地，把它的价值加入当前岛屿总和。
                    islandSum += grid[row][col];

                    // 枚举四个相邻方向。
                    for (int dir = 0; dir < 4; ++dir) {
                        int nextRow = row + directions[dir];
                        int nextCol = col + directions[dir + 1];

                        // 越界的格子不能访问。
                        if (nextRow < 0 || nextRow >= m || nextCol < 0 || nextCol >= n) {
                            continue;
                        }

                        // 水格子不属于岛屿；已经访问过的格子也不需要重复入栈。
                        if (grid[nextRow][nextCol] == 0 || visited[nextRow][nextCol]) {
                            continue;
                        }

                        // 找到同一个岛屿中的新陆地格子。
                        visited[nextRow][nextCol] = true;
                        stackCells.push({nextRow, nextCol});
                    }
                }

                // 一个岛屿遍历完毕后，判断它的总价值是否能被 k 整除。
                if (islandSum % k == 0) {
                    ++answer;
                }
            }
        }

        return answer;
    }
};
