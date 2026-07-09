#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<bool> pathExistenceQueries(int n, vector<int>& nums, int maxDiff, vector<vector<int>>& queries) {
        // componentId[i] 表示节点 i 所在的连通块编号。
        //
        // 因为 nums 已经按非递减顺序排序，所以图的连通块一定是一段一段连续的下标区间。
        // 只要相邻两个点 i - 1 和 i 满足 nums[i] - nums[i - 1] <= maxDiff，
        // 它们之间就有边，这两个点就属于同一个连通块。
        //
        // 如果 nums[i] - nums[i - 1] > maxDiff，
        // 那么左边所有点的值都 <= nums[i - 1]，右边所有点的值都 >= nums[i]，
        // 跨过这条缝的任意两点差值都会 > maxDiff，不可能有边，也不可能有路径。
        vector<int> componentId(n, 0);

        // currentComponent 是当前扫描到的位置所属的连通块编号。
        // 从节点 0 开始，初始属于第 0 个连通块。
        int currentComponent = 0;

        for (int i = 1; i < n; ++i) {
            // gap 是相邻两个 nums 值的差。
            // 由于 nums 非递减，所以 abs(nums[i] - nums[i - 1]) 可以直接写成这个差值。
            int gap = nums[i] - nums[i - 1];

            // 相邻差值超过 maxDiff，说明 i - 1 和 i 之间没有边，
            // 并且这里会把图切成两个不同的连通块。
            if (gap > maxDiff) {
                ++currentComponent;
            }

            // 记录节点 i 所属的连通块编号。
            componentId[i] = currentComponent;
        }

        vector<bool> answer;
        answer.reserve(queries.size());

        for (const auto& query : queries) {
            int u = query[0];
            int v = query[1];

            // 两个节点在同一个连通块中，当且仅当它们之间存在路径。
            answer.push_back(componentId[u] == componentId[v]);
        }

        return answer;
    }
};
