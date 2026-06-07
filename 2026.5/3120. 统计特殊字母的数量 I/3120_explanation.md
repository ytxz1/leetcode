# LeetCode 3120 - 统计特殊字母的数量 I

## 题目描述

给你一个字符串 `word`。如果 `word` 中同时存在某个字母的小写形式和大写形式，则称这个字母为 **特殊字母**。

返回 `word` 中 **特殊字母** 的数量。

## 示例

### 示例 1
```
输入: word = "aaAbcBC"
输出: 3
解释: 特殊字母是 'a'、'b'、'c'，因为它们同时存在大小写形式
```

### 示例 2
```
输入: word = "abc"
输出: 0
解释: 不存在特殊字母
```

### 示例 3
```
输入: word = "aA"
输出: 1
解释: 特殊字母是 'a'
```

## 解题思路

### 核心思想

要判断一个字母是否是"特殊字母"，需要检查：
1. 该字母的小写形式是否存在于字符串中
2. 该字母的大写形式是否存在于字符串中

如果两者都存在，则该字母是特殊字母。

### 算法步骤

1. **统计字母出现情况**
   - 使用两个哈希集合（unordered_set）分别记录出现的小写字母和大写字母
   - 遍历字符串，将每个字母添加到对应的集合中

2. **找出特殊字母**
   - 遍历小写字母集合
   - 检查每个小写字母对应的大写形式是否在大写集合中
   - 如果在，则该字母是特殊字母

3. **返回数量**
   - 统计满足条件的字母数量

### 代码实现

```cpp
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

class Solution {
public:
    int numberOfSpecialChars(string word) {
        // 使用 unordered_set 记录出现的小写字母和大写字母
        unordered_set<char> lower_set;
        unordered_set<char> upper_set;

        // 遍历字符串，统计大小写字母
        for (char c : word) {
            if (islower(c)) {
                lower_set.insert(c);
            } else if (isupper(c)) {
                upper_set.insert(c);
            }
        }

        // 统计同时存在大小写的字母
        int count = 0;
        for (char c : lower_set) {
            if (upper_set.count(toupper(c))) {
                count++;
            }
        }

        return count;
    }
};
```

## C++ 关键知识点

### 1. unordered_set（哈希集合）

```cpp
unordered_set<char> my_set;

// 插入元素
my_set.insert('a');

// 查找元素
if (my_set.count('a')) {
    // 元素存在
}

// 遍历集合
for (char c : my_set) {
    cout << c << endl;
}
```

**特点：**
- 插入、删除、查找的平均时间复杂度为 O(1)
- 基于哈希表实现
- 元素无序存储

### 2. 字符处理函数

```cpp
// 判断是否为小写字母
islower('a')  // 返回 true
islower('A')  // 返回 false

// 判断是否为大写字母
isupper('A')  // 返回 true
isupper('a')  // 返回 false

// 转换为大写
toupper('a')  // 返回 'A'

// 转换为小写
tolower('A')  // 返回 'a'
```

### 3. 范围 for 循环

```cpp
// 遍历字符串
for (char c : word) {
    // 处理每个字符
}

// 遍历集合
for (char c : lower_set) {
    // 处理每个元素
}
```

## 复杂度分析

- **时间复杂度**: O(n)
  - 需要遍历整个字符串一次
  - unordered_set 的插入和查找操作平均为 O(1)

- **空间复杂度**: O(1)
  - 最多存储 26 个小写字母和 26 个大写字母
  - 空间使用是常数级别的

## 与 Python 实现的对比

| 特性 | C++ | Python |
|------|-----|--------|
| 集合类型 | `unordered_set` | `set` |
| 插入操作 | `set.insert(x)` | `set.add(x)` |
| 查找操作 | `set.count(x)` | `x in set` |
| 大写转换 | `toupper(x)` | `x.upper()` |
| 小写转换 | `tolower(x)` | `x.lower()` |
| 判断大写 | `isupper(x)` | `x.isupper()` |
| 判断小写 | `islower(x)` | `x.islower()` |

## 测试用例

```cpp
vector<pair<string, int>> test_cases = {
    {"aaAbcBC", 3},      // 特殊字母: a, b, c
    {"abc", 0},           // 无特殊字母
    {"ABC", 0},           // 无特殊字母
    {"aA", 1},            // 特殊字母: a
    {"", 0},              // 空字符串
    {"aAbBcCdD", 4},      // 特殊字母: a, b, c, d
};
```

## 总结

这道题目的核心是理解"特殊字母"的定义，并使用合适的数据结构（哈希集合）来高效地统计满足条件的字母数量。

**C++ 实现要点：**
1. 使用 `unordered_set` 实现 O(1) 的查找操作
2. 使用 `islower()`、`isupper()` 判断字母大小写
3. 使用 `toupper()`、`tolower()` 进行大小写转换
4. 使用 `count()` 方法检查元素是否存在

通过使用哈希集合，我们可以在 O(n) 的时间复杂度内完成问题的求解。
