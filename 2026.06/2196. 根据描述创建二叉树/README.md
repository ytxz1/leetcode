# 2196. 根据描述创建二叉树

## 题目描述

给你一个二维整数数组 `descriptions`，其中 `descriptions[i] = [parenti, childi, isLefti]` 表示 `parenti` 是 `childi` 在**二叉树**中的**父节点**。

- 如果 `isLefti == 1`，那么 `childi` 是 `parenti` 的**左子节点**
- 如果 `isLefti == 0`，那么 `childi` 是 `parenti` 的**右子节点**

构造出 `descriptions` 所描述的二叉树并返回其**根节点**。

## 示例

### 示例 1

```
输入：descriptions = [[20,15,1],[20,17,0],[50,20,1],[50,80,0],[80,19,1]]
输出：[50,20,80,15,17,19]

解释：
- 50 是 20 的父节点（20 是左子节点）
- 50 是 80 的父节点（80 是右子节点）
- 20 是 15 的父节点（15 是左子节点）
- 20 是 17 的父节点（17 是右子节点）
- 80 是 19 的父节点（19 是左子节点）

构建的二叉树：
        50
       /  \
      20   80
     / \   /
    15 17 19
```

### 示例 2

```
输入：descriptions = [[1,2,1],[2,3,0],[3,4,1]]
输出：[1,2,null,null,3,4]

解释：
- 1 是 2 的父节点（2 是左子节点）
- 2 是 3 的父节点（3 是右子节点）
- 3 是 4 的父节点（4 是左子节点）

构建的二叉树：
      1
     /
    2
     \
      3
     /
    4
```

## 解题思路

### 核心问题

1. **如何创建节点**：使用哈希表存储已创建的节点，避免重复创建
2. **如何建立关系**：根据 `isLeft` 标志设置左子节点或右子节点
3. **如何找到根节点**：根节点是唯一**不在子节点集合中**的节点

### 关键观察

- 每个节点最多有一个父节点
- 根节点没有父节点
- 所有其他节点都有且只有一个父节点

### 算法步骤

1. **遍历所有描述**
   - 创建父节点和子节点（如果不存在）
   - 根据 `isLeft` 建立父子关系
   - 记录所有子节点

2. **找到根节点**
   - 遍历所有节点
   - 找到不在子节点集合中的节点

## 代码实现

### 方法一：哈希表 + 集合（逐行详解）

```cpp
TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
```
**第1行**：函数签名，接收一个二维数组 `descriptions`，返回二叉树的根节点。

```cpp
    unordered_map<int, TreeNode*> nodes;  // 值 -> 节点指针
```
**第2行**：创建哈希表，键是节点的值（int），值是节点的指针（TreeNode*）。
- 作用：通过节点值快速找到对应的节点对象
- 例如：`nodes[50]` 就是值为 50 的节点指针

```cpp
    unordered_set<int> children;          // 记录所有子节点
```
**第3行**：创建集合，记录所有曾经作为子节点出现的值。
- 作用：最后用来找根节点（根节点不在这个集合中）

```cpp
    // 第一遍遍历：创建节点并建立关系
    for (auto& desc : descriptions) {
```
**第4-5行**：遍历每一条描述。`desc` 是一个长度为 3 的数组 `[parent, child, isLeft]`。

```cpp
        int parentVal = desc[0];  // 父节点的值
        int childVal = desc[1];   // 子节点的值
        bool isLeft = desc[2] == 1;  // 是否是左子节点
```
**第6-8行**：解构描述数组。
- `desc[0]`：父节点的值
- `desc[1]`：子节点的值
- `desc[2]`：1 表示左子节点，0 表示右子节点

```cpp
        // 创建节点（如果不存在）
        if (nodes.find(parentVal) == nodes.end()) {
            nodes[parentVal] = new TreeNode(parentVal);
        }
```
**第9-11行**：如果父节点还没创建过，就创建一个新的 TreeNode。
- `nodes.find(parentVal) == nodes.end()`：在哈希表中找不到这个节点
- `new TreeNode(parentVal)`：创建新节点，值为 parentVal
- `nodes[parentVal] = ...`：存入哈希表

```cpp
        if (nodes.find(childVal) == nodes.end()) {
            nodes[childVal] = new TreeNode(childVal);
        }
```
**第12-14行**：同理，如果子节点还没创建过，也创建一个新的。

```cpp
        // 建立父子关系
        if (isLeft) {
            nodes[parentVal]->left = nodes[childVal];  // 设置为左子节点
        } else {
            nodes[parentVal]->right = nodes[childVal]; // 设置为右子节点
        }
```
**第15-19行**：根据 `isLeft` 标志建立父子关系。
- `nodes[parentVal]`：通过哈希表找到父节点对象
- `->left` 或 `->right`：设置左子节点或右子节点指针
- `nodes[childVal]`：通过哈希表找到子节点对象

```cpp
        // 记录子节点
        children.insert(childVal);
```
**第20-21行**：将子节点的值加入集合。
- 这个集合记录了所有曾经是"子节点"的值
- 根节点永远不会出现在这个集合中（因为根节点没有父节点）

```cpp
    // 找根节点：不在子节点集合中的节点
    for (auto& [val, node] : nodes) {
```
**第22-23行**：遍历哈希表中的所有节点。`[val, node]` 是结构化绑定，`val` 是节点值，`node` 是节点指针。

```cpp
        if (children.find(val) == children.end()) {
            return node;  // 找到根节点，返回
        }
```
**第24-26行**：如果当前节点的值不在子节点集合中，说明它是根节点（没有父节点指向它）。

```cpp
    }
    return nullptr;  // 不会执行到这里（题目保证有解）
}
```
**第27-29行**：函数结束。正常情况下不会执行到 `return nullptr`，因为题目保证描述是有效的。

### 方法二：哈希表 + 入度计算（逐行详解）

**入度概念**：在图论中，入度是指向某个节点的边的数量。
- 根节点的入度 = 0（没有父节点指向它）
- 其他节点的入度 = 1（有且只有一个父节点）

```cpp
TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
```
**第1行**：函数签名，与方法一相同。

```cpp
    unordered_map<int, TreeNode*> nodes;
```
**第2行**：哈希表，存储值到节点指针的映射。

```cpp
    unordered_map<int, int> inDegree;  // 入度：被指向的次数
```
**第3行**：哈希表，记录每个节点的入度（被多少个父节点指向）。
- 键：节点的值
- 值：入度（被指向的次数）

```cpp
    for (auto& desc : descriptions) {
```
**第4行**：遍历每一条描述。

```cpp
        int parentVal = desc[0];
        int childVal = desc[1];
        bool isLeft = desc[2] == 1;
```
**第5-7行**：解构描述数组，获取父节点值、子节点值、是否左子节点。

```cpp
        // 创建节点
        if (nodes.find(parentVal) == nodes.end()) {
            nodes[parentVal] = new TreeNode(parentVal);
            inDegree[parentVal] = 0;  // 初始化入度为 0
        }
```
**第8-11行**：如果父节点不存在，创建它并初始化入度为 0。
- 新创建的节点默认入度为 0，因为它还没被任何父节点指向

```cpp
        if (nodes.find(childVal) == nodes.end()) {
            nodes[childVal] = new TreeNode(childVal);
            inDegree[childVal] = 0;  // 初始化入度为 0
        }
```
**第12-15行**：同理，如果子节点不存在，创建它并初始化入度为 0。

```cpp
        // 建立关系
        if (isLeft) {
            nodes[parentVal]->left = nodes[childVal];
        } else {
            nodes[parentVal]->right = nodes[childVal];
        }
```
**第16-20行**：建立父子关系，与方法一相同。

```cpp
        // 子节点入度 +1
        inDegree[childVal]++;
```
**第21-22行**：子节点被一个父节点指向了，所以入度加 1。
- 这是关键步骤：每个子节点的入度都会增加
- 根节点永远不会被增加入度（因为它不是任何节点的子节点）

```cpp
    // 找入度为 0 的节点（根节点）
    for (auto& [val, degree] : inDegree) {
```
**第23-24行**：遍历所有节点的入度。`[val, degree]` 是结构化绑定。

```cpp
        if (degree == 0) {
            return nodes[val];  // 入度为 0 的节点就是根节点
        }
```
**第25-27行**：如果某个节点的入度为 0，说明没有父节点指向它，它就是根节点。

```cpp
    }
    return nullptr;
}
```
**第28-30行**：函数结束。

### 方法三：简洁版（逐行详解）

**特点**：代码更简洁，使用了 C++ 的简写语法。

```cpp
TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
```
**第1行**：函数签名。

```cpp
    unordered_map<int, TreeNode*> nodes;
```
**第2行**：哈希表，存储值到节点指针的映射。

```cpp
    unordered_set<int> isChild;
```
**第3行**：集合，记录哪些值曾经作为子节点出现过。
- 与方法一的 `children` 作用相同，只是变量名更直观

```cpp
    for (auto& desc : descriptions) {
```
**第4行**：遍历每一条描述。

```cpp
        int p = desc[0], c = desc[1], l = desc[2];
```
**第5行**：解构描述数组，使用更短的变量名。
- `p`：parent（父节点值）
- `c`：child（子节点值）
- `l`：isLeft（是否左子节点）

```cpp
        if (!nodes.count(p)) nodes[p] = new TreeNode(p);
```
**第6行**：如果父节点不存在（`count` 返回 0），创建它。
- `nodes.count(p)`：返回键 `p` 在哈希表中出现的次数（0 或 1）
- `!nodes.count(p)`：等价于 `nodes.find(p) == nodes.end()`

```cpp
        if (!nodes.count(c)) nodes[c] = new TreeNode(c);
```
**第7行**：同理，如果子节点不存在，创建它。

```cpp
        if (l) nodes[p]->left = nodes[c];
        else nodes[p]->right = nodes[c];
```
**第8-9行**：建立父子关系。
- `if (l)`：如果 `l` 为 true（即 `isLeft == 1`），设置为左子节点
- `else`：否则设置为右子节点

```cpp
        isChild.insert(c);
```
**第10行**：将子节点的值加入集合。

```cpp
    }
```
**第11行**：结束 for 循环。

```cpp
    for (auto& [val, node] : nodes) {
```
**第12行**：遍历哈希表中的所有节点。

```cpp
        if (!isChild.count(val)) return node;
```
**第13行**：如果当前节点的值不在子节点集合中，说明它是根节点。
- `!isChild.count(val)`：等价于 `isChild.find(val) == isChild.end()`

```cpp
    }
    return nullptr;
}
```
**第14-16行**：函数结束。

## 复杂度分析

| 方法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 方法一 | O(N) | O(N) | 使用集合记录子节点 |
| 方法二 | O(N) | O(N) | 使用入度计算 |
| 方法三 | O(N) | O(N) | 代码更简洁 |

**说明**：N 为 descriptions 的长度

## 图解示例

以 `descriptions = [[20,15,1],[20,17,0],[50,20,1],[50,80,0],[80,19,1]]` 为例：

### 步骤 1：遍历描述，创建节点

```
描述 [20,15,1]：创建节点 20 和 15，15 是 20 的左子节点
描述 [20,17,0]：创建节点 17，17 是 20 的右子节点
描述 [50,20,1]：创建节点 50，20 是 50 的左子节点
描述 [50,80,0]：创建节点 80，80 是 50 的右子节点
描述 [80,19,1]：创建节点 19，19 是 80 的左子节点
```

### 步骤 2：记录子节点

```
子节点集合 = {15, 17, 20, 80, 19}
```

### 步骤 3：找根节点

```
所有节点 = {20, 15, 17, 50, 80, 19}
根节点 = 50（不在子节点集合中）
```

### 最终二叉树

```
        50
       /  \
      20   80
     / \   /
    15 17 19
```

## 测试用例

```cpp
// 测试用例1
descriptions = [[20,15,1],[20,17,0],[50,20,1],[50,80,0],[80,19,1]]
// 输出: [50,20,80,15,17,19]

// 测试用例2
descriptions = [[1,2,1],[2,3,0],[3,4,1]]
// 输出: [1,2,null,null,3,4]
```

## 总结

本题的关键是：

1. **使用哈希表**存储已创建的节点，避免重复创建
2. **找到根节点**：根节点是唯一不在子节点集合中的节点
3. **建立关系**：根据 `isLeft` 标志设置左子节点或右子节点

三种方法的本质相同，只是找根节点的方式略有不同：
- 方法一：使用集合记录子节点
- 方法二：使用入度计算
- 方法三：代码更简洁
