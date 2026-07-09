#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

/*
 * 1833. 雪糕的最大数量
 *
 * 题目：
 *   costs[i] 表示第 i 支雪糕的价格。
 *   手里一共有 coins 枚硬币。
 *   每支雪糕最多购买一次。
 *   要求在总花费不超过 coins 的前提下，购买尽可能多的雪糕。
 *
 * 核心思路：排序 + 贪心
 *
 *   1. 把所有雪糕价格从小到大排序。
 *   2. 从最便宜的雪糕开始购买。
 *   3. 如果当前剩余硬币 coins 足够支付 cost：
 *        coins -= cost;
 *        count++;
 *   4. 如果连当前最便宜的未购买雪糕都买不起：
 *        后面的雪糕价格只会更高，所以直接结束循环。
 *
 * 为什么先买便宜的一定最优？
 *
 *   目标是“购买数量最多”，不是“花的钱最多”。
 *
 *   对于任意要购买的 k 支雪糕，价格最小的 k 支雪糕的总价，
 *   一定不大于其他任意 k 支雪糕的总价。
 *
 *   因此：
 *     - 如果最便宜的 k 支都买不起，那么任何 k 支都买不起。
 *     - 如果最便宜的 k 支买得起，就至少能购买 k 支。
 *
 *   所以按价格从小到大购买，可以得到最大数量。
 *
 * 时间复杂度：O(n log n)
 *   排序需要 O(n log n)，之后遍历需要 O(n)。
 *
 * 空间复杂度：O(log n)
 *   取决于 C++ sort 内部使用的递归栈。
 *   如果只计算题目要求的额外数组空间，可以认为没有新建 O(n) 数组。
 */

class Solution {
public:
    int maxIceCream(vector<int>& costs, int coins) {
        /*
         * 第一步：按照价格从小到大排序。
         *
         * 例如：
         *   排序前 costs = [1,3,2,4,1]
         *   排序后 costs = [1,1,2,3,4]
         *
         * 排序后，越靠前的雪糕越便宜。
         */
        sort(costs.begin(), costs.end());

        /*
         * count 表示已经成功购买的雪糕数量。
         */
        int count = 0;

        /*
         * 从最便宜的雪糕开始依次尝试购买。
         *
         * cost 表示当前雪糕的价格。
         */
        for (int cost : costs) {
            /*
             * 如果剩余硬币少于当前价格，就买不起当前雪糕。
             *
             * 因为 costs 已经升序排列，后面的价格满足：
             *
             *   后面的价格 >= 当前 cost
             *
             * 所以后面的雪糕也全部买不起，可以直接结束循环。
             */
            if (coins < cost) {
                break;
            }

            /*
             * 当前雪糕可以买：
             *
             *   1. 从剩余硬币中扣除它的价格。
             *   2. 已购买数量增加 1。
             */
            coins -= cost;
            count++;
        }

        /*
         * 返回最多可以买到的雪糕数量。
         */
        return count;
    }
};

/*
 * 以下是本地测试代码。
 *
 * LeetCode 提交时，只需要提交上面的 Solution 类。
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

void runTest(vector<int> costs, int coins, int expected) {
    /*
     * maxIceCream 会原地排序 costs。
     * 为了输出原始输入，这里提前复制一份。
     */
    vector<int> originalCosts = costs;

    Solution solution;
    int actual = solution.maxIceCream(costs, coins);

    cout << "costs = ";
    printVector(originalCosts);
    cout << ", coins = " << coins;
    cout << ", expected = " << expected;
    cout << ", actual = " << actual;
    cout << (actual == expected ? "  OK" : "  FAIL") << '\n';
}

int main() {
    // 官方示例 1：可以买 1、1、2、3，共 4 支。
    runTest({1, 3, 2, 4, 1}, 7, 4);

    // 官方示例 2：最便宜的雪糕也需要 6 枚硬币，一支都买不起。
    runTest({10, 6, 8, 7, 7, 8}, 5, 0);

    // 官方示例 3：总价正好是 18，不超过 20，可以全部买下。
    runTest({1, 6, 3, 1, 2, 5}, 20, 6);

    // 正好花完预算。
    runTest({2, 2, 3}, 4, 2);

    // 有剩余硬币，但仍买不起下一支。
    runTest({1, 2, 5, 6}, 4, 2);

    // 所有雪糕价格相同。
    runTest({3, 3, 3, 3}, 10, 3);

    // 只有一支且可以买到。
    runTest({5}, 5, 1);

    // 只有一支但买不起。
    runTest({5}, 4, 0);

    return 0;
}
