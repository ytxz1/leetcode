#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxActiveSectionsAfterTrade(string s) {
        int ones = 0;
        int bestGain = 0;
        int previousZeroLen = -1000000000;

        for (int i = 0, n = (int)s.size(); i < n; ) {
            int j = i;
            while (j < n && s[j] == s[i]) {
                ++j;
            }

            int len = j - i;
            if (s[i] == '1') {
                ones += len;
            } else {
                bestGain = max(bestGain, previousZeroLen + len);
                previousZeroLen = len;
            }

            i = j;
        }

        return ones + bestGain;
    }
};

#ifdef LOCAL
int main() {
    vector<pair<string, int>> tests = {
        {"01", 1},
        {"0100", 4},
        {"1000100", 7},
        {"01010", 4},
        {"0", 0},
        {"1", 1},
        {"0000", 0},
        {"1111", 4},
        {"00100", 5},
        {"0011100110", 9},
        {"10101", 5},
        {"0100010100", 7}
    };

    Solution sol;
    for (auto &[s, expected] : tests) {
        int actual = sol.maxActiveSectionsAfterTrade(s);
        cout << "s = " << s << ", answer = " << actual
             << ", expected = " << expected << '\n';
        assert(actual == expected);
    }
    cout << "All tests passed.\n";
    return 0;
}
#endif
