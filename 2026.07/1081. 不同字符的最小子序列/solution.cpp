#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string smallestSubsequence(string s) {
        // remaining[x]：当前扫描位置之后，还剩多少个字符 x。
        array<int, 26> remaining{};
        for (char c : s) {
            ++remaining[c - 'a'];
        }

        // inAnswer[x]：字符 x 当前是否已经在答案中。
        array<bool, 26> inAnswer{};
        string answer;

        for (char c : s) {
            const int current = c - 'a';
            --remaining[current];

            // 已经选过的字符不能再次加入，否则会破坏“恰好一次”。
            if (inAnswer[current]) {
                continue;
            }

            // 若栈顶比 c 大，并且后面还能重新找到栈顶字符，
            // 就先弹出栈顶，让较小的 c 尽量靠前。
            while (!answer.empty() &&
                   answer.back() > c &&
                   remaining[answer.back() - 'a'] > 0) {
                inAnswer[answer.back() - 'a'] = false;
                answer.pop_back();
            }

            answer.push_back(c);
            inAnswer[current] = true;
        }

        return answer;
    }
};
