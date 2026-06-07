# 46. 全排列

## 题目描述

给定一个**不含重复数字**的数组 `nums`，返回其所有可能的**全排列**。你可以按**任意顺序**返回答案。

## 示例

### 示例 1
```
输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
```

### 示例 2
```
输入：nums = [0,1]
输出：[[0,1],[1,0]]
```

### 示例 3
```
输入：nums = [1]
输出：[[1]]
```

## 概念解释

### 什么是全排列？

**全排列**是指从 n 个不同元素中取出 n 个元素，按照一定的顺序排成一列。

- n 个元素的全排列共有 **n!** 种
- 例如：`[1,2,3]` 的全排列有 3! = 6 种

### 全排列的生成过程

以 `[1,2,3]` 为例，全排列的生成过程可以表示为一棵**排列树**：

```
                        []
                 /       |       \
              [1]       [2]       [3]
             /   \     /   \     /   \
          [1,2] [1,3] [2,1] [2,3] [3,1] [3,2]
            |     |     |     |     |     |
        [1,2,3][1,3,2][2,1,3][2,3,1][3,1,2][3,2,1]
```

## 解题思路

### 核心思想：回溯法

回溯法是解决全排列问题的经典方法。核心思想是：

1. **逐位构建**：从左到右，逐个位置填入数字
2. **选择与撤销**：每次选择一个未使用的数字填入当前位置，递归处理下一位，然后撤销选择
3. **终止条件**：当所有位置都填满时，得到一个完整排列

### 回溯法模板

```python
def backtrack(路径, 选择列表):
    if 满足结束条件:
        result.add(路径)
        return

    for 选择 in 选择列表:
        做选择
        backtrack(路径, 选择列表)
        撤销选择
```

## 代码实现

### 方法一：回溯法（标准模板）

**思路**：使用 `used` 数组标记哪些数字已使用，每次从剩余未使用的数字中选择一个。

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;
        vector<bool> used(nums.size(), false);

        backtrack(nums, current, used, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, vector<int>& current,
                   vector<bool>& used, vector<vector<int>>& result) {
        // 终止条件：排列构建完成
        if (current.size() == nums.size()) {
            result.push_back(current);
            return;
        }

        // 遍历所有数字
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;  // 跳过已使用的数字

            // 做选择
            current.push_back(nums[i]);
            used[i] = true;

            // 递归
            backtrack(nums, current, used, result);

            // 撤销选择（回溯）
            current.pop_back();
            used[i] = false;
        }
    }
};
```

**执行过程图解**：

以 `nums = [1,2,3]` 为例：

```
第1层：选择第1个数字
  选择1: current=[1], used=[T,F,F]
  选择2: current=[2], used=[F,T,F]
  选择3: current=[3], used=[F,F,T]

第2层：选择第2个数字（以选择1为例）
  选择2: current=[1,2], used=[T,T,F]
  选择3: current=[1,3], used=[T,F,T]

第3层：选择第3个数字（以选择1→2为例）
  选择3: current=[1,2,3], used=[T,T,T] → 找到一个排列！

回溯：撤销选择3，回到 [1,2]
回溯：撤销选择2，回到 [1]
...
```

### 方法二：交换法（原地交换）

**思路**：固定当前位置，将后面的每个数字交换到当前位置，然后递归处理剩余位置。

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;
        backtrack(nums, 0, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, int start, vector<vector<int>>& result) {
        // 终止条件
        if (start == nums.size()) {
            result.push_back(nums);
            return;
        }

        // 将每个数字交换到 start 位置
        for (int i = start; i < nums.size(); i++) {
            swap(nums[start], nums[i]);       // 做选择
            backtrack(nums, start + 1, result); // 递归
            swap(nums[start], nums[i]);       // 撤销选择
        }
    }
};
```

**执行过程图解**：

以 `nums = [1,2,3]` 为例：

```
初始: [1,2,3]

start=0: 将第0位分别换成 1,2,3
  交换(0,0): [1,2,3] → 递归 start=1
    交换(1,1): [1,2,3] → 递归 start=2
      交换(2,2): [1,2,3] → 找到排列！
    交换(1,2): [1,3,2] → 递归 start=2
      交换(2,2): [1,3,2] → 找到排列！
  交换(0,1): [2,1,3] → 递归 start=1
    ...
  交换(0,2): [3,2,1] → 递归 start=1
    ...
```

### 方法三：使用 STL 的 next_permutation

**思路**：利用 C++ 标准库函数直接生成下一个排列。

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;

        // 先排序，从最小排列开始
        sort(nums.begin(), nums.end());

        // 生成所有排列
        do {
            result.push_back(nums);
        } while (next_permutation(nums.begin(), nums.end()));

        return result;
    }
};
```

**next_permutation 原理**：
1. 从右向左找到第一个相邻升序对 `(i, i+1)`
2. 从右向左找到第一个比 `nums[i]` 大的数 `nums[j]`
3. 交换 `nums[i]` 和 `nums[j]`
4. 反转 `i+1` 到末尾的部分

### 方法四：DFS 深度优先搜索

**思路**：与回溯法类似，使用 DFS 遍历排列树。

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        dfs(nums, path, used, result);
        return result;
    }

private:
    void dfs(vector<int>& nums, vector<int>& path, vector<bool>& used,
             vector<vector<int>>& result) {
        if (path.size() == nums.size()) {
            result.push_back(path);
            return;
        }

        for (int i = 0; i < nums.size(); i++) {
            if (!used[i]) {
                path.push_back(nums[i]);
                used[i] = true;
                dfs(nums, path, used, result);
                path.pop_back();
                used[i] = false;
            }
        }
    }
};
```

## 复杂度分析

| 方法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 回溯法 | O(N! × N) | O(N) | 最常用，易于理解 |
| 交换法 | O(N! × N) | O(N) | 原地操作，无需额外空间 |
| next_permutation | O(N! × N) | O(1) | 使用 STL，代码简洁 |
| DFS | O(N! × N) | O(N) | 与回溯法本质相同 |

**说明**：
- N! 是排列的数量
- 每个排列需要 O(N) 时间构造
- 空间复杂度不计结果空间

## 回溯法核心要点

### 1. 选择与撤销

```cpp
// 做选择
current.push_back(nums[i]);
used[i] = true;

// 递归
backtrack(nums, current, used, result);

// 撤销选择（必须与做选择的顺序相反）
current.pop_back();
used[i] = false;
```

### 2. 剪枝条件

```cpp
if (used[i]) continue;  // 跳过已使用的数字
```

### 3. 终止条件

```cpp
if (current.size() == nums.size()) {
    result.push_back(current);
    return;
}
```

## 与其他题目的关系

| 题目 | 区别 |
|------|------|
| 46. 全排列 | 无重复数字 |
| 47. 全排列 II | 有重复数字，需要去重 |
| 31. 下一个排列 | 只求下一个排列 |
| 60. 排列序列 | 求第 k 个排列 |

## 测试用例

```cpp
// 测试用例1
nums = [1,2,3]
// 输出: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]

// 测试用例2
nums = [0,1]
// 输出: [[0,1],[1,0]]

// 测试用例3
nums = [1]
// 输出: [[1]]
```

## 总结

全排列问题是回溯法的经典应用：

1. **回溯法**是最常用的解法，使用 `used` 数组标记已使用的数字
2. **交换法**通过原地交换避免额外空间，但需要理解交换的含义
3. **next_permutation** 是 C++ STL 提供的便捷方法
4. **DFS** 与回溯法本质相同，只是表述方式不同

掌握回溯法的模板是解决排列、组合、子集等问题的关键。
