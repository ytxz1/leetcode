#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long sumScores(string s) {
        const int n = static_cast<int>(s.size());
        vector<int> z(n);

        // 整个字符串与自身的最长公共前缀长度就是 n。
        z[0] = n;

        // [left, right] 是当前右端点最靠右的 Z-box：
        // s[left..right] == s[0..right-left]。
        int left = 0;
        int right = 0;

        for (int i = 1; i < n; ++i) {
            // i 在已有 Z-box 内时，先复用已经计算过的匹配信息。
            if (i <= right) {
                z[i] = min(z[i - left], right - i + 1);
            }

            // 只有尚未确定的部分才需要继续逐字符比较。
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
                ++z[i];
            }

            // 产生正长度匹配且区间伸得更远时，更新最右 Z-box。
            // z[i] == 0 时没有匹配区间，不能把空区间当成 Z-box。
            if (z[i] > 0 && i + z[i] - 1 > right) {
                left = i;
                right = i + z[i] - 1;
            }
        }

        long long answer = 0;
        for (int length : z) {
            answer += length;
        }
        return answer;
    }
};
