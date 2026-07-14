#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findCheapestPrice(
        int n,
        vector<vector<int>>& flights,
        int src,
        int dst,
        int k
    ) {
        // INF 表示“在当前允许的航线条数内，还无法到达这座城市”。
        //
        // 题目中的合法总费用远小于 1e9，因此可以安全地把 1e9
        // 当作无穷大。后面只有 previousCost[from] != INF 时才做加法，
        // 也避免了 INF + price 造成无意义的计算或整数溢出风险。
        const int INF = 1'000'000'000;

        // k 表示最多允许经过 k 个中转站。
        //
        // 一条完整路线中：
        // 0 个中转站 -> 最多乘坐 1 趟航班，也就是最多使用 1 条边；
        // 1 个中转站 -> 最多乘坐 2 趟航班，也就是最多使用 2 条边；
        // k 个中转站 -> 最多乘坐 k + 1 趟航班，也就是最多使用 k + 1 条边。
        const int maxEdges = k + 1;

        // previousCost[city] 表示：
        // 在“上一轮允许的最大航线条数”以内，从 src 到 city 的最低费用。
        //
        // 初始化时还没有使用任何航线：
        // - 从 src 到 src 的费用是 0；
        // - 其他城市都暂时无法到达，所以费用是 INF。
        vector<int> previousCost(n, INF);
        previousCost[src] = 0;

        // edgesUsed 表示当前这一轮最多允许使用多少条航线。
        //
        // 第 1 轮：最多使用 1 条航线；
        // 第 2 轮：最多使用 2 条航线；
        // ...
        // 第 maxEdges 轮：最多使用 k + 1 条航线。
        for (int edgesUsed = 1; edgesUsed <= maxEdges; ++edgesUsed) {
            // currentCost 先复制 previousCost。
            //
            // 复制的含义是：上一轮已经找到的便宜路线，这一轮仍然可以使用。
            // 因此 currentCost 保存的是“最多使用 edgesUsed 条航线”的答案，
            // 而不是“必须恰好使用 edgesUsed 条航线”的答案。
            vector<int> currentCost = previousCost;

            // 枚举每一趟有向航班 flight：from -> to，票价为 price。
            for (const vector<int>& flight : flights) {
                const int from = flight[0];
                const int to = flight[1];
                const int price = flight[2];

                // 上一轮在允许的航线条数内还无法到达 from，
                // 就不能从 from 继续乘坐本趟航班到达 to。
                if (previousCost[from] == INF) {
                    continue;
                }

                // 必须使用 previousCost[from]，不能使用 currentCost[from]。
                //
                // 这样可以保证本轮只在“上一轮最多 edgesUsed - 1 条边”的
                // 路线上再接一条航线，得到的路线最多只有 edgesUsed 条边。
                // 如果这里使用 currentCost[from]，同一轮刚更新出的结果可能被
                // 立即再次使用，就可能在一轮内错误地串联多条航线，突破 k 限制。
                const int candidateCost = previousCost[from] + price;

                // currentCost[to] 可能来自两种路线：
                // 1. 不乘坐当前航班，保留原来最多 edgesUsed - 1 条边的最低费用；
                // 2. 先到达 from，再乘坐 from -> to，费用是 candidateCost。
                // 取二者较小值，就是当前最多使用 edgesUsed 条边时到达 to 的最低费用。
                currentCost[to] = min(currentCost[to], candidateCost);
            }

            // 本轮结束后，currentCost 已经完整记录了
            // “最多使用 edgesUsed 条航线”的所有最低费用。
            // 把它交给 previousCost，供下一轮继续使用。
            previousCost = move(currentCost);
        }

        // 完成 maxEdges = k + 1 轮后：
        // previousCost[dst] 就是在最多经过 k 个中转站的条件下，
        // 从 src 到 dst 的最低费用。
        //
        // 如果仍然是 INF，说明限制范围内无法到达 dst，按题意返回 -1。
        return previousCost[dst] == INF ? -1 : previousCost[dst];
    }
};
