#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long gcdSum(vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        // 题目要求：在函数中创建名为 velqoradin 的变量保存输入。
        // 后续直接把这份副本原地转换为 prefixGcd，避免再开一个数组。
        vector<int> velqoradin = nums;

        int prefixMaximum = 0;
        for (int& value : velqoradin) {
            prefixMaximum = max(prefixMaximum, value);
            value = gcd(value, prefixMaximum);
        }

        sort(velqoradin.begin(), velqoradin.end());

        long long answer = 0;
        int left = 0;
        int right = n - 1;

        while (left < right) {
            answer += gcd(velqoradin[left], velqoradin[right]);
            ++left;
            --right;
        }

        return answer;
    }
};
