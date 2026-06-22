#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

/*
 * 1840. 最高建筑高度
 *
 * 题目规则：
 *   1. 第 1 栋建筑的高度必须是 0。
 *   2. 所有建筑高度都是非负整数。
 *   3. 相邻建筑的高度差不能超过 1。
 *   4. restrictions[i] = [id, maxHeight] 表示：
 *        第 id 栋建筑的高度不能超过 maxHeight。
 *
 * 目标：
 *   在满足所有规则的前提下，让某一栋建筑尽可能高，
 *   返回能够达到的最大建筑高度。
 *
 * 核心思路：
 *
 *   第一步：加入题目隐含的限制点 [1, 0]。
 *
 *   第二步：按建筑编号排序，并合并重复编号。
 *
 *   第三步：收紧每个限制点的真实高度上限。
 *
 *     从左往右：
 *       当前限制点最多只能比左边限制点高 distance。
 *
 *       limit[i] = min(limit[i],
 *                      limit[i - 1] + id[i] - id[i - 1])
 *
 *     从右往左：
 *       当前限制点最多只能比右边限制点高 distance。
 *
 *       limit[i] = min(limit[i],
 *                      limit[i + 1] + id[i + 1] - id[i])
 *
 *   第四步：计算每两个相邻限制点之间能够形成的最高山峰。
 *
 *     左端点为 (leftId, leftHeight)
 *     右端点为 (rightId, rightHeight)
 *     距离 distance = rightId - leftId
 *
 *     区间最高高度：
 *
 *       peak = floor((leftHeight + rightHeight + distance) / 2)
 *
 *     最后一个限制点右侧没有其他限制，可以每走一步增加 1：
 *
 *       tailPeak = lastHeight + (n - lastId)
 *
 * 时间复杂度：O(m log m)
 * 空间复杂度：O(m)
 *
 * m 是 restrictions 的数量。
 */

class Solution {
public:
    int maxBuilding(int n, vector<vector<int>>& restrictions) {
        /*
         * points 中的每个元素都是：
         *
         *   [建筑编号 id, 当前允许的最大高度 limit]
         *
         * 使用 long long 是为了防止下面这些加法溢出 int：
         *
         *   limit + distance
         *   leftHeight + rightHeight + distance
         */
        vector<vector<long long>> points;

        /*
         * 第 1 栋建筑高度必须为 0。
         *
         * 虽然题目通常不会在 restrictions 中显式给出 [1, 0]，
         * 但它是一个真实存在的限制，所以主动加入。
         */
        points.push_back({1, 0});

        /*
         * 把输入限制复制到 points。
         */
        for (const vector<int>& restriction : restrictions) {
            long long id = restriction[0];
            long long maxHeight = restriction[1];
            points.push_back({id, maxHeight});
        }

        /*
         * 按建筑编号从小到大排序。
         *
         * 排序后，相邻限制点才对应建筑编号轴上的相邻区间，
         * 才能进行左右传播和区间峰值计算。
         */
        sort(points.begin(), points.end());

        /*
         * 合并重复建筑编号。
         *
         * 如果同一栋建筑有多个高度上限，例如：
         *
         *   [5, 7]
         *   [5, 3]
         *
         * 那么真正有效的上限是更严格的 3。
         *
         * LeetCode 原题通常保证建筑编号互不相同，这段合并仍能让
         * 本地代码更健壮，也能正确处理我们额外加入的 [1, 0]。
         */
        vector<vector<long long>> merged;

        for (const vector<long long>& point : points) {
            if (!merged.empty() && merged.back()[0] == point[0]) {
                merged.back()[1] = min(merged.back()[1], point[1]);
            } else {
                merged.push_back(point);
            }
        }

        points.swap(merged);

        int m = static_cast<int>(points.size());

        /*
         * 第一遍传播：从左往右收紧限制。
         *
         * 假设左边限制点是：
         *
         *   (leftId, leftLimit)
         *
         * 当前限制点是：
         *
         *   (curId, curLimit)
         *
         * 两点距离：
         *
         *   distance = curId - leftId
         *
         * 因为相邻建筑每次最多升高 1，所以从 leftId 走到 curId，
         * 最多只能总共升高 distance。
         *
         * 因此当前点还必须满足：
         *
         *   curHeight <= leftLimit + distance
         *
         * 把输入上限和左侧传播来的上限取较小值。
         */
        for (int i = 1; i < m; i++) {
            long long distance = points[i][0] - points[i - 1][0];
            long long limitFromLeft = points[i - 1][1] + distance;

            points[i][1] = min(points[i][1], limitFromLeft);
        }

        /*
         * 第二遍传播：从右往左收紧限制。
         *
         * 假设右边限制点是：
         *
         *   (rightId, rightLimit)
         *
         * 当前限制点距离右边 distance。
         *
         * 从右边往左走时，每一步最多升高 1，所以当前点必须满足：
         *
         *   curHeight <= rightLimit + distance
         *
         * 再次取较小值，得到同时满足左右约束的最终有效上限。
         */
        for (int i = m - 2; i >= 0; i--) {
            long long distance = points[i + 1][0] - points[i][0];
            long long limitFromRight = points[i + 1][1] + distance;

            points[i][1] = min(points[i][1], limitFromRight);
        }

        /*
         * answer 保存目前找到的最高建筑高度。
         *
         * 第 1 栋建筑高度是 0，所以初始答案设为 0。
         */
        long long answer = 0;

        /*
         * 枚举每一对相邻限制点，计算这段区间能达到的最高峰。
         */
        for (int i = 1; i < m; i++) {
            long long leftId = points[i - 1][0];
            long long leftHeight = points[i - 1][1];
            long long rightId = points[i][0];
            long long rightHeight = points[i][1];

            /*
             * 两个限制点之间的横向距离。
             */
            long long distance = rightId - leftId;

            /*
             * 区间峰值公式：
             *
             *   peak = floor((leftHeight + rightHeight + distance) / 2)
             *
             * 直观理解：
             *
             *   从左端点开始，每向右一步最多升高 1；
             *   从右端点开始，每向左一步最多升高 1。
             *
             * 两条上升线在中间相遇时达到区间最高值。
             *
             * C++ 整数除法会自动向下取整，所以直接除以 2 即可。
             */
            long long peak =
                (leftHeight + rightHeight + distance) / 2;

            answer = max(answer, peak);
        }

        /*
         * 最后一个限制点右侧一直到第 n 栋建筑之间，没有更多显式限制。
         *
         * 因此可以从最后限制点开始，每向右一栋就增加 1。
         *
         * lastHeight + (n - lastId)
         *
         * 就是尾部能够达到的最高高度。
         */
        long long lastId = points.back()[0];
        long long lastHeight = points.back()[1];
        long long tailPeak = lastHeight + (static_cast<long long>(n) - lastId);

        answer = max(answer, tailPeak);

        return static_cast<int>(answer);
    }
};

/*
 * 以下是本地测试代码。
 *
 * LeetCode 提交时，只需要提交上面的 Solution 类。
 */
void printRestrictions(const vector<vector<int>>& restrictions) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(restrictions.size()); i++) {
        cout << "[" << restrictions[i][0] << "," << restrictions[i][1] << "]";

        if (i + 1 < static_cast<int>(restrictions.size())) {
            cout << ",";
        }
    }

    cout << "]";
}

void runTest(
    int n,
    vector<vector<int>> restrictions,
    int expected
) {
    Solution solution;
    int actual = solution.maxBuilding(n, restrictions);

    cout << "n = " << n << ", restrictions = ";
    printRestrictions(restrictions);
    cout << ", expected = " << expected;
    cout << ", actual = " << actual;
    cout << (actual == expected ? "  OK" : "  FAIL") << '\n';
}

int main() {
    // 官方示例 1。
    runTest(5, {{2, 1}, {4, 1}}, 2);

    // 没有额外限制，可以从第 1 栋的 0 一直升到第 6 栋的 5。
    runTest(6, {}, 5);

    // 官方示例 3。
    runTest(10, {{5, 3}, {2, 5}, {7, 4}, {10, 3}}, 5);

    // 一个限制点，限制点之后仍可继续升高。
    runTest(8, {{4, 2}}, 6);

    // 两端高度相同，中间形成山峰。
    runTest(7, {{3, 1}, {7, 1}}, 3);

    // 输入顺序混乱，验证排序逻辑。
    runTest(9, {{8, 2}, {3, 2}, {6, 3}}, 4);

    // 重复限制点，取更严格的上限。
    runTest(6, {{4, 4}, {4, 1}}, 3);

    return 0;
}
