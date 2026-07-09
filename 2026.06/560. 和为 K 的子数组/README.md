# 560. 和为 K 的子数组

## 题目描述

给你一个整数数组 `nums` 和一个整数 `k`，请你统计并返回 **该数组中和为 `k` 的子数组的个数**。

**子数组**是数组中元素的**连续**非空序列。

## 示例

### 示例 1
```
输入：nums = [1,1,1], k = 2
输出：2
解释：和为 2 的子数组有 [1,1]（索引 0-1）和 [1,1]（索引 1-2）
```

### 示例 2
```
输入：nums = [1,2,3], k = 3
输出：2
解释：和为 3 的子数组有 [1,2]（索引 0-1）和 [3]（索引 2）
```

### 示例 3
```
输入：nums = [-1,-1,1], k = 0
输出：1
解释：和为 0 的子数组有 [-1,1]（索引 1-2）
```

## 概念解释

### 什么是子数组？

**子数组**（Subarray）是指原数组中**连续**的一段元素组成的数组。

- 子数组要求元素在原数组中是连续的
- 与**子序列**（Subsequence）不同，子序列不要求连续
- 长度为 n 的数组共有 **n × (n+1) / 2** 个非空子数组

### 什么是前缀和？

**前缀和**（Prefix Sum）是指从数组第一个元素到当前元素的所有元素之和。

```
nums =       [1,  2,  3]
前缀和 = [1, 3,  6]   （包含当前元素）
```

前缀和有一个重要性质：
- **子数组 `nums[j..i]` 的和 = `prefixSum[i] - prefixSum[j-1]`**（其中 `prefixSum[-1] = 0`）

利用这个性质，判断「是否存在和为 k 的子数组」就转化为判断「是否存在两个前缀和之差等于 k」。

## 解题思路

### 核心思想：前缀和 + 哈希表

这是解决「连续子数组和」问题的经典套路，核心公式：

```
子数组 nums[j..i] 的和 = prefixSum[i] - prefixSum[j-1]
                            ↓
         prefixSum[i] - prefixSum[j-1] = k
                            ↓
         prefixSum[j-1] = prefixSum[i] - k
```

**算法流程**：

1. 遍历数组，计算当前位置的前缀和 `currentSum`
2. 在哈希表中查找 `currentSum - k` 出现的次数 —— 这个次数就是以当前位置结尾的、和为 k 的子数组个数
3. 将当前前缀和 `currentSum` 加入哈希表（记录出现次数）
4. 继续遍历下一个元素

**关键细节**：初始时哈希表中应包含 `{0: 1}`，表示前缀和为 0 出现了 1 次（即空数组的前缀和）。这样当 `currentSum == k` 时，`currentSum - k == 0` 就能匹配到。

### 为什么哈希表能快速统计？

常规做法是：枚举每个子数组的起点 `j`，计算 `nums[j..i]` 的和是否等于 k。这需要 O(N²) 时间。

而通过前缀和，我们不需要枚举起点 —— 对每个终点 `i`，哈希表直接告诉我们「有多少个起点 `j` 使得 `nums[j..i]` 的和等于 k」。这个信息就等价于「有多少个前缀和等于 `currentSum - k`」。

### 图解示例

以 `nums = [1, 2, 3], k = 3` 为例：

```
遍历过程:
i=0, nums[0]=1
  currentSum = 0 + 1 = 1
  查找 currentSum - k = 1 - 3 = -2 → 哈希表中没有 → count = 0
  记录前缀和: map = {0:1, 1:1}

i=1, nums[1]=2
  currentSum = 1 + 2 = 3
  查找 currentSum - k = 3 - 3 = 0 → 哈希表中有 1 个 → count = 1（对应子数组 [1,2]）
  记录前缀和: map = {0:1, 1:1, 3:1}

i=2, nums[2]=3
  currentSum = 3 + 3 = 6
  查找 currentSum - k = 6 - 3 = 3 → 哈希表中有 1 个 → count = 2（对应子数组 [3]）
  记录前缀和: map = {0:1, 1:1, 3:1, 6:1}

结果: count = 2
```

## 代码实现

### 方法一：前缀和 + 哈希表（最优解）

**思路**：利用哈希表记录每个前缀和出现的次数，在遍历过程中快速统计以当前元素结尾的、和为 k 的子数组个数。

```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<int, int> prefixSumCount;
        prefixSumCount[0] = 1;  // 前缀和为0出现1次（空数组）

        int currentSum = 0;
        int count = 0;

        for (int num : nums) {
            currentSum += num;  // 计算当前位置的前缀和

            // 查找是否存在前缀和 = currentSum - k
            // 如果存在，说明有子数组的和为 k
            if (prefixSumCount.find(currentSum - k) != prefixSumCount.end()) {
                count += prefixSumCount[currentSum - k];
            }

            // 将当前前缀和加入哈希表
            prefixSumCount[currentSum]++;
        }

        return count;
    }
};
```

**执行过程图解**：

```
nums = [1, 1, 1], k = 2

初始化: prefixSumCount = {0: 1}, currentSum = 0, count = 0

i=0, num=1:
  currentSum = 0 + 1 = 1
  currentSum - k = 1 - 2 = -1 → 不在哈希表中
  记录: prefixSumCount = {0:1, 1:1}

i=1, num=1:
  currentSum = 1 + 1 = 2
  currentSum - k = 2 - 2 = 0 → ⭐ 找到! count += 1 → count = 1
  记录: prefixSumCount = {0:1, 1:1, 2:1}

i=2, num=1:
  currentSum = 2 + 1 = 3
  currentSum - k = 3 - 2 = 1 → ⭐ 找到! count += 1 → count = 2
  记录: prefixSumCount = {0:1, 1:1, 2:1, 3:1}

结果: count = 2 ✅
```

### 方法二：暴力枚举

**思路**：枚举每个子数组的起点和终点，计算子数组的和。

```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int count = 0;
        int n = nums.size();

        for (int start = 0; start < n; start++) {
            int sum = 0;
            for (int end = start; end < n; end++) {
                sum += nums[end];     // 以 start 为起点，逐步扩展终点
                if (sum == k) count++;
            }
        }

        return count;
    }
};
```

**优化说明**：内层循环不需要每次都重新计算和，而是在扩展终点的过程中累加，这样每个子数组的和只需 O(1) 时间。

### 方法三：前缀和数组 + 两层遍历

**思路**：先计算出所有前缀和，然后枚举起点和终点，利用前缀和相减得到子数组和。

```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> prefixSum(n + 1, 0);  // prefixSum[i] 表示前 i 个元素的和

        // 计算前缀和
        for (int i = 0; i < n; i++) {
            prefixSum[i + 1] = prefixSum[i] + nums[i];
        }

        int count = 0;
        // 枚举子数组起点和终点
        for (int start = 0; start < n; start++) {
            for (int end = start; end < n; end++) {
                // 子数组 nums[start..end] 的和 = prefixSum[end+1] - prefixSum[start]
                if (prefixSum[end + 1] - prefixSum[start] == k) {
                    count++;
                }
            }
        }

        return count;
    }
};
```

## 复杂度分析

| 方法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 前缀和+哈希表 | O(N) | O(N) | 最优解，一次遍历 |
| 暴力枚举 | O(N²) | O(1) | 最简单直观 |
| 前缀和数组+遍历 | O(N²) | O(N) | 中间方案 |

## 方法一详解：前缀和 + 哈希表

### 为什么初始 map 中要有 {0: 1}？

考虑这样的场景：
```
nums = [1, 2, 3], k = 3
- 当 i = 1 时，currentSum = 3，currentSum - k = 0
- 我们需要匹配到空数组的前缀和 0，才能正确统计子数组 [1, 2]
```
初始放入 {0: 1} 确保了前缀和本身就等于 k 的情况能被正确统计。

### 为什么先查找再记录？

**必须"先查找、再记录当前前缀和"**，顺序不能颠倒。

原因：如果先记录当前前缀和再查找，当 k = 0 时，`currentSum - k = currentSum`，会错误地将当前子数组（长度为 0）也算进去。

举例：`nums = [1], k = 0`
- 正确的做法：`currentSum = 1`, 查找 `1 - 0 = 1` → 哈希表中只有 {0:1} → 没有匹配 → 正确结果 0
- 错误的做法：先记录 `{0:1, 1:1}`，再查找 `1 - 0 = 1` → 找到 `当前元素自身` → 错误结果 1

### 代码模板

```cpp
unordered_map<int, int> map;
map[0] = 1;
int sum = 0, count = 0;
for (int num : nums) {
    sum += num;
    if (map.find(sum - k) != map.end()) {
        count += map[sum - k];
    }
    map[sum]++;
}
```

## 扩展：前缀和问题的通用套路

前缀和 + 哈希表是解决**连续子数组**问题的经典套路，适用于以下场景：

| 问题 | 哈希表记录什么 |
|------|--------------|
| 和为 k 的子数组个数 | 前缀和 → 出现次数 |
| 和能被 k 整除的子数组个数 | 前缀和模 k → 出现次数 |
| 和为 0 的子数组 | 前缀和 → 首次出现位置 |
| 和为 target 的最长子数组长度 | 前缀和 → 最早出现位置 |

## 易错点

1. **`k` 可能为负数** —— 但当 k 为负数时代码仍然适用，因为我们在哈希表中查找的是 `currentSum - k`（等于 currentSum + |k|）
2. **`nums` 中的数字可以是负数** —— 这意味着前缀和不一定是递增的，但哈希表方法仍然有效
3. **子数组要求连续** —— 不能跳过元素，否则就是子序列问题
4. **结果可能很大** —— 题目保证结果在 32 位整数范围内

## 测试用例

```cpp
// 测试用例1
nums = [1,1,1], k = 2
// 输出: 2

// 测试用例2
nums = [1,2,3], k = 3
// 输出: 2

// 测试用例3
nums = [-1,-1,1], k = 0
// 输出: 1

// 测试用例4（边界情况）
nums = [1], k = 0
// 输出: 0

// 测试用例5（包含负数）
nums = [-1,1,0], k = 0
// 输出: 3（[-1,1], [0], [-1,1,0]）
```

## 总结

和为 K 的子数组问题的核心是**前缀和 + 哈希表**：

1. **暴力法** O(N²)：枚举所有子数组，直观但效率低
2. **前缀和数组** O(N²)：利用前缀和快速计算子数组和，但仍然是两层循环
3. **前缀和 + 哈希表** O(N) ✅：一次遍历，将时间复杂度降到线性

关键在于将「子数组和等于 k」转化为「两个前缀和之差等于 k」，然后用哈希表实现 O(1) 查找。这是**空间换时间**的经典应用，也是解决连续子数组相关问题的通用方法论。
