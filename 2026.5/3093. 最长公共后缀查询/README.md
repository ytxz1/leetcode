# 3093. 最长公共后缀查询

## 题目描述

给你两个字符串数组 `wordsContainer` 和 `wordsQuery`。

对于每个 `wordsQuery[i]`，你需要从 `wordsContainer` 中找到一个与 `wordsQuery[i]` 有 **最长公共后缀** 的字符串。

**选择规则（按优先级）：**
1. 选择公共后缀长度 **最长** 的字符串
2. 如果有多个字符串有相同的最长公共后缀，选择长度 **最短** 的
3. 如果还有多个字符串有相同的最短长度，选择在 `wordsContainer` 中出现 **更早** 的（索引更小的）

返回一个整数数组 `ans`，其中 `ans[i]` 是 `wordsContainer` 中与 `wordsQuery[i]` 有最长公共后缀字符串的下标。

## 示例

### 示例 1
```
输入:
wordsContainer = ["abcd", "bcd", "xbcd"]
wordsQuery = ["cd", "bcd", "xyz"]

输出: [1, 1, 1]

解释:
- "cd" 与 wordsContainer 中字符串的公共后缀:
  - "abcd": "cd" (长度2)
  - "bcd": "cd" (长度2)
  - "xbcd": "cd" (长度2)
  - 都有长度为2的公共后缀，选择最短的 "bcd" (索引1)

- "bcd" 与 wordsContainer 中字符串的公共后缀:
  - "abcd": "bcd" (长度3)
  - "bcd": "bcd" (长度3)
  - "xbcd": "bcd" (长度3)
  - 都有长度为3的公共后缀，选择最短的 "bcd" (索引1)

- "xyz" 与 wordsContainer 中字符串的公共后缀:
  - "abcd": "" (长度0)
  - "bcd": "" (长度0)
  - "xbcd": "" (长度0)
  - 都没有公共后缀，选择最短的 "bcd" (索引1)
```

### 示例 2
```
输入:
wordsContainer = ["abcdefgh", "poiuygh", "ghghgh"]
wordsQuery = ["gh", "acbfgh", "acbfegh"]

输出: [2, 0, 2]

解释:
- "gh" 与 wordsContainer 中字符串的公共后缀:
  - "abcdefgh": "gh" (长度2)
  - "poiuygh": "gh" (长度2)
  - "ghghgh": "gh" (长度2)
  - 都有长度为2的公共后缀，选择最短的 "ghghgh" (索引2)

- "acbfgh" 与 wordsContainer 中字符串的公共后缀:
  - "abcdefgh": "fgh" (长度3) - 从后往前: h=h, g=g, f=f
  - "poiuygh": "gh" (长度2)
  - "ghghgh": "gh" (长度2)
  - "abcdefgh" 有最长公共后缀 (长度3)，选择索引0

- "acbfegh" 与 wordsContainer 中字符串的公共后缀:
  - "abcdefgh": "gh" (长度2) - 从后往前: h=h, g=g, e≠f
  - "poiuygh": "gh" (长度2)
  - "ghghgh": "gh" (长度2)
  - 都有长度为2的公共后缀，选择最短的 "ghghgh" (索引2)
```

## 解题思路

### 核心概念：公共后缀
两个字符串的公共后缀是指从字符串末尾开始，连续相同的字符序列。

例如：
- "abcd" 和 "bcd" 的公共后缀是 "bcd"
- "abcdefgh" 和 "acbfgh" 的公共后缀是 "gh"

### 算法选择

#### 方法一：暴力法
对于每个查询字符串，遍历 wordsContainer 中的所有字符串，计算公共后缀长度，然后根据规则选择最优解。

**时间复杂度：** O(Q × C × L)
- Q: wordsQuery 的长度
- C: wordsContainer 的长度
- L: 字符串的平均长度

#### 方法二：字典树（Trie）

**核心思想：后缀 → 前缀转换**

公共后缀问题可以通过 **反转字符串** 转化为公共前缀问题。例如：
- "abcdefgh" 和 "acbfgh" 的公共后缀是 "gh"
- 反转后："hgfedcba" 和 "hgefbca" 的公共前缀是 "hg"

这样就可以使用 **字典树（Trie）** 高效处理前缀匹配问题。

**字典树结构设计：**

```cpp
struct TrieNode {
    TrieNode* children[26];  // 26个字母的子节点
    int bestIdx;             // 经过该节点的最优字符串索引
    int bestLen;             // 最优字符串的长度

    TrieNode() : bestIdx(-1), bestLen(INT_MAX) {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};
```

每个节点维护两个关键信息：
- `bestIdx`：经过该节点的所有字符串中，长度最短的字符串索引
- `bestLen`：该最短字符串的长度

**插入过程详解：**

以 `wordsContainer = ["abcdefgh", "poiuygh", "ghghgh"]` 为例：

1. 插入 "abcdefgh"（索引0，长度8）：
   - 反转后："hgfedcba"
   - 路径：root → 'h' → 'g' → 'f' → 'e' → 'd' → 'c' → 'b' → 'a'
   - 每个节点更新：bestIdx=0, bestLen=8

2. 插入 "poiuygh"（索引1，长度7）：
   - 反转后："hgyu iop"
   - 路径：root → 'h' → 'g' → 'y' → 'u' → 'i' → 'o' → 'p'
   - 'h' 和 'g' 节点：bestLen=7 < 8，更新为 bestIdx=1, bestLen=7
   - 新节点：bestIdx=1, bestLen=7

3. 插入 "ghghgh"（索引2，长度6）：
   - 反转后："hghghg"
   - 路径：root → 'h' → 'g' → 'h' → 'g' → 'h' → 'g'
   - 'h' 和 'g' 节点：bestLen=6 < 7，更新为 bestIdx=2, bestLen=6
   - 新节点：bestIdx=2, bestLen=6

**查询过程详解：**

查询 "acbfgh"（反转后："hgefbca"）：

1. 从根节点开始，初始化：bestIdx = root.bestIdx（最短单词索引）, bestDepth = 0
2. 逐字符匹配：
   - 'h'：匹配成功，depth=1，检查节点最优解
   - 'g'：匹配成功，depth=2，检查节点最优解
   - 'e'：匹配成功，depth=3，检查节点最优解
   - 'f'：匹配成功，depth=4，检查节点最优解
   - 'b'：匹配成功，depth=5，检查节点最优解
   - 'c'：匹配成功，depth=6，检查节点最优解
   - 'a'：匹配成功，depth=7，检查节点最优解

3. 返回最优解（深度最大，且在该深度下字符串最短）

**关键优化点：**
- 每个节点存储经过它的最优解，避免重复计算
- 查询时在每个匹配节点都检查是否更新全局最优
- 根节点存储所有字符串的最短长度（用于无匹配时的默认值）

**优势：**
- 预处理后，查询效率更高
- 适合多次查询的场景
- 空间换时间的经典应用

**时间复杂度：**
- 预处理：O(C × L) - C个字符串，每个长度L
- 查询：O(Q × L) - Q个查询，每个长度L
- 总计：O((C + Q) × L)

#### 方法三：优化暴力法
在方法一的基础上，添加一些剪枝策略：
- 如果当前单词长度已经比最优解长，且不可能有更长的公共后缀，跳过
- 计算公共后缀时，如果已经超过当前最优解，提前返回

### 关键点

1. **公共后缀计算：** 从两个字符串的末尾开始比较，直到字符不同或到达任一字符串的开头。

2. **选择规则的优先级：**
   - 首先：最长公共后缀
   - 其次：最短字符串长度
   - 最后：最小索引

3. **边界情况：**
   - 所有字符串都没有公共后缀（公共后缀长度为0）
   - wordsContainer 或 wordsQuery 为空

## 代码实现

### 方法一：暴力法
```cpp
vector<int> stringIndices(vector<string>& wordsContainer, vector<string>& wordsQuery) {
    vector<int> ans;
    for (const string& query : wordsQuery) {
        int bestIdx = 0;
        int bestSuffixLen = -1;
        int bestWordLen = wordsContainer[0].size();

        for (int i = 0; i < wordsContainer.size(); i++) {
            const string& word = wordsContainer[i];
            int suffixLen = getCommonSuffixLength(query, word);

            if (suffixLen > bestSuffixLen ||
                (suffixLen == bestSuffixLen && word.size() < bestWordLen) ||
                (suffixLen == bestSuffixLen && word.size() == bestWordLen && i < bestIdx)) {
                bestIdx = i;
                bestSuffixLen = suffixLen;
                bestWordLen = word.size();
            }
        }
        ans.push_back(bestIdx);
    }
    return ans;
}
```

### 方法二：字典树（Trie）
```cpp
struct TrieNode {
    TrieNode* children[26];
    int bestIdx;
    int bestLen;

    TrieNode() : bestIdx(-1), bestLen(1000001) {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};
```

## 复杂度分析

| 方法 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|-----------|-----------|---------|
| 暴力法 | O(Q × C × L) | O(1) | 数据规模较小 |
| 字典树 | O((C + Q) × L) | O(C × L) | 多次查询 |
| 优化暴力法 | O(Q × C × L) | O(1) | 实际运行更快 |

## 测试用例

```cpp
// 测试用例1
wordsContainer = ["abcd", "bcd", "xbcd"]
wordsQuery = ["cd", "bcd", "xyz"]
// 输出: [1, 1, 1]

// 测试用例2
wordsContainer = ["abcdefgh", "poiuygh", "ghghgh"]
wordsQuery = ["gh", "acbfgh", "acbfegh"]
// 输出: [2, 0, 2]
```

## 总结

本题的关键是理解公共后缀的概念和选择规则的优先级。对于小规模数据，暴力法足够高效；对于大规模数据或多次查询场景，字典树是更好的选择。
