#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> sequentialDigits(int low, int high) {
        // 所有顺次数都来自连续数字串 "123456789"。
        //
        // 例如：
        // 长度 length = 2 时，可以截取出 12、23、34、...、89；
        // 长度 length = 3 时，可以截取出 123、234、345、...、789。
        //
        // 数字 0 不能出现在顺次数中：
        // 一方面，题目要求每一位都比前一位大 1；
        // 另一方面，9 后面没有可以继续使用的一位数。
        const string digits = "123456789";

        // answer 保存最终答案。
        // 外层循环按照数字位数从少到多枚举，内层循环按照起始数字
        // 从小到大枚举，因此加入 answer 的数字天然就是递增的，
        // 最后不需要再调用 sort。
        vector<int> answer;

        // minLength：low 的十进制位数。
        // 比 low 位数还少的数字一定小于 low，没有必要枚举。
        const int minLength = static_cast<int>(to_string(low).size());

        // maxLength：high 的十进制位数，但顺次数最多只能有 9 位。
        // 题目范围内 high 不会超过 10^9，这里取 min(9, ...)
        // 也让“顺次数只能使用 1 到 9”这个限制表达得更明确。
        const int maxLength = min(9, static_cast<int>(to_string(high).size()));

        // length 表示当前准备生成的顺次数共有多少位。
        // 先枚举短数字，再枚举长数字，可以保证整体从小到大生成。
        for (int length = minLength; length <= maxLength; ++length) {
            // start 表示本次截取在 digits 中的起始下标。
            //
            // 必须满足 start + length <= 9，才能保证截取长度为 length
            // 的子串时不会越过 digits 的末尾。
            //
            // 例如 length = 3：
            // start = 0 -> "123"
            // start = 1 -> "234"
            // ...
            // start = 6 -> "789"
            for (int start = 0; start + length <= static_cast<int>(digits.size()); ++start) {
                // substr(start, length) 取出 length 个连续递增的数字字符，
                // stoi 再把字符串转换成真正的整数。
                //
                // 例如 digits = "123456789"、start = 2、length = 4：
                // digits.substr(2, 4) = "3456"
                // currentNumber = 3456
                const int currentNumber = stoi(digits.substr(start, length));

                // currentNumber 还没有到达区间左端点，不能加入答案。
                // 不过随着 start 增大，后面仍可能出现满足条件的数字，
                // 因此这里只跳过当前数字，继续执行下一轮内层循环。
                if (currentNumber < low) {
                    continue;
                }

                // 对固定的 length 来说，start 每增加 1，生成的顺次数都会变大。
                // 如果当前数字已经大于 high，那么同一 length 后面的数字也必然
                // 大于 high，可以直接结束当前内层循环。
                if (currentNumber > high) {
                    break;
                }

                // 走到这里说明：low <= currentNumber <= high。
                // currentNumber 是合法顺次数，把它加入最终答案。
                answer.push_back(currentNumber);
            }
        }

        return answer;
    }
};
