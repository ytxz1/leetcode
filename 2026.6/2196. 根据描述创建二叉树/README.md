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

### 方法一：哈希表 + 集合

```cpp
TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
    unordered_map<int, TreeNode*> nodes;  // 值 -> 节点指针
    unordered_set<int> children;          // 记录所有子节点

    // 第一遍遍历：创建节点并建立关系
    for (auto& desc : descriptions) {
        int parentVal = desc[0];
        int childVal = desc[1];
        bool isLeft = desc[2] == 1;

        // 创建节点（如果不存在）
        if (nodes.find(parentVal) == nodes.end()) {
            nodes[parentVal] = new TreeNode(parentVal);
        }
        if (nodes.find(childVal) == nodes.end()) {
            nodes[childVal] = new TreeNode(childVal);
        }

        // 建立父子关系
        if (isLeft) {
            nodes[parentVal]->left = nodes[childVal];
        } else {
            nodes[parentVal]->right = nodes[childVal];
        }

        // 记录子节点
        children.insert(childVal);
    }

    // 找根节点：不在子节点集合中的节点
    for (auto& [val, node] : nodes) {
        if (children.find(val) == children.end()) {
            return node;
        }
    }

    return nullptr;
}
```

### 方法二：哈希表 + 入度计算

```cpp
TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
    unordered_map<int, TreeNode*> nodes;
    unordered_map<int, int> inDegree;  // 入度：被指向的次数

    for (auto& desc : descriptions) {
        int parentVal = desc[0];
        int childVal = desc[1];
        bool isLeft = desc[2] == 1;

        // 创建节点
        if (nodes.find(parentVal) == nodes.end()) {
            nodes[parentVal] = new TreeNode(parentVal);
            inDegree[parentVal] = 0;
        }
        if (nodes.find(childVal) == nodes.end()) {
            nodes[childVal] = new TreeNode(childVal);
            inDegree[childVal] = 0;
        }

        // 建立关系
        if (isLeft) {
            nodes[parentVal]->left = nodes[childVal];
        } else {
            nodes[parentVal]->right = nodes[childVal];
        }

        // 子节点入度 +1
        inDegree[childVal]++;
    }

    // 找入度为 0 的节点（根节点）
    for (auto& [val, degree] : inDegree) {
        if (degree == 0) {
            return nodes[val];
        }
    }

    return nullptr;
}
```

### 方法三：简洁版

```cpp
TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
    unordered_map<int, TreeNode*> nodes;
    unordered_set<int> isChild;

    for (auto& desc : descriptions) {
        int p = desc[0], c = desc[1], l = desc[2];

        if (!nodes.count(p)) nodes[p] = new TreeNode(p);
        if (!nodes.count(c)) nodes[c] = new TreeNode(c);

        if (l) nodes[p]->left = nodes[c];
        else nodes[p]->right = nodes[c];

        isChild.insert(c);
    }

    for (auto& [val, node] : nodes) {
        if (!isChild.count(val)) return node;
    }

    return nullptr;
}
```

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
