# 3635. 最早完成陆地和水上游乐设施的时间 II

## 题目描述

给你两个整数数组 `landStartTime` 和 `landDuration`，分别表示每个陆地游乐设施的**开始时间**和**持续时间**。

给你另外两个整数数组 `waterStartTime` 和 `waterDuration`，分别表示每个水上游乐设施的**开始时间**和**持续时间**。

你需要选择**一个陆地游乐设施**和**一个水上游乐设施**，完成这两个游乐设施。

### 规则

1. 对于每个游乐设施，你必须在它的**开始时间之后（含开始时间）**才能开始游玩
2. 一个游乐设施的**结束时间 = 开始游玩时间 + 设施持续时间**
3. 你需要完成一个陆地设施和一个水上设施，且**两个设施不能同时进行**
4. **可以任意顺序**：先玩陆地再玩水上，或者先玩水上再玩陆地

### 目标

返回完成这两个游乐设施的**最早结束时间**。

## 示例

### 示例 1

```
输入:
landStartTime = [2, 8], landDuration = [4, 1]
waterStartTime = [6], waterDuration = [3]

输出: 9

解释:
方案1：先玩陆地设施0（开始时间2，持续4，结束时间6），然后水上设施0（开始时间6，持续3，结束时间9）
方案2：先玩陆地设施1（开始时间8，持续1，结束时间9），然后水上设施0（开始时间9，持续3，结束时间12）
方案3：先玩水上设施0（开始时间6，持续3，结束时间9），然后陆地设施1（开始时间9，持续1，结束时间10）

最早结束时间是 9
```

### 示例 2

```
输入:
landStartTime = [1, 5], landDuration = [3, 2]
waterStartTime = [2, 4], waterDuration = [3, 1]

输出: 5

解释:
先玩水上设施1（开始时间4，持续1，结束时间5），然后陆地设施1（开始时间5，持续2，结束时间7）
或者先玩陆地设施0（开始时间1，持续3，结束时间4），然后水上设施1（开始时间4，持续1，结束时间5）
```

## 解题思路

### 核心问题

需要选择：
1. 一个陆地设施（索引 i）
2. 一个水上设施（索引 j）
3. 决定顺序（先陆地后水上，或先水上后陆地）

使得总结束时间最小。

### 关键观察

对于任意设施，如果它作为第一个设施，它的结束时间是固定的：
```
结束时间 = max(当前时间, 设施开始时间) + 设施持续时间
```

但如果当前时间是 0（刚开始），则：
```
结束时间 = 设施开始时间 + 设施持续时间
```

### 算法思路

#### 思路一：暴力法

枚举所有可能的组合和顺序：
- 情况1：先玩陆地 i，再玩水上 j
  - 陆地结束时间 = landStartTime[i] + landDuration[i]
  - 水上开始时间 = max(陆地结束时间, waterStartTime[j])
  - 总结束时间 = 水上开始时间 + waterDuration[j]

- 情况2：先玩水上 j，再玩陆地 i
  - 水上结束时间 = waterStartTime[j] + waterDuration[j]
  - 陆地开始时间 = max(水上结束时间, landStartTime[i])
  - 总结束时间 = 陆地开始时间 + landDuration[i]

取所有情况的最小值。

**时间复杂度：** O(N × M)

#### 思路二：贪心 + 排序 + 后缀预处理

1. **预处理**：计算每个设施的结束时间（如果作为第一个设施）
2. **排序**：对设施按开始时间排序
3. **后缀预处理**：计算从每个位置开始到末尾的最小结束时间
4. **二分查找**：对于每个第一个设施，二分查找满足条件的第二个设施

**时间复杂度：** O(N log N + M log M)

#### 思路三：贪心优化

观察到对于固定的第一个设施，最优的第二个设施是：
- 开始时间 >= 第一个设施结束时间的设施中，结束时间最小的

可以对每个设施预处理：
- 如果作为第一个设施，它的结束时间
- 如果作为第二个设施，在给定开始时间约束下的最早结束时间

## 代码实现

### 方法一：暴力法

```cpp
int earliestFinishTime(vector<int>& landStartTime, vector<int>& landDuration,
                      vector<int>& waterStartTime, vector<int>& waterDuration) {
    int n = landStartTime.size();
    int m = waterStartTime.size();
    int ans = INT_MAX;

    // 情况1：先玩陆地，再玩水上
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int landEnd = landStartTime[i] + landDuration[i];
            int waterStart = max(landEnd, waterStartTime[j]);
            int finishTime = waterStart + waterDuration[j];
            ans = min(ans, finishTime);
        }
    }

    // 情况2：先玩水上，再玩陆地
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            int waterEnd = waterStartTime[j] + waterDuration[j];
            int landStart = max(waterEnd, landStartTime[i]);
            int finishTime = landStart + landDuration[i];
            ans = min(ans, finishTime);
        }
    }

    return ans;
}
```

### 方法二：贪心 + 排序 + 后缀预处理

```cpp
int earliestFinishTime(vector<int>& landStartTime, vector<int>& landDuration,
                      vector<int>& waterStartTime, vector<int>& waterDuration) {
    int n = landStartTime.size();
    int m = waterStartTime.size();

    // 预处理：计算每个设施的结束时间
    vector<pair<int, int>> landEnd, waterEnd;
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
        int idx = m;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (waterEnd[mid].second >= landEndTime) {
                idx = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        if (idx < m) {
            ans = min(ans, waterSuffixMin[idx]);
        }
    }

    // 情况2：先玩水上，再玩陆地
    for (int j = 0; j < m; j++) {
        int waterEndTime = waterEnd[j].first;
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
```

## 复杂度分析

| 方法 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|-----------|-----------|---------|
| 暴力法 | O(N × M) | O(1) | 数据规模较小 |
| 贪心+排序+二分 | O(N log N + M log M) | O(N + M) | 数据规模较大 |

## 测试用例

```cpp
// 测试用例1
landStartTime = [2, 8], landDuration = [4, 1]
waterStartTime = [6], waterDuration = [3]
// 输出: 9

// 测试用例2
landStartTime = [1, 5], landDuration = [3, 2]
waterStartTime = [2, 4], waterDuration = [3, 1]
// 输出: 5
```

## 总结

本题的关键是：
1. **枚举顺序**：需要考虑两种顺序（陆地→水上，水上→陆地）
2. **贪心选择**：对于固定的第一个设施，选择能最早完成的第二个设施
3. **预处理优化**：通过排序和后缀预处理，将 O(N × M) 优化到 O(N log N + M log M)

对于小规模数据，暴力法足够高效；对于大规模数据，贪心 + 排序 + 二分是更好的选择。
