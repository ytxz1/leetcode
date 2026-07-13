#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> arrayRankTransform(vector<int>& arr) {
        // sortedValues 是 arr 的一个副本。
        //
        // 为什么要复制一份？
        // 因为最终答案必须按照 arr 原来的顺序返回，
        // 如果直接排序 arr，就会破坏原数组中每个数字的位置。
        vector<int> sortedValues = arr;

        // 把副本从小到大排序。
        //
        // 排序之后，越小的数字越靠前。
        // 数组序号 rank 的规则正好也是：
        // 最小的不同数字 rank = 1，第二小的不同数字 rank = 2，以此类推。
        sort(sortedValues.begin(), sortedValues.end());

        // valueToRank[value] 表示数字 value 对应的序号 rank。
        //
        // 例如 arr = [40, 10, 20, 30]，
        // 排序后是 [10, 20, 30, 40]，
        // 那么映射就是：
        // valueToRank[10] = 1
        // valueToRank[20] = 2
        // valueToRank[30] = 3
        // valueToRank[40] = 4
        unordered_map<int, int> valueToRank;

        // nextRank 表示下一个“新数字”应该拿到的序号。
        //
        // 题目要求序号从 1 开始，所以初始值是 1。
        int nextRank = 1;

        for (int value : sortedValues) {
            // sortedValues 里可能有重复数字。
            //
            // 例如 arr = [100, 100, 100]，
            // sortedValues 也是 [100, 100, 100]。
            //
            // 相同的数字必须拥有相同的 rank，
            // 所以只有当 value 第一次出现时，才给它分配新的 rank。
            if (!valueToRank.count(value)) {
                valueToRank[value] = nextRank;
                ++nextRank;
            }
        }

        // answer[i] 表示 arr[i] 转换后的序号。
        vector<int> answer;
        answer.reserve(arr.size());

        for (int value : arr) {
            // 按照 arr 的原始顺序，把每个数字替换成它对应的 rank。
            answer.push_back(valueToRank[value]);
        }

        return answer;
    }
};
