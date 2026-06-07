# 3121. 统计特殊字母的数量 II

## 题目描述

给你一个字符串 `word`。如果 `word` 中同时出现某个字母 `c` 的小写形式和大写形式，并且 **每个** 小写形式的 `c` 都出现在第一个大写形式的 `c` 之前，则称字母 `c` 是一个 **特殊字母**。

返回 `word` 中 **特殊字母** 的数量。

## 示例

### 示例 1
```
输入: word = "aaAbcBC"
输出: 3
解释:
- 字母 'a' 是特殊字母，因为所有 'a'(位置0,1) 都在第一个 'A'(位置2) 之前
- 字母 'b' 是特殊字母，因为所有 'b'(位置3) 都在第一个 'B'(位置5) 之前
- 字母 'c' 是特殊字母，因为所有 'c'(位置4) 都在第一个 'C'(位置6) 之前
```

### 示例 2
```
输入: word = "abc"
输出: 0
解释: 没有同时出现小写和大写的字母
```

### 示例 3
```
输入: word = "AbBCab"
输出: 0
解释: 没有满足条件的特殊字母
```

## 解题思路

### 核心条件
一个字母 `c` 是特殊字母需要满足：
1. **同时存在**：小写 `c` 和大写 `C` 都必须出现
2. **顺序约束**：所有小写 `c` 的位置都必须在第一个大写 `C` 的位置之前

### 关键观察
- 只需要记录每个字母的 **最后出现的小写位置** 和 **首次出现的大写位置**
- 如果 `最后小写位置 < 首次大写位置`，则该字母是特殊字母

---

## 方法一：哈希表记录位置

### 思路
使用两个数组分别记录：
- `lastLower[i]`：字母 `i` 最后一次出现小写的位置
- `firstUpper[i]`：字母 `i` 第一次出现大写的位置

遍历字符串，更新这两个数组，最后检查条件。

### 代码
```cpp
class Solution {
public:
    int numberOfSpecialChars(string word) {
        vector<int> firstUpper(26, -1);  // 首次大写位置
        vector<int> lastLower(26, -1);   // 最后小写位置

        for (int i = 0; i < word.size(); i++) {
            char c = word[i];
            if (c >= 'a' && c <= 'z') {
                lastLower[c - 'a'] = i;
            } else if (c >= 'A' && c <= 'Z') {
                if (firstUpper[c - 'A'] == -1) {
                    firstUpper[c - 'A'] = i;
                }
            }
        }

        int count = 0;
        for (int i = 0; i < 26; i++) {
            if (lastLower[i] != -1 && firstUpper[i] != -1 &&
                lastLower[i] < firstUpper[i]) {
                count++;
            }
        }
        return count;
    }
};
```

### 复杂度分析
- **时间复杂度**：O(n)，需要遍历字符串一次
- **空间复杂度**：O(1)，使用固定大小的数组（26个字母）

---

## 方法二：位置集合检查

### 思路
记录每个字母所有出现的位置，然后检查是否所有小写位置都在首个大写位置之前。

### 代码
```cpp
class Solution {
public:
    int numberOfSpecialChars(string word) {
        vector<vector<int>> lowerPos(26);  // 小写位置列表
        vector<vector<int>> upperPos(26);  // 大写位置列表

        for (int i = 0; i < word.size(); i++) {
            char c = word[i];
            if (c >= 'a' && c <= 'z') {
                lowerPos[c - 'a'].push_back(i);
            } else if (c >= 'A' && c <= 'Z') {
                upperPos[c - 'A'].push_back(i);
            }
        }

        int count = 0;
        for (int i = 0; i < 26; i++) {
            if (lowerPos[i].empty() || upperPos[i].empty()) {
                continue;
            }
            int firstUpper = upperPos[i][0];
            bool valid = true;
            for (int pos : lowerPos[i]) {
                if (pos >= firstUpper) {
                    valid = false;
                    break;
                }
            }
            if (valid) count++;
        }
        return count;
    }
};
```

### 复杂度分析
- **时间复杂度**：O(n)，最坏情况下需要检查所有位置
- **空间复杂度**：O(n)，需要存储所有位置

---

## 方法三：单次遍历状态标记

### 思路
使用状态机的思想，为每个字母维护一个状态：
- 状态 0：未出现
- 状态 1：只出现小写
- 状态 2：大写之后又出现小写（无效）
- 状态 3：有效特殊字母

遍历过程中实时更新状态。

### 代码
```cpp
class Solution {
public:
    int numberOfSpecialChars(string word) {
        vector<int> state(26, 0);        // 字母状态
        vector<bool> seenUpper(26, false); // 是否出现过大写

        for (char c : word) {
            if (c >= 'a' && c <= 'z') {
                int idx = c - 'a';
                if (seenUpper[idx]) {
                    state[idx] = 2;  // 大写后出现小写，无效
                } else if (state[idx] == 0) {
                    state[idx] = 1;  // 首次出现小写
                }
            } else if (c >= 'A' && c <= 'Z') {
                int idx = c - 'A';
                seenUpper[idx] = true;
                if (state[idx] == 1) {
                    state[idx] = 3;  // 有小写，现在出现大写，有效
                }
            }
        }

        int count = 0;
        for (int s : state) {
            if (s == 3) count++;
        }
        return count;
    }
};
```

### 复杂度分析
- **时间复杂度**：O(n)，单次遍历
- **空间复杂度**：O(1)，固定大小数组

---

## 方法四：位运算优化

### 思路
使用位运算来压缩状态：
- `seenLower`：哪些字母出现过小写
- `seenUpper`：哪些字母出现过大写
- `invalidAfterUpper`：哪些字母在大写后又出现小写

通过位与、位或、位非操作快速计算结果。

### 代码
```cpp
class Solution {
public:
    int numberOfSpecialChars(string word) {
        int seenLower = 0, seenUpper = 0;
        int invalidAfterUpper = 0;

        for (char c : word) {
            if (c >= 'a' && c <= 'z') {
                int bit = 1 << (c - 'a');
                if (seenUpper & bit) {
                    invalidAfterUpper |= bit;
                }
                seenLower |= bit;
            } else if (c >= 'A' && c <= 'Z') {
                int bit = 1 << (c - 'A');
                seenUpper |= bit;
            }
        }

        int special = seenLower & seenUpper & ~invalidAfterUpper;

        int count = 0;
        while (special) {
            count += special & 1;
            special >>= 1;
        }
        return count;
    }
};
```

### 复杂度分析
- **时间复杂度**：O(n)，单次遍历
- **空间复杂度**：O(1)，只使用几个整数变量

---

## 方法对比

| 方法 | 时间复杂度 | 空间复杂度 | 优点 | 缺点 |
|------|-----------|-----------|------|------|
| 方法一 | O(n) | O(1) | 简单直观，易于理解 | 需要两次遍历（字符串+数组） |
| 方法二 | O(n) | O(n) | 逻辑清晰 | 空间占用较大 |
| 方法三 | O(n) | O(1) | 单次遍历，状态清晰 | 状态定义稍复杂 |
| 方法四 | O(n) | O(1) | 极致优化，位运算高效 | 代码可读性较差 |

## 测试用例

```cpp
// 测试用例1: "aaAbcBC" -> 3
// 测试用例2: "abc" -> 0
// 测试用例3: "AbBCab" -> 0
```

## 总结

本题的关键在于理解"特殊字母"的定义：
1. 必须同时出现小写和大写
2. 所有小写必须在第一个大写之前

推荐使用 **方法一** 或 **方法三**，它们在时间和空间上都是最优的，且代码清晰易懂。方法四的位运算虽然高效，但可读性较差，适合对位运算熟悉的用户。
