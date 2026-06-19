#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

/*
 * 42. 接雨水
 *
 * 主解法：双指针
 *
 * 对于下标 i，它上方能够接到的雨水量是：
 *
 *   water[i] = min(左边最高柱子, 右边最高柱子) - height[i]
 *
 * 双指针从数组两端向中间移动：
 *
 *   left     ：当前等待处理的左侧位置
 *   right    ：当前等待处理的右侧位置
 *   leftMax  ：区间 [0, left] 中最高柱子的高度
 *   rightMax ：区间 [right, n - 1] 中最高柱子的高度
 *   water    ：已经确定并累加的雨水总量
 *
 * 当 height[left] <= height[right] 时：
 *   右边至少存在 height[right] 这根不低于 height[left] 的柱子。
 *   因此，left 位置能接多少水只取决于 leftMax。
 *
 * 当 height[left] > height[right] 时：
 *   左边至少存在 height[left] 这根高于 height[right] 的柱子。
 *   因此，right 位置能接多少水只取决于 rightMax。
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */

class Solution {
public:
    int trap(vector<int>& height) {
        /*
         * 少于 3 根柱子时，不可能形成“左边界 + 低洼处 + 右边界”，
         * 所以一定接不到雨水。
         */
        if (height.size() < 3) {
            return 0;
        }

        /*
         * left 和 right 分别指向当前尚未处理区间的两端。
         */
        int left = 0;
        int right = static_cast<int>(height.size()) - 1;

        /*
         * leftMax：从数组最左端到 left，出现过的最高柱子。
         * rightMax：从 right 到数组最右端，出现过的最高柱子。
         *
         * 初始时，两边都还没有处理任何柱子，所以设为 0。
         */
        int leftMax = 0;
        int rightMax = 0;

        /*
         * water 保存已经计算出的雨水总量。
         */
        int water = 0;

        /*
         * 每一轮至少处理 left 或 right 中的一个位置，
         * 所以 left 和 right 最终一定会相遇。
         */
        while (left < right) {
            if (height[left] <= height[right]) {
                /*
                 * 当前左柱不高于当前右柱。
                 *
                 * 此时可以确定：left 的右侧至少存在一根高度为
                 * height[right] 的柱子，并且：
                 *
                 *   height[right] >= height[left]
                 *
                 * 因此，当前 left 位置是否能接水，以及能接多少水，
                 * 可以由左侧最高边界 leftMax 决定。
                 */

                /*
                 * 把当前柱子纳入左侧最高值。
                 *
                 * 如果 height[left] 刷新了最高值，那么：
                 *   leftMax == height[left]
                 *   本位置接水量为 0。
                 *
                 * 如果没有刷新最高值，那么：
                 *   leftMax > height[left]
                 *   高度差 leftMax - height[left] 就是本位置的水量。
                 */
                leftMax = max(leftMax, height[left]);

                /*
                 * 累加当前 left 位置上方的雨水。
                 *
                 * 因为刚刚执行了 max，所以这里永远不会得到负数。
                 */
                water += leftMax - height[left];

                /*
                 * left 位置已经计算完成，向中间移动。
                 */
                left++;
            } else {
                /*
                 * 当前左柱高于当前右柱。
                 *
                 * 此时可以确定：right 的左侧至少存在一根高度为
                 * height[left] 的柱子，并且：
                 *
                 *   height[left] > height[right]
                 *
                 * 因此，当前 right 位置能接多少水可以由右侧最高边界
                 * rightMax 决定。
                 */

                /*
                 * 把当前柱子纳入右侧最高值。
                 */
                rightMax = max(rightMax, height[right]);

                /*
                 * 累加当前 right 位置上方的雨水。
                 */
                water += rightMax - height[right];

                /*
                 * right 位置已经计算完成，向中间移动。
                 */
                right--;
            }
        }

        return water;
    }
};

/*
 * 以下是本地测试代码。
 * 在 LeetCode 提交时，只需要提交上面的 Solution 类。
 */
void printVector(const vector<int>& nums) {
    cout << "[";
    for (int i = 0; i < static_cast<int>(nums.size()); i++) {
        cout << nums[i];
        if (i + 1 < static_cast<int>(nums.size())) {
            cout << ",";
        }
    }
    cout << "]";
}

void runTest(vector<int> height, int expected) {
    Solution solution;
    int actual = solution.trap(height);

    cout << "height = ";
    printVector(height);
    cout << ", expected = " << expected;
    cout << ", actual = " << actual;
    cout << (actual == expected ? "  OK" : "  FAIL") << '\n';
}

int main() {
    runTest({0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6);
    runTest({4, 2, 0, 3, 2, 5}, 9);
    runTest({2, 0, 2}, 2);
    runTest({3, 2, 1}, 0);
    runTest({1, 2, 3, 4}, 0);
    runTest({3, 3, 3}, 0);
    runTest({}, 0);
    runTest({5}, 0);

    return 0;
}
