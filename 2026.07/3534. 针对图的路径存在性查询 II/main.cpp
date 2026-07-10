#include <bits/stdc++.h>
using namespace std;

#include "solution.cpp"

void printVector(const vector<int>& values) {
    cout << "[";
    for (int i = 0; i < (int)values.size(); ++i) {
        if (i > 0) {
            cout << ", ";
        }
        cout << values[i];
    }
    cout << "]";
}

void runCase(
    const string& caseName,
    int n,
    vector<int> nums,
    int maxDiff,
    vector<vector<int>> queries,
    vector<int> expected
) {
    Solution sol;

    cout << "==== " << caseName << " ====\n";
    cout << "n = " << n << "\n";
    cout << "nums = ";
    printVector(nums);
    cout << "\n";
    cout << "maxDiff = " << maxDiff << "\n";

    // 在下面这一行打断点，可以进入 solution.cpp 逐步调试。
    vector<int> actual = sol.pathExistenceQueries(n, nums, maxDiff, queries);

    cout << "actual   = ";
    printVector(actual);
    cout << "\n";

    cout << "expected = ";
    printVector(expected);
    cout << "\n";

    cout << (actual == expected ? "OK" : "FAIL") << "\n\n";
}

int main() {
    // 示例 1：
    // nums = [1, 8, 3, 4, 2], maxDiff = 3
    // 查询 [0,3] 和 [2,4] 都可以一条边直达。
    runCase(
        "Example 1",
        5,
        {1, 8, 3, 4, 2},
        3,
        {{0, 3}, {2, 4}},
        {1, 1}
    );

    // 示例 2：
    // [0,2] 需要走 0 -> 1 -> 2，距离为 2；
    // [2,3] 跨越两个连通块，不可达。
    runCase(
        "Example 2",
        5,
        {5, 3, 1, 9, 10},
        2,
        {{0, 1}, {0, 2}, {2, 3}, {4, 3}},
        {1, 2, -1, 1}
    );

    // 示例 3：
    // 没有任意两个不同节点能连边，所以只有自己到自己距离为 0。
    runCase(
        "Example 3",
        3,
        {3, 6, 1},
        1,
        {{0, 0}, {0, 1}, {1, 2}},
        {0, -1, -1}
    );

    // 补充例子 1：长链。
    // 每次只能从 value 0 -> 2 -> 4 -> 6 -> 8 -> 10，适合观察倍增跳跃。
    runCase(
        "Long Chain",
        6,
        {0, 2, 4, 6, 8, 10},
        2,
        {{0, 5}, {1, 4}, {2, 5}},
        {5, 3, 3}
    );

    // 补充例子 2：重复值。
    // maxDiff = 0 时，相同值之间仍然有边，所以不同节点之间距离为 1。
    runCase(
        "Duplicate Values",
        4,
        {7, 7, 7, 7},
        0,
        {{0, 3}, {1, 1}, {2, 0}},
        {1, 0, 1}
    );

    // 补充例子 3：完全不可达。
    // 相邻值差都大于 maxDiff，任何不同节点之间都没有路径。
    runCase(
        "Disconnected",
        5,
        {0, 10, 20, 30, 40},
        9,
        {{0, 4}, {0, 0}, {1, 2}},
        {-1, 0, -1}
    );

    // 补充例子 4：一步可以跨多个排序位置。
    // maxDiff = 8 时，0 可以直接到 8，所以 [0,2] 距离为 1。
    runCase(
        "Jump Over Multiple Values",
        5,
        {0, 4, 8, 12, 16},
        8,
        {{0, 4}, {0, 2}, {1, 4}},
        {2, 1, 2}
    );

    return 0;
}
