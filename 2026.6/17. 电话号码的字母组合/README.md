# 17. 电话号码的字母组合

## 📋 目录

- [题目描述](#题目描述)
- [示例详解](#示例详解)
- [约束条件](#约束条件)
- [解题思路](#解题思路)
- [代码实现](#代码实现)
- [复杂度分析](#复杂度分析)
- [关键点与注意事项](#关键点与注意事项)
- [常见错误与陷阱](#常见错误与陷阱)
- [相关题目推荐](#相关题目推荐)
- [参考资料](#参考资料)

---

## 题目描述

给定一个仅包含数字 `2-9` 的字符串，返回所有它能表示的字母组合。答案可以按 **任意顺序** 返回。

给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。

```
2 -> a, b, c
3 -> d, e, f
4 -> g, h, i
5 -> j, k, l
6 -> m, n, o
7 -> p, q, r, s
8 -> t, u, v
9 -> w, x, y, z
```

### 题目本质

这道题本质上是一个 **全排列问题** 的变体，需要生成所有可能的字母组合。可以使用 **回溯法**（Backtracking）或 **深度优先搜索**（DFS）来解决。

### 核心思想

对于输入的每一位数字，从其对应的字母中选择一个，然后递归处理下一位数字，直到处理完所有数字。

---

## 示例详解

### 示例 1：基本示例

```
输入：digits = "23"
输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
```

**详细分析：**

| 数字 | 对应字母 |
|------|---------|
| 2    | a, b, c |
| 3    | d, e, f |

**组合过程：**
1. 选择 '2' 对应的 'a'，然后选择 '3' 对应的 'd' → "ad"
2. 选择 '2' 对应的 'a'，然后选择 '3' 对应的 'e' → "ae"
3. 选择 '2' 对应的 'a'，然后选择 '3' 对应的 'f' → "af"
4. 选择 '2' 对应的 'b'，然后选择 '3' 对应的 'd' → "bd"
5. 选择 '2' 对应的 'b'，然后选择 '3' 对应的 'e' → "be"
6. 选择 '2' 对应的 'b'，然后选择 '3' 对应的 'f' → "bf"
7. 选择 '2' 对应的 'c'，然后选择 '3' 对应的 'd' → "cd"
8. 选择 '2' 对应的 'c'，然后选择 '3' 对应的 'e' → "ce"
9. 选择 '2' 对应的 'c'，然后选择 '3' 对应的 'f' → "cf"

**结果：** 共 3 × 3 = 9 种组合

### 示例 2：空字符串

```
输入：digits = ""
输出：[]
```

**分析：** 输入为空，没有数字，因此没有字母组合，返回空列表。

### 示例 3：单个数字

```
输入：digits = "2"
输出：["a","b","c"]
```

**分析：** 只有一位数字 '2'，对应字母 'a'、'b'、'c'，返回这三个字母。

### 示例 4：多位数字

```
输入：digits = "234"
输出：["adg","adh","adi","aeg","aeh","aei","afg","afh","afi",
       "bdg","bdh","bdi","beg","beh","bei","bfg","bfh","bfi",
       "cdg","cdh","cdi","ceg","ceh","cei","cfg","cfh","cfi"]
```

**分析：**
- 数字 '2' 对应 3 个字母
- 数字 '3' 对应 3 个字母
- 数字 '4' 对应 3 个字母
- 总共 3 × 3 × 3 = 27 种组合

### 示例 5：包含 '7' 和 '9'

```
输入：digits = "79"
输出：["pw","px","py","pz","qw","qx","qy","qz","rw","rx","ry","rz","sw","sx","sy","sz"]
```

**分析：**
- 数字 '7' 对应 4 个字母（p, q, r, s）
- 数字 '9' 对应 4 个字母（w, x, y, z）
- 总共 4 × 4 = 16 种组合

---

## 约束条件

- `0 <= digits.length <= 4`
- `digits[i]` 是范围 `['2', '9']` 的一个数字

### 约束分析

1. **长度限制**：最多 4 位数字，最多生成 4^4 = 256 种组合
2. **数字范围**：只包含 '2'-'9'，不包含 '0'、'1'、'*'、'#'
3. **空字符串**：可能输入为空字符串，需要特殊处理

---

## 解题思路

### 方法一：回溯法（推荐）

**核心思想：** 使用递归和回溯，逐位生成所有可能的字母组合。

**算法步骤：**
1. 创建数字到字母的映射表
2. 定义回溯函数，参数包括当前处理的数字位置和当前路径
3. 如果当前位置等于数字长度，将当前路径加入结果
4. 否则，遍历当前数字对应的所有字母，递归处理下一位
5. 回溯时移除最后添加的字母

**图解说明：**

```
输入：digits = "23"

回溯树：
                    ""
                   / | \
                  a  b  c
                 /|\ /|\ /|\
                d e f d e f d e f

结果：["ad","ae","af","bd","be","bf","cd","ce","cf"]
```

**详细过程：**

```
1. 调用 backtrack(0, "")
   - index = 0, 当前数字 '2', 对应字母 'a', 'b', 'c'
   
2. 选择 'a': path = "a", 调用 backtrack(1, "a")
   - index = 1, 当前数字 '3', 对应字母 'd', 'e', 'f'
   
3. 选择 'd': path = "ad", 调用 backtrack(2, "ad")
   - index = 2, 等于 digits.length(), 将 "ad" 加入结果
   
4. 回溯: path = "a"
   
5. 选择 'e': path = "ae", 调用 backtrack(2, "ae")
   - index = 2, 等于 digits.length(), 将 "ae" 加入结果
   
6. 继续回溯...
```

**优点：**
- 代码简洁易懂
- 逻辑清晰
- 时间复杂度最优

**缺点：**
- 需要递归调用，有栈开销

---

### 方法二：迭代法

**核心思想：** 使用队列或循环，逐步生成所有组合。

**算法步骤：**
1. 初始化结果列表为空
2. 对于每一位数字，遍历当前结果列表，将新字母追加到每个结果后面
3. 更新结果列表

**图解说明：**

```
输入：digits = "23"

初始：result = [""]

处理数字 '2' (对应 'a', 'b', 'c'):
  - 对于 result 中的每个字符串 "":
    - 追加 'a': "a"
    - 追加 'b': "b"
    - 追加 'c': "c"
  - result = ["a", "b", "c"]

处理数字 '3' (对应 'd', 'e', 'f'):
  - 对于 result 中的每个字符串 "a", "b", "c":
    - "a" + 'd' = "ad"
    - "a" + 'e' = "ae"
    - "a" + 'f' = "af"
    - "b" + 'd' = "bd"
    - ...
  - result = ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"]
```

**优点：**
- 不需要递归
- 代码直观

**缺点：**
- 需要多次复制字符串
- 空间复杂度较高

---

### 方法三：BFS（广度优先搜索）

**核心思想：** 使用队列进行广度优先搜索。

**算法步骤：**
1. 将空字符串加入队列
2. 对于每一位数字，处理队列中的所有字符串
3. 对于每个字符串，追加当前数字对应的所有字母
4. 将新字符串加入队列
5. 重复直到处理完所有数字

**优点：**
- 层次清晰
- 易于理解

**缺点：**
- 需要额外的队列空间

---

### 方法四：DFS（深度优先搜索）

**核心思想：** 与回溯法类似，但使用显式栈。

**算法步骤：**
1. 使用栈存储状态（当前索引和当前路径）
2. 从栈中弹出状态
3. 如果索引等于数字长度，将路径加入结果
4. 否则，将当前数字对应的所有字母压入栈

**优点：**
- 避免递归
- 可以处理更深的搜索

**缺点：**
- 代码复杂

---

## 代码实现

### C++ 实现

#### 解法一：回溯法（推荐）

```cpp
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Solution {
private:
    // 数字到字母的映射
    string letterMap[10] = {
        "",     // 0
        "",     // 1
        "abc",  // 2
        "def",  // 3
        "ghi",  // 4
        "jkl",  // 5
        "mno",  // 6
        "pqrs", // 7
        "tuv",  // 8
        "wxyz"  // 9
    };
    
    vector<string> result;  // 存储结果
    string path;            // 当前路径
    
    // 回溯函数
    void backtrack(string& digits, int index) {
        // 终止条件：处理完所有数字
        if (index == digits.size()) {
            result.push_back(path);
            return;
        }
        
        // 获取当前数字
        int digit = digits[index] - '0';
        string letters = letterMap[digit];
        
        // 遍历当前数字对应的所有字母
        for (char c : letters) {
            path.push_back(c);           // 做选择
            backtrack(digits, index + 1); // 递归
            path.pop_back();             // 撤销选择（回溯）
        }
    }
    
public:
    vector<string> letterCombinations(string digits) {
        // 边界条件：空字符串
        if (digits.empty()) {
            return {};
        }
        
        // 清空结果
        result.clear();
        path.clear();
        
        // 开始回溯
        backtrack(digits, 0);
        
        return result;
    }
};
```

#### 解法二：迭代法

```cpp
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) {
            return {};
        }
        
        // 数字到字母的映射
        string letterMap[10] = {
            "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
        };
        
        vector<string> result = {""};
        
        // 对于每一位数字
        for (char digit : digits) {
            vector<string> temp;
            string letters = letterMap[digit - '0'];
            
            // 遍历当前结果列表
            for (string& s : result) {
                // 追加当前数字对应的所有字母
                for (char c : letters) {
                    temp.push_back(s + c);
                }
            }
            
            // 更新结果列表
            result = temp;
        }
        
        return result;
    }
};
```

#### 解法三：BFS（广度优先搜索）

```cpp
#include <queue>

class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) {
            return {};
        }
        
        // 数字到字母的映射
        string letterMap[10] = {
            "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
        };
        
        queue<string> q;
        q.push("");
        
        // 对于每一位数字
        for (char digit : digits) {
            int size = q.size();
            string letters = letterMap[digit - '0'];
            
            // 处理当前队列中的所有字符串
            for (int i = 0; i < size; i++) {
                string current = q.front();
                q.pop();
                
                // 追加当前数字对应的所有字母
                for (char c : letters) {
                    q.push(current + c);
                }
            }
        }
        
        // 将队列转换为向量
        vector<string> result;
        while (!q.empty()) {
            result.push_back(q.front());
            q.pop();
        }
        
        return result;
    }
};
```

#### 解法四：DFS（使用显式栈）

```cpp
#include <stack>

class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) {
            return {};
        }
        
        // 数字到字母的映射
        string letterMap[10] = {
            "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
        };
        
        vector<string> result;
        
        // 使用栈存储状态：(当前索引, 当前路径)
        stack<pair<int, string>> stk;
        stk.push({0, ""});
        
        while (!stk.empty()) {
            auto [index, path] = stk.top();
            stk.pop();
            
            // 终止条件：处理完所有数字
            if (index == digits.size()) {
                result.push_back(path);
                continue;
            }
            
            // 获取当前数字对应的字母
            int digit = digits[index] - '0';
            string letters = letterMap[digit];
            
            // 将所有可能的选择压入栈
            for (char c : letters) {
                stk.push({index + 1, path + c});
            }
        }
        
        return result;
    }
};
```

---

## 复杂度分析

### 时间复杂度

| 方法 | 时间复杂度 | 说明 |
|------|-----------|------|
| 回溯法 | O(3^m × 4^n) | m 是对应 3 个字母的数字个数，n 是对应 4 个字母的数字个数 |
| 迭代法 | O(3^m × 4^n) | 同上 |
| BFS | O(3^m × 4^n) | 同上 |
| DFS | O(3^m × 4^n) | 同上 |

**详细分析：**
- 数字 2, 3, 4, 5, 6, 8 对应 3 个字母
- 数字 7, 9 对应 4 个字母
- 最坏情况：输入 "7777"，生成 4^4 = 256 种组合
- 最好情况：输入 "2222"，生成 3^4 = 81 种组合

### 空间复杂度

| 方法 | 空间复杂度 | 说明 |
|------|-----------|------|
| 回溯法 | O(m + n) | 递归栈的深度 |
| 迭代法 | O(3^m × 4^n) | 存储所有结果 |
| BFS | O(3^m × 4^n) | 队列的大小 |
| DFS | O(3^m × 4^n) | 栈的大小 |

**注意：** 如果不考虑输出结果的空间，回溯法的空间复杂度是 O(m + n)，其中 m + n 是数字的长度。

### 复杂度总结

| 方法 | 时间复杂度 | 空间复杂度 | 优点 | 缺点 |
|------|-----------|-----------|------|------|
| 回溯法 | O(3^m × 4^n) | O(m + n) | 代码简洁，空间效率高 | 递归调用 |
| 迭代法 | O(3^m × 4^n) | O(3^m × 4^n) | 不需要递归 | 需要多次复制字符串 |
| BFS | O(3^m × 4^n) | O(3^m × 4^n) | 层次清晰 | 需要额外队列空间 |
| DFS | O(3^m × 4^n) | O(3^m × 4^n) | 避免递归 | 代码复杂 |

---

## 关键点与注意事项

### 1. 边界条件处理

需要处理以下边界情况：

1. **空字符串**：输入为空字符串时，返回空列表
2. **单个数字**：只有一位数字时，返回该数字对应的字母列表
3. **包含 '0' 或 '1'**：题目保证不会出现，但实际代码中最好处理

### 2. 数字到字母的映射

```cpp
// 正确的映射方式
string letterMap[10] = {
    "",     // 0 - 无对应字母
    "",     // 1 - 无对应字母
    "abc",  // 2
    "def",  // 3
    "ghi",  // 4
    "jkl",  // 5
    "mno",  // 6
    "pqrs", // 7 - 注意是4个字母
    "tuv",  // 8
    "wxyz"  // 9 - 注意是4个字母
};
```

**注意：** 数字 '7' 和 '9' 对应 4 个字母，其他数字对应 3 个字母。

### 3. 回溯模板

```cpp
void backtrack(参数) {
    if (终止条件) {
        存放结果;
        return;
    }
    
    for (选择：本层集合) {
        处理节点;
        backtrack(路径, 选择列表); // 递归
        回溯，撤销处理结果;
    }
}
```

### 4. 优化技巧

1. **预分配空间**：如果知道结果数量，可以预分配 vector 空间
2. **使用引用**：避免不必要的字符串复制
3. **剪枝**：本题不需要剪枝，所有路径都是有效的

### 5. 常见错误

1. **忘记处理空字符串**：会导致运行时错误
2. **映射错误**：数字 '7' 和 '9' 有 4 个字母，容易遗漏
3. **回溯不完整**：忘记撤销选择，导致结果错误

---

## 常见错误与陷阱

### 错误 1：忘记处理空字符串

```cpp
// 错误：没有处理空字符串
vector<string> letterCombinations(string digits) {
    // 如果 digits 为空，下面的代码会出错
    backtrack(digits, 0);
    return result;
}

// 正确：先处理空字符串
vector<string> letterCombinations(string digits) {
    if (digits.empty()) {
        return {};
    }
    backtrack(digits, 0);
    return result;
}
```

### 错误 2：映射错误

```cpp
// 错误：'7' 和 '9' 只映射了 3 个字母
string letterMap[10] = {
    "", "", "abc", "def", "ghi", "jkl", "mno", "pqr", "tuv", "wxy"
};

// 正确：'7' 映射 "pqrs"，'9' 映射 "wxyz"
string letterMap[10] = {
    "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
};
```

### 错误 3：回溯不完整

```cpp
// 错误：忘记撤销选择
for (char c : letters) {
    path.push_back(c);
    backtrack(digits, index + 1);
    // 缺少 path.pop_back();
}

// 正确：添加撤销选择
for (char c : letters) {
    path.push_back(c);
    backtrack(digits, index + 1);
    path.pop_back();  // 撤销选择
}
```

### 错误 4：结果重复

```cpp
// 错误：在循环外添加结果
for (char c : letters) {
    path.push_back(c);
    backtrack(digits, index + 1);
}
if (index == digits.size()) {
    result.push_back(path);  // 错误：会被多次添加
}

// 正确：在终止条件中添加结果
if (index == digits.size()) {
    result.push_back(path);
    return;
}
for (char c : letters) {
    path.push_back(c);
    backtrack(digits, index + 1);
    path.pop_back();
}
```

### 错误 5：类型错误

```cpp
// 错误：使用 char 而不是 int
int digit = digits[index];  // 错误：这是 ASCII 值

// 正确：转换为数字
int digit = digits[index] - '0';
```

---

## 相关题目推荐

### 基础题目

1. **46. 全排列**
   - 难度：中等
   - 链接：https://leetcode.cn/problems/permutations/
   - 关联：回溯法的经典应用

2. **47. 全排列 II**
   - 难度：中等
   - 链接：https://leetcode.cn/problems/permutations-ii/
   - 关联：包含重复元素的全排列

3. **78. 子集**
   - 难度：中等
   - 链接：https://leetcode.cn/problems/subsets/
   - 关联：回溯法生成所有子集

### 进阶题目

4. **22. 括号生成**
   - 难度：中等
   - 链接：https://leetcode.cn/problems/generate-parentheses/
   - 关联：回溯法生成合法括号

5. **39. 组合总和**
   - 难度：中等
   - 链接：https://leetcode.cn/problems/combination-sum/
   - 关联：回溯法求组合

6. **40. 组合总和 II**
   - 难度：中等
   - 链接：https://leetcode.cn/problems/combination-sum-ii/
   - 关联：包含重复元素的组合

### 相关算法

7. **回溯法详解**
   - 链接：https://programmercarl.com/backtracking_summary.html
   - 关联：回溯法的通用模板

8. **深度优先搜索**
   - 链接：https://en.wikipedia.org/wiki/Depth-first_search
   - 关联：DFS 的基本概念

---

## 参考资料

### 官方资源

- [LeetCode 17 题目页面](https://leetcode.cn/problems/letter-combinations-of-a-phone-number/)
- [LeetCode 官方题解](https://leetcode.cn/problems/letter-combinations-of-a-phone-number/solution/)

### 算法资料

- [回溯法算法详解](https://programmercarl.com/backtracking_summary.html)
- [深度优先搜索](https://en.wikipedia.org/wiki/Depth-first_search)
- [全排列问题](https://en.wikipedia.org/wiki/Permutation)

### 编程语言文档

- [C++ string](https://en.cppreference.com/w/cpp/string/basic_string)
- [C++ vector](https://en.cppreference.com/w/cpp/container/vector)
- [C++ stack](https://en.cppreference.com/w/cpp/container/stack)
- [C++ queue](https://en.cppreference.com/w/cpp/container/queue)

---

## 更新日志

- **2026-06-09**：初始版本，包含题目描述、多种解法、复杂度分析等

---

## 作者备注

这道题是回溯法的经典应用，主要考察：
1. 对回溯法的理解
2. 递归和回溯的实现
3. 边界条件的处理

建议先理解基本的回溯模板，再尝试优化和改进。

---

**最后更新：2026-06-09**
