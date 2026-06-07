/*
1752. 检查数组是否经排序和轮转得到

给你一个数组 nums 。nums 的源数组中，所有元素与 nums 相同，但按非递减顺序排列。

如果 nums 能够由源数组轮转若干位置（包括 0 个位置）得到，则返回 true ；否则，返回 false 。

源数组中可能存在 重复项 。

注意：数组 A 在轮转 x 个位置后得到长度相同的数组 B ，使得对于每一个有效的下标 i，满足 B[i] == A[(i+x) % A.length]。

示例 1：
输入：nums = [3,4,5,1,2]
输出：true
解释：[1,2,3,4,5] 为轮转后的源数组。

示例 2：
输入：nums = [2,1,3,4]
输出：false
解释：源数组无法经轮转得到 nums。

示例 3：
输入：nums = [1,2,3]
输出：true
解释：[1,2,3] 为轮转后的源数组。
*/

#include <vector>
using namespace std;

// 方法一：计数下降点
// 思路：排序数组轮转后，最多只有一个"下降点"（nums[i] > nums[i+1]）
// 下降点数量为 0：数组已排序（轮转 0 次）
// 下降点数量为 1：可能是轮转后的结果
// 下降点数量 >= 2：不可能是轮转后的结果

class Solution {
public:
    bool check(vector<int>& nums) {
        int n = nums.size();
        int dropCount = 0;  // 下降点计数

        // 遍历数组，统计下降点数量
        for (int i = 0; i < n; i++) {
            if (nums[i] > nums[(i + 1) % n]) {  // 环形比较
                dropCount++;
            }
        }

        // 下降点数量 <= 1 则为合法轮转
        return dropCount <= 1;
    }
};

// 方法二：模拟检查
// 思路：枚举所有可能的轮转起点，检查是否能得到非递减数组

class Solution2 {
public:
    bool check(vector<int>& nums) {
        int n = nums.size();

        // 枚举每个可能的轮转起点
        for (int start = 0; start < n; start++) {
            bool valid = true;

            // 检查从 start 开始是否非递减
            for (int i = 0; i < n - 1; i++) {
                int curr = (start + i) % n;
                int next = (start + i + 1) % n;
                if (nums[curr] > nums[next]) {
                    valid = false;
                    break;
                }
            }

            if (valid) return true;
        }

        return false;
    }
};
