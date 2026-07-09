#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

/*
 * 560. 和为 K 的子数组
 *
 * 题目：给你一个整数数组 nums 和一个整数 k，请你统计并返回该数组中和为 k 的子数组的个数。
 *
 * 示例：
 *   输入：nums = [1,1,1], k = 2
 *   输出：2
 *
 * 子数组：数组中元素的连续非空序列。
 * 例如：[1,2,3] 的子数组有 [1],[2],[3],[1,2],[2,3],[1,2,3]
 */

// ============================================================================
// 方法一：前缀和 + 哈希表（最优解）
// ============================================================================
// 思路：利用哈希表记录每个前缀和出现的次数，一次遍历即可统计
// 核心公式：子数组 nums[j..i] 的和 = prefixSum[i] - prefixSum[j-1] = k
//         => prefixSum[j-1] = prefixSum[i] - k
// 时间复杂度: O(N)，只需遍历一次数组
// 空间复杂度: O(N)，哈希表最多存储 N+1 个前缀和
class Solution1 {
public:
    int subarraySum(vector<int>& nums, int k) {
        // 哈希表：key = 前缀和，value = 该前缀和出现的次数
        unordered_map<int, int> prefixSumCount;
        // 初始化：前缀和为 0 出现 1 次（空数组）
        prefixSumCount[0] = 1;

        int currentSum = 0;  // 当前前缀和
        int count = 0;       // 满足条件的子数组个数

        for (int num : nums) {
            currentSum += num;  // 更新前缀和

            // 查找有多少个前缀和等于 currentSum - k
            // 每找到一个，就对应一个以当前元素结尾的、和为 k 的子数组
            if (prefixSumCount.find(currentSum - k) != prefixSumCount.end()) {
                count += prefixSumCount[currentSum - k];
            }

            // 将当前前缀和加入哈希表
            prefixSumCount[currentSum]++;
        }

        return count;
    }
};

// ============================================================================
// 方法二：暴力枚举
// ============================================================================
// 思路：枚举每个子数组的起点和终点，在扩展终点的过程中累加和
// 时间复杂度: O(N²)
// 空间复杂度: O(1)
class Solution2 {
public:
    int subarraySum(vector<int>& nums, int k) {
        int count = 0;
        int n = nums.size();

        for (int start = 0; start < n; start++) {
            int sum = 0;
            for (int end = start; end < n; end++) {
                sum += nums[end];          // 以 start 为起点，逐步扩展终点
                if (sum == k) {
                    count++;
                }
            }
        }

        return count;
    }
};

// ============================================================================
// 方法三：前缀和数组 + 两层遍历
// ============================================================================
// 思路：先计算所有前缀和，然后枚举起点和终点，利用前缀和之差求子数组和
// 时间复杂度: O(N²)
// 空间复杂度: O(N)
class Solution3 {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        // prefixSum[i] 表示前 i 个元素的和
        // prefixSum[0] = 0 (空数组)
        // prefixSum[1] = nums[0]
        // prefixSum[i] = nums[0] + ... + nums[i-1]
        vector<int> prefixSum(n + 1, 0);

        // 计算前缀和
        for (int i = 0; i < n; i++) {
            prefixSum[i + 1] = prefixSum[i] + nums[i];
        }

        int count = 0;
        // 枚举子数组的起点和终点
        for (int start = 0; start < n; start++) {
            for (int end = start; end < n; end++) {
                // 子数组 nums[start..end] 的和
                // = prefixSum[end+1] - prefixSum[start]
                if (prefixSum[end + 1] - prefixSum[start] == k) {
                    count++;
                }
            }
        }

        return count;
    }
};

// ============================================================================
// 方法四：前缀和 + 哈希表（完整注释版）
// ============================================================================
// 思路：与方法一相同，增加详细注释
// 时间复杂度: O(N)
// 空间复杂度: O(N)
class Solution4 {
public:
    int subarraySum(vector<int>& nums, int k) {
        // 哈希表记录每个前缀和出现的次数
        unordered_map<int, int> prefixSumCount;

        // 初始状态：前缀和为 0 出现 1 次
        // 这很关键 —— 当 currentSum == k 时，
        // currentSum - k == 0 需要在哈希表中找到匹配
        prefixSumCount[0] = 1;

        int sum = 0;    // 到当前位置为止的前缀和
        int ans = 0;    // 满足条件的子数组数量

        // 遍历数组
        for (const int& x : nums) {
            sum += x;   // 更新前缀和

            // 检查是否存在前缀和 sum - k
            // 如果存在，说明从该位置+1到当前位置的子数组和为 k
            auto it = prefixSumCount.find(sum - k);
            if (it != prefixSumCount.end()) {
                ans += it->second;
            }

            // 将当前前缀和记录到哈希表中
            // 注意：必须在查找之后再记录，避免 k = 0 时重复计数
            prefixSumCount[sum]++;
        }

        return ans;
    }
};

// 辅助函数：打印测试结果
void printResult(const string& testName, const vector<int>& nums, int k, int result) {
    cout << testName << ": nums = [";
    for (int i = 0; i < nums.size(); i++) {
        cout << nums[i];
        if (i < nums.size() - 1) cout << ",";
    }
    cout << "], k = " << k << " → " << result << endl;
}

int main() {
    Solution1 sol1;
    Solution2 sol2;
    Solution3 sol3;
    Solution4 sol4;

    // ============================================================
    // 测试用例
    // ============================================================

    // 测试用例1：基本示例
    vector<int> nums1 = {1, 1, 1};
    int k1 = 2;
    cout << "========== 测试用例1 ==========" << endl;
    printResult("输入", nums1, k1, -1);
    cout << "方法一(前缀和+哈希表): " << sol1.subarraySum(nums1, k1) << endl;
    cout << "方法二(暴力枚举):     " << sol2.subarraySum(nums1, k1) << endl;
    cout << "方法三(前缀和数组):   " << sol3.subarraySum(nums1, k1) << endl;
    cout << "方法四(完整注释版):   " << sol4.subarraySum(nums1, k1) << endl;
    cout << endl;

    // 测试用例2
    vector<int> nums2 = {1, 2, 3};
    int k2 = 3;
    cout << "========== 测试用例2 ==========" << endl;
    printResult("输入", nums2, k2, -1);
    cout << "方法一(前缀和+哈希表): " << sol1.subarraySum(nums2, k2) << endl;
    cout << "方法二(暴力枚举):     " << sol2.subarraySum(nums2, k2) << endl;
    cout << "方法三(前缀和数组):   " << sol3.subarraySum(nums2, k2) << endl;
    cout << "方法四(完整注释版):   " << sol4.subarraySum(nums2, k2) << endl;
    cout << endl;

    // 测试用例3：包含负数
    vector<int> nums3 = {-1, -1, 1};
    int k3 = 0;
    cout << "========== 测试用例3 ==========" << endl;
    printResult("输入", nums3, k3, -1);
    cout << "方法一(前缀和+哈希表): " << sol1.subarraySum(nums3, k3) << endl;
    cout << "方法二(暴力枚举):     " << sol2.subarraySum(nums3, k3) << endl;
    cout << "方法三(前缀和数组):   " << sol3.subarraySum(nums3, k3) << endl;
    cout << "方法四(完整注释版):   " << sol4.subarraySum(nums3, k3) << endl;
    cout << endl;

    // 测试用例4：边界 —— 没有满足条件的子数组
    vector<int> nums4 = {1};
    int k4 = 0;
    cout << "========== 测试用例4 ==========" << endl;
    printResult("输入", nums4, k4, -1);
    cout << "方法一(前缀和+哈希表): " << sol1.subarraySum(nums4, k4) << endl;
    cout << "方法二(暴力枚举):     " << sol2.subarraySum(nums4, k4) << endl;
    cout << "方法三(前缀和数组):   " << sol3.subarraySum(nums4, k4) << endl;
    cout << "方法四(完整注释版):   " << sol4.subarraySum(nums4, k4) << endl;
    cout << endl;

    // 测试用例5：包含负数，多个可能
    vector<int> nums5 = {-1, 1, 0};
    int k5 = 0;
    cout << "========== 测试用例5 ==========" << endl;
    printResult("输入", nums5, k5, -1);
    cout << "方法一(前缀和+哈希表): " << sol1.subarraySum(nums5, k5) << endl;
    cout << "方法二(暴力枚举):     " << sol2.subarraySum(nums5, k5) << endl;
    cout << "方法三(前缀和数组):   " << sol3.subarraySum(nums5, k5) << endl;
    cout << "方法四(完整注释版):   " << sol4.subarraySum(nums5, k5) << endl;
    cout << endl;

    // 测试用例6：单元素数组
    vector<int> nums6 = {1};
    int k6 = 1;
    cout << "========== 测试用例6 ==========" << endl;
    printResult("输入", nums6, k6, -1);
    cout << "方法一(前缀和+哈希表): " << sol1.subarraySum(nums6, k6) << endl;
    cout << "方法二(暴力枚举):     " << sol2.subarraySum(nums6, k6) << endl;
    cout << "方法三(前缀和数组):   " << sol3.subarraySum(nums6, k6) << endl;
    cout << "方法四(完整注释版):   " << sol4.subarraySum(nums6, k6) << endl;

    return 0;
}
