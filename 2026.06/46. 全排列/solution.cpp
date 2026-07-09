#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
 * 46. 全排列
 *
 * 题目：给定一个不含重复数字的数组 nums，返回其所有可能的全排列。
 *       你可以按任意顺序返回答案。
 *
 * 示例：
 *   输入：nums = [1,2,3]
 *   输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
 *
 * 全排列：n个不同元素的所有可能排列，共 n! 种
 * 例如：[1,2,3] 的全排列有 3! = 6 种
 */

// ============================================================================
// 方法一：回溯法（标准模板）
// ============================================================================
// 思路：逐位选择，每次从剩余未使用的数字中选择一个放到当前位置
// 时间复杂度: O(N! * N)，N! 个排列，每个排列需要 O(N) 时间构造
// 空间复杂度: O(N)，递归栈深度和 visited 数组
class Solution1 {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;      // 当前正在构建的排列
        vector<bool> used(nums.size(), false);  // 标记哪些数字已使用

        backtrack(nums, current, used, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, vector<int>& current,
                   vector<bool>& used, vector<vector<int>>& result) {
        // 终止条件：当前排列长度等于数组长度，说明已经构建了一个完整排列
        if (current.size() == nums.size()) {
            result.push_back(current);
            return;
        }

        // 遍历所有数字，选择一个未使用的数字放到当前位置
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;  // 跳过已使用的数字

            // 做选择
            current.push_back(nums[i]);
            used[i] = true;

            // 递归构建下一位
            backtrack(nums, current, used, result);

            // 撤销选择（回溯）
            current.pop_back();
            used[i] = false;
        }
    }
};

// ============================================================================
// 方法二：交换法（原地交换）
// ============================================================================
// 思路：固定当前位置，将后面的每个数字交换到当前位置，然后递归处理剩余位置
// 时间复杂度: O(N! * N)
// 空间复杂度: O(N)，递归栈深度
class Solution2 {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;
        backtrack(nums, 0, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, int start, vector<vector<int>>& result) {
        // 终止条件：已经固定到最后一个位置
        if (start == nums.size()) {
            result.push_back(nums);
            return;
        }

        // 将 start 位置之后的每个数字交换到 start 位置
        for (int i = start; i < nums.size(); i++) {
            swap(nums[start], nums[i]);      // 做选择：将 nums[i] 放到 start 位置
            backtrack(nums, start + 1, result); // 递归处理下一个位置
            swap(nums[start], nums[i]);      // 撤销选择：恢复原状
        }
    }
};

// ============================================================================
// 方法三：使用 STL 的 next_permutation
// ============================================================================
// 思路：利用 C++ 标准库的 next_permutation 函数生成下一个排列
// 时间复杂度: O(N! * N)
// 空间复杂度: O(1)（不计结果空间）
class Solution3 {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;

        // 需要先排序，确保从最小排列开始
        sort(nums.begin(), nums.end());

        // 生成所有排列
        do {
            result.push_back(nums);
        } while (next_permutation(nums.begin(), nums.end()));

        return result;
    }
};

// ============================================================================
// 方法四：DFS 深度优先搜索
// ============================================================================
// 思路：每次从剩余数字中选择一个，构建排列树
// 时间复杂度: O(N! * N)
// 空间复杂度: O(N)
class Solution4 {
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
        // 找到一个完整排列
        if (path.size() == nums.size()) {
            result.push_back(path);
            return;
        }

        // 遍历所有可选数字
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

// 辅助函数：打印排列
void printPermutations(const vector<vector<int>>& perms) {
    cout << "[";
    for (int i = 0; i < perms.size(); i++) {
        cout << "[";
        for (int j = 0; j < perms[i].size(); j++) {
            cout << perms[i][j];
            if (j < perms[i].size() - 1) cout << ",";
        }
        cout << "]";
        if (i < perms.size() - 1) cout << ",";
    }
    cout << "]" << endl;
}

int main() {
    Solution1 sol1;
    Solution2 sol2;
    Solution3 sol3;
    Solution4 sol4;

    // 测试用例1
    vector<int> nums1 = {1, 2, 3};
    cout << "测试用例1: [1,2,3]" << endl;
    cout << "方法一(回溯): "; printPermutations(sol1.permute(nums1));
    cout << "方法二(交换): "; printPermutations(sol2.permute(nums1));
    cout << "方法三(STL):  "; printPermutations(sol3.permute(nums1));
    cout << "方法四(DFS):  "; printPermutations(sol4.permute(nums1));
    cout << endl;

    // 测试用例2
    vector<int> nums2 = {0, 1};
    cout << "测试用例2: [0,1]" << endl;
    cout << "方法一(回溯): "; printPermutations(sol1.permute(nums2));
    cout << "方法二(交换): "; printPermutations(sol2.permute(nums2));
    cout << "方法三(STL):  "; printPermutations(sol3.permute(nums2));
    cout << "方法四(DFS):  "; printPermutations(sol4.permute(nums2));
    cout << endl;

    // 测试用例3
    vector<int> nums3 = {1};
    cout << "测试用例3: [1]" << endl;
    cout << "方法一(回溯): "; printPermutations(sol1.permute(nums3));
    cout << "方法二(交换): "; printPermutations(sol2.permute(nums3));
    cout << "方法三(STL):  "; printPermutations(sol3.permute(nums3));
    cout << "方法四(DFS):  "; printPermutations(sol4.permute(nums3));

    return 0;
}
