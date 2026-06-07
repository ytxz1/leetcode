#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

/*
 * 3635. 最早完成陆地和水上游乐设施的时间 II
 *
 * 题目：给定陆地游乐设施和水上游乐设施的开始时间和持续时间，
 *       需要选择一个陆地设施和一个水上设施，两个设施不能重叠（可以任意顺序），
 *       返回完成两个设施的最早时间。
 *
 * 输入：
 *   - landStartTime[i]: 第i个陆地设施的开始时间
 *   - landDuration[i]: 第i个陆地设施的持续时间
 *   - waterStartTime[j]: 第j个水上设施的开始时间
 *   - waterDuration[j]: 第j个水上设施的持续时间
 *
 * 输出：完成一个陆地设施和一个水上设施的最早结束时间
 *
 * 关键点：
 *   1. 两个设施不能同时进行
 *   2. 可以先玩陆地再玩水上，也可以先玩水上再玩陆地
 *   3. 选择最优的设施组合和顺序
 */

// ============================================================================
// 方法一：暴力法
// ============================================================================
// 思路：枚举所有可能的组合和顺序，取最小值
// 时间复杂度: O(N * M)，N=陆地设施数，M=水上设施数
// 空间复杂度: O(1)
class Solution1 {
public:
    int earliestFinishTime(vector<int>& landStartTime, vector<int>& landDuration,
                          vector<int>& waterStartTime, vector<int>& waterDuration) {
        int n = landStartTime.size();
        int m = waterStartTime.size();
        int ans = INT_MAX;

        // 情况1：先玩陆地，再玩水上
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // 陆地设施的结束时间
                int landEnd = landStartTime[i] + landDuration[i];
                // 水上设施的开始时间 = max(陆地结束时间, 水上设施开始时间)
                int waterStart = max(landEnd, waterStartTime[j]);
                // 总结束时间
                int finishTime = waterStart + waterDuration[j];
                ans = min(ans, finishTime);
            }
        }

        // 情况2：先玩水上，再玩陆地
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n; i++) {
                // 水上设施的结束时间
                int waterEnd = waterStartTime[j] + waterDuration[j];
                // 陆地设施的开始时间 = max(水上结束时间, 陆地设施开始时间)
                int landStart = max(waterEnd, landStartTime[i]);
                // 总结束时间
                int finishTime = landStart + landDuration[i];
                ans = min(ans, finishTime);
            }
        }

        return ans;
    }
};

// ============================================================================
// 方法二：贪心 + 排序 + 后缀预处理
// ============================================================================
// 思路：
// 1. 预处理：计算每个设施的最早结束时间（如果作为第一个设施）
// 2. 对第二个设施按开始时间排序
// 3. 预处理后缀最小结束时间
// 4. 对于每个第一个设施，二分查找最优的第二个设施
//
// 时间复杂度: O(N log N + M log M)
// 空间复杂度: O(N + M)
class Solution2 {
public:
    int earliestFinishTime(vector<int>& landStartTime, vector<int>& landDuration,
                          vector<int>& waterStartTime, vector<int>& waterDuration) {
        int n = landStartTime.size();
        int m = waterStartTime.size();

        // 预处理：计算每个设施作为第一个设施时的结束时间
        vector<pair<int, int>> landEnd;  // {结束时间, 开始时间}
        vector<pair<int, int>> waterEnd;

        for (int i = 0; i < n; i++) {
            landEnd.push_back({landStartTime[i] + landDuration[i], landStartTime[i]});
        }
        for (int j = 0; j < m; j++) {
            waterEnd.push_back({waterStartTime[j] + waterDuration[j], waterStartTime[j]});
        }

        // 按开始时间排序
        sort(landEnd.begin(), landEnd.end());
        sort(waterEnd.begin(), waterEnd.end());

        // 预处理后缀最小结束时间
        vector<int> landSuffixMin(n), waterSuffixMin(m);
        landSuffixMin[n-1] = landEnd[n-1].first;
        for (int i = n-2; i >= 0; i--) {
            landSuffixMin[i] = min(landEnd[i].first, landSuffixMin[i+1]);
        }
        waterSuffixMin[m-1] = waterEnd[m-1].first;
        for (int j = m-2; j >= 0; j--) {
            waterSuffixMin[j] = min(waterEnd[j].first, waterSuffixMin[j+1]);
        }

        int ans = INT_MAX;

        // 情况1：先玩陆地，再玩水上
        for (int i = 0; i < n; i++) {
            int landEndTime = landEnd[i].first;
            // 二分查找第一个开始时间 >= landEndTime 的水上设施
            int left = 0, right = m - 1;
            int idx = m;  // 默认没有满足条件的
            while (left <= right) {
                int mid = (left + right) / 2;
                if (waterEnd[mid].second >= landEndTime) {
                    idx = mid;
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            // 如果找到了，更新答案
            if (idx < m) {
                ans = min(ans, waterSuffixMin[idx]);
            }
        }

        // 情况2：先玩水上，再玩陆地
        for (int j = 0; j < m; j++) {
            int waterEndTime = waterEnd[j].first;
            // 二分查找第一个开始时间 >= waterEndTime 的陆地设施
            int left = 0, right = n - 1;
            int idx = n;
            while (left <= right) {
                int mid = (left + right) / 2;
                if (landEnd[mid].second >= waterEndTime) {
                    idx = mid;
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            if (idx < n) {
                ans = min(ans, landSuffixMin[idx]);
            }
        }

        return ans;
    }
};

// ============================================================================
// 方法三：贪心 + 双指针
// ============================================================================
// 思路：
// 1. 预处理每个设施的最早结束时间
// 2. 分别对两种顺序（陆地→水上，水上→陆地）计算最优解
// 3. 对于每种顺序，使用贪心策略选择最优组合
//
// 时间复杂度: O(N log N + M log M)
// 空间复杂度: O(N + M)
class Solution3 {
public:
    int earliestFinishTime(vector<int>& landStartTime, vector<int>& landDuration,
                          vector<int>& waterStartTime, vector<int>& waterDuration) {
        int n = landStartTime.size();
        int m = waterStartTime.size();

        // 计算每个设施的结束时间
        vector<int> landEndTime(n), waterEndTime(m);
        for (int i = 0; i < n; i++) {
            landEndTime[i] = landStartTime[i] + landDuration[i];
        }
        for (int j = 0; j < m; j++) {
            waterEndTime[j] = waterStartTime[j] + waterDuration[j];
        }

        int ans = INT_MAX;

        // 情况1：先玩陆地，再玩水上
        // 对于每个陆地设施，找到能最早完成的水上设施
        for (int i = 0; i < n; i++) {
            int finishTime1 = landEndTime[i];  // 陆地设施结束时间
            for (int j = 0; j < m; j++) {
                // 水上设施开始时间 = max(陆地结束时间, 水上设施开始时间)
                int startTime2 = max(finishTime1, waterStartTime[j]);
                int finishTime2 = startTime2 + waterDuration[j];
                ans = min(ans, finishTime2);
            }
        }

        // 情况2：先玩水上，再玩陆地
        for (int j = 0; j < m; j++) {
            int finishTime1 = waterEndTime[j];  // 水上设施结束时间
            for (int i = 0; i < n; i++) {
                // 陆地设施开始时间 = max(水上结束时间, 陆地设施开始时间)
                int startTime2 = max(finishTime1, landStartTime[i]);
                int finishTime2 = startTime2 + landDuration[i];
                ans = min(ans, finishTime2);
            }
        }

        return ans;
    }
};

int main() {
    Solution1 sol1;
    Solution2 sol2;
    Solution3 sol3;

    // 测试用例1
    vector<int> landStart1 = {2, 8};
    vector<int> landDuration1 = {4, 1};
    vector<int> waterStart1 = {6};
    vector<int> waterDuration1 = {3};
    cout << "测试用例1:" << endl;
    cout << "陆地设施: 开始=[2,8], 持续=[4,1]" << endl;
    cout << "水上设施: 开始=[6], 持续=[3]" << endl;
    cout << "方法一: " << sol1.earliestFinishTime(landStart1, landDuration1, waterStart1, waterDuration1) << endl;
    cout << "方法二: " << sol2.earliestFinishTime(landStart1, landDuration1, waterStart1, waterDuration1) << endl;
    cout << "方法三: " << sol3.earliestFinishTime(landStart1, landDuration1, waterStart1, waterDuration1) << endl;
    cout << endl;

    // 测试用例2
    vector<int> landStart2 = {1, 5};
    vector<int> landDuration2 = {3, 2};
    vector<int> waterStart2 = {2, 4};
    vector<int> waterDuration2 = {3, 1};
    cout << "测试用例2:" << endl;
    cout << "陆地设施: 开始=[1,5], 持续=[3,2]" << endl;
    cout << "水上设施: 开始=[2,4], 持续=[3,1]" << endl;
    cout << "方法一: " << sol1.earliestFinishTime(landStart2, landDuration2, waterStart2, waterDuration2) << endl;
    cout << "方法二: " << sol2.earliestFinishTime(landStart2, landDuration2, waterStart2, waterDuration2) << endl;
    cout << "方法三: " << sol3.earliestFinishTime(landStart2, landDuration2, waterStart2, waterDuration2) << endl;

    return 0;
}
