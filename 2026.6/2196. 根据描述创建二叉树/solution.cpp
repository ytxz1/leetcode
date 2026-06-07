#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
using namespace std;

/*
 * 2196. 根据描述创建二叉树
 *
 * 题目：给你一个二维整数数组 descriptions，其中 descriptions[i] = [parenti, childi, isLefti]
 *       表示 parenti 是 childi 在二叉树中的父节点。
 *       - 如果 isLefti == 1，那么 childi 是 parenti 的左子节点
 *       - 如果 isLefti == 0，那么 childi 是 parenti 的右子节点
 *       构造出 descriptions 所描述的二叉树并返回其根节点。
 *
 * 示例：
 *   输入：descriptions = [[20,15,1],[20,17,0],[50,20,1],[50,80,0],[80,19,1]]
 *   输出：[50,20,80,15,17,19]
 *
 * 思路：
 *   1. 使用哈希表存储所有已创建的节点
 *   2. 使用集合记录所有子节点，根节点是唯一不在子节点集合中的节点
 *   3. 根据描述建立父子关系
 */

// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// ============================================================================
// 方法一：哈希表 + 集合
// ============================================================================
// 思路：
//   1. 遍历所有描述，创建所有节点并建立父子关系
//   2. 使用集合记录所有子节点
//   3. 根节点是不在子节点集合中的节点
//
// 时间复杂度: O(N)，N 为 descriptions 的长度
// 空间复杂度: O(N)，存储所有节点
class Solution1 {
public:
    TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
        // 哈希表：值 -> 节点指针
        unordered_map<int, TreeNode*> nodes;
        // 集合：记录所有子节点
        unordered_set<int> children;

        // 第一遍遍历：创建所有节点并建立父子关系
        for (auto& desc : descriptions) {
            int parentVal = desc[0];
            int childVal = desc[1];
            bool isLeft = desc[2] == 1;

            // 如果父节点不存在，创建它
            if (nodes.find(parentVal) == nodes.end()) {
                nodes[parentVal] = new TreeNode(parentVal);
            }
            // 如果子节点不存在，创建它
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

        // 找到根节点：不在子节点集合中的节点
        for (auto& [val, node] : nodes) {
            if (children.find(val) == children.end()) {
                return node;
            }
        }

        return nullptr;  // 不会执行到这里
    }
};

// ============================================================================
// 方法二：哈希表 + 入度计算
// ============================================================================
// 思路：
//   1. 使用入度（被指向的次数）来确定根节点
//   2. 根节点的入度为 0（没有父节点指向它）
//
// 时间复杂度: O(N)
// 空间复杂度: O(N)
class Solution2 {
public:
    TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
        unordered_map<int, TreeNode*> nodes;
        unordered_map<int, int> inDegree;  // 入度：被指向的次数

        for (auto& desc : descriptions) {
            int parentVal = desc[0];
            int childVal = desc[1];
            bool isLeft = desc[2] == 1;

            // 创建节点（如果不存在）
            if (nodes.find(parentVal) == nodes.end()) {
                nodes[parentVal] = new TreeNode(parentVal);
                inDegree[parentVal] = 0;  // 初始化入度
            }
            if (nodes.find(childVal) == nodes.end()) {
                nodes[childVal] = new TreeNode(childVal);
                inDegree[childVal] = 0;  // 初始化入度
            }

            // 建立父子关系
            if (isLeft) {
                nodes[parentVal]->left = nodes[childVal];
            } else {
                nodes[parentVal]->right = nodes[childVal];
            }

            // 子节点入度 +1
            inDegree[childVal]++;
        }

        // 找到入度为 0 的节点（根节点）
        for (auto& [val, degree] : inDegree) {
            if (degree == 0) {
                return nodes[val];
            }
        }

        return nullptr;
    }
};

// ============================================================================
// 方法三：只用哈希表（更简洁）
// ============================================================================
// 思路：
//   1. 只需要记录哪些节点是子节点
//   2. 遍历时同时建立关系，最后找不在子节点集合中的节点
//
// 时间复杂度: O(N)
// 空间复杂度: O(N)
class Solution3 {
public:
    TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {
        unordered_map<int, TreeNode*> nodes;
        unordered_set<int> isChild;  // 标记是否是子节点

        for (auto& desc : descriptions) {
            int p = desc[0], c = desc[1], l = desc[2];

            // 创建节点
            if (!nodes.count(p)) nodes[p] = new TreeNode(p);
            if (!nodes.count(c)) nodes[c] = new TreeNode(c);

            // 建立关系
            if (l) nodes[p]->left = nodes[c];
            else nodes[p]->right = nodes[c];

            // 标记子节点
            isChild.insert(c);
        }

        // 找根节点
        for (auto& [val, node] : nodes) {
            if (!isChild.count(val)) return node;
        }

        return nullptr;
    }
};

// 辅助函数：层序遍历打印二叉树
vector<int> levelOrder(TreeNode* root) {
    vector<int> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();

        if (node) {
            result.push_back(node->val);
            q.push(node->left);
            q.push(node->right);
        } else {
            result.push_back(-1);  // 用 -1 表示 null
        }
    }

    // 移除末尾的 -1
    while (!result.empty() && result.back() == -1) {
        result.pop_back();
    }

    return result;
}

// 辅助函数：打印数组
void printVector(const vector<int>& vec) {
    cout << "[";
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == -1) cout << "null";
        else cout << vec[i];
        if (i < vec.size() - 1) cout << ",";
    }
    cout << "]" << endl;
}

int main() {
    Solution1 sol1;
    Solution2 sol2;
    Solution3 sol3;

    // 测试用例1
    vector<vector<int>> desc1 = {{20,15,1},{20,17,0},{50,20,1},{50,80,0},{80,19,1}};
    cout << "测试用例1:" << endl;
    cout << "descriptions = [[20,15,1],[20,17,0],[50,20,1],[50,80,0],[80,19,1]]" << endl;
    TreeNode* root1 = sol1.createBinaryTree(desc1);
    cout << "方法一: "; printVector(levelOrder(root1));
    root1 = sol2.createBinaryTree(desc1);
    cout << "方法二: "; printVector(levelOrder(root1));
    root1 = sol3.createBinaryTree(desc1);
    cout << "方法三: "; printVector(levelOrder(root1));
    cout << endl;

    // 测试用例2
    vector<vector<int>> desc2 = {{1,2,1},{2,3,0},{3,4,1}};
    cout << "测试用例2:" << endl;
    cout << "descriptions = [[1,2,1],[2,3,0],[3,4,1]]" << endl;
    TreeNode* root2 = sol1.createBinaryTree(desc2);
    cout << "方法一: "; printVector(levelOrder(root2));
    root2 = sol2.createBinaryTree(desc2);
    cout << "方法二: "; printVector(levelOrder(root2));
    root2 = sol3.createBinaryTree(desc2);
    cout << "方法三: "; printVector(levelOrder(root2));

    return 0;
}
