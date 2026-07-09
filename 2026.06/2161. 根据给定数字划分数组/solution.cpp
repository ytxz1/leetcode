#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * 2161. 根据给定数字划分数组
 *
 * 题目描述：
 * 给你一个下标从 0 开始的整数数组 nums 和一个整数 pivot。
 * 请你将 nums 重新排列，使得满足以下条件：
 * 1. 小于 pivot 的元素都出现在前面
 * 2. 等于 pivot 的元素都出现在中间
 * 3. 大于 pivot 的元素都出现在后面
 * 返回重新排列后的数组。
 */

// ==================== 解法一：三路分割（推荐）====================
// 思路：遍历数组，分别收集三类元素
// 时间复杂度：O(n)
// 空间复杂度：O(n)
class Solution1 {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        vector<int> less;      // 存储小于 pivot 的元素
        vector<int> equal;     // 存储等于 pivot 的元素
        vector<int> greater;   // 存储大于 pivot 的元素

        // 第一次遍历：分类收集元素
        for (int num : nums) {
            if (num < pivot) {
                less.push_back(num);
            } else if (num == pivot) {
                equal.push_back(num);
            } else {
                greater.push_back(num);
            }
        }

        // 合并结果
        vector<int> result;
        result.reserve(nums.size());  // 预分配空间，提高效率
        result.insert(result.end(), less.begin(), less.end());
        result.insert(result.end(), equal.begin(), equal.end());
        result.insert(result.end(), greater.begin(), greater.end());

        return result;
    }
};

// ==================== 解法二：统计计数法 ====================
// 思路：先统计各类元素数量，再按位置填充结果数组
// 时间复杂度：O(n)
// 空间复杂度：O(n)
class Solution2 {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        int n = nums.size();
        vector<int> result(n);

        // 统计各类元素的数量
        int lessCount = 0, equalCount = 0, greaterCount = 0;
        for (int num : nums) {
            if (num < pivot) {
                lessCount++;
            } else if (num == pivot) {
                equalCount++;
            } else {
                greaterCount++;
            }
        }

        // 计算各类元素的起始位置
        int lessStart = 0;
        int equalStart = lessCount;
        int greaterStart = lessCount + equalCount;

        // 第二次遍历：填充结果数组
        for (int num : nums) {
            if (num < pivot) {
                result[lessStart++] = num;
            } else if (num == pivot) {
                result[equalStart++] = num;
            } else {
                result[greaterStart++] = num;
            }
        }

        return result;
    }
};

// ==================== 解法三：双指针法 ====================
// 思路：使用两个指针分别从前往后和从后往前遍历
// 时间复杂度：O(n)
// 空间复杂度：O(n)
class Solution3 {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        int n = nums.size();
        vector<int> result(n);

        // 第一遍：放置小于 pivot 的元素
        int left = 0;
        for (int num : nums) {
            if (num < pivot) {
                result[left++] = num;
            }
        }

        // 第二遍：放置等于 pivot 的元素
        int mid = left;
        for (int num : nums) {
            if (num == pivot) {
                result[mid++] = num;
            }
        }

        // 第三遍：放置大于 pivot 的元素
        int right = mid;
        for (int num : nums) {
            if (num > pivot) {
                result[right++] = num;
            }
        }

        return result;
    }
};

// ==================== 解法四：单次遍历优化 ====================
// 思路：只遍历一次，使用多个结果数组
// 时间复杂度：O(n)
// 空间复杂度：O(n)
class Solution4 {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        vector<int> less, equal, greater;

        // 单次遍历完成分类
        for (int num : nums) {
            if (num < pivot) {
                less.push_back(num);
            } else if (num == pivot) {
                equal.push_back(num);
            } else {
                greater.push_back(num);
            }
        }

        // 使用 move 语义优化合并
        vector<int> result;
        result.reserve(nums.size());
        result.insert(result.end(), make_move_iterator(less.begin()), make_move_iterator(less.end()));
        result.insert(result.end(), make_move_iterator(equal.begin()), make_move_iterator(equal.end()));
        result.insert(result.end(), make_move_iterator(greater.begin()), make_move_iterator(greater.end()));

        return result;
    }
};

// ==================== 辅助函数：打印数组 ====================
void printVector(const vector<int>& vec, const string& label = "") {
    if (!label.empty()) {
        cout << label << ": ";
    }
    cout << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

// ==================== 测试函数 ====================
void testSolution(Solution1& sol, const string& name) {
    cout << "========== " << name << " ==========" << endl;

    // 测试用例 1
    vector<int> nums1 = {9, 12, 5, 10, 14, 3, 10};
    int pivot1 = 10;
    vector<int> result1 = sol.pivotArray(nums1, pivot1);
    printVector(nums1, "输入");
    cout << "pivot: " << pivot1 << endl;
    printVector(result1, "输出");
    printVector({9, 5, 3, 10, 10, 12, 14}, "期望");
    cout << "测试通过: " << (result1 == vector<int>{9, 5, 3, 10, 10, 12, 14} ? "✓" : "✗") << endl;
    cout << endl;

    // 测试用例 2
    vector<int> nums2 = {-3, 4, 3, 2};
    int pivot2 = 2;
    vector<int> result2 = sol.pivotArray(nums2, pivot2);
    printVector(nums2, "输入");
    cout << "pivot: " << pivot2 << endl;
    printVector(result2, "输出");
    printVector({-3, 2, 4, 3}, "期望");
    cout << "测试通过: " << (result2 == vector<int>{-3, 2, 4, 3} ? "✓" : "✗") << endl;
    cout << endl;

    // 测试用例 3：所有元素相等
    vector<int> nums3 = {5, 5, 5, 5};
    int pivot3 = 5;
    vector<int> result3 = sol.pivotArray(nums3, pivot3);
    printVector(nums3, "输入");
    cout << "pivot: " << pivot3 << endl;
    printVector(result3, "输出");
    printVector({5, 5, 5, 5}, "期望");
    cout << "测试通过: " << (result3 == vector<int>{5, 5, 5, 5} ? "✓" : "✗") << endl;
    cout << endl;

    // 测试用例 4：单个元素
    vector<int> nums4 = {1};
    int pivot4 = 1;
    vector<int> result4 = sol.pivotArray(nums4, pivot4);
    printVector(nums4, "输入");
    cout << "pivot: " << pivot4 << endl;
    printVector(result4, "输出");
    printVector({1}, "期望");
    cout << "测试通过: " << (result4 == vector<int>{1} ? "✓" : "✗") << endl;
    cout << endl;

    // 测试用例 5：大数值
    vector<int> nums5 = {1000000, -1000000, 0, 500000};
    int pivot5 = 0;
    vector<int> result5 = sol.pivotArray(nums5, pivot5);
    printVector(nums5, "输入");
    cout << "pivot: " << pivot5 << endl;
    printVector(result5, "输出");
    printVector({-1000000, 0, 1000000, 500000}, "期望");
    cout << "测试通过: " << (result5 == vector<int>{-1000000, 0, 1000000, 500000} ? "✓" : "✗") << endl;
    cout << endl;
}

int main() {
    cout << "2161. 根据给定数字划分数组" << endl;
    cout << endl;

    // 测试解法一
    Solution1 sol1;
    testSolution(sol1, "解法一：三路分割");

    // 测试解法二
    Solution2 sol2;
    cout << "========== 解法二：统计计数法 ==========" << endl;
    vector<int> nums1 = {9, 12, 5, 10, 14, 3, 10};
    int pivot1 = 10;
    vector<int> result1 = sol2.pivotArray(nums1, pivot1);
    printVector(nums1, "输入");
    cout << "pivot: " << pivot1 << endl;
    printVector(result1, "输出");
    printVector({9, 5, 3, 10, 10, 12, 14}, "期望");
    cout << "测试通过: " << (result1 == vector<int>{9, 5, 3, 10, 10, 12, 14} ? "✓" : "✗") << endl;
    cout << endl;

    // 测试解法三
    Solution3 sol3;
    cout << "========== 解法三：双指针法 ==========" << endl;
    vector<int> nums2 = {-3, 4, 3, 2};
    int pivot2 = 2;
    vector<int> result2 = sol3.pivotArray(nums2, pivot2);
    printVector(nums2, "输入");
    cout << "pivot: " << pivot2 << endl;
    printVector(result2, "输出");
    printVector({-3, 2, 4, 3}, "期望");
    cout << "测试通过: " << (result2 == vector<int>{-3, 2, 4, 3} ? "✓" : "✗") << endl;
    cout << endl;

    // 测试解法四
    Solution4 sol4;
    cout << "========== 解法四：单次遍历优化 ==========" << endl;
    vector<int> nums3 = {1, 2, 3, 4, 5};
    int pivot3 = 3;
    vector<int> result3 = sol4.pivotArray(nums3, pivot3);
    printVector(nums3, "输入");
    cout << "pivot: " << pivot3 << endl;
    printVector(result3, "输出");
    printVector({1, 2, 3, 4, 5}, "期望");
    cout << "测试通过: " << (result3 == vector<int>{1, 2, 3, 4, 5} ? "✓" : "✗") << endl;
    cout << endl;

    cout << "所有测试完成！" << endl;
    return 0;
}
