#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = (int)nums.size();

        // answer 存放每一个长度为 k 的滑动窗口最大值。
        // 一共有 n - k + 1 个窗口，所以提前 reserve 可以减少动态扩容。
        vector<int> answer;
        answer.reserve(n - k + 1);

        // window 存放的是 nums 的下标，不是直接存数字。
        //
        // 它始终满足两个性质：
        // 1. 下标从队头到队尾递增，表示这些元素都按出现顺序排好。
        // 2. nums[下标] 从队头到队尾单调递减。
        //
        // 因此，window.front() 永远指向当前窗口中的最大值下标。
        deque<int> window;

        for (int right = 0; right < n; ++right) {
            // left 是当前窗口的左边界。
            // 当 right < k - 1 时，窗口还没有凑满 k 个元素，left 会是负数。
            // 这没有关系，因为只有 right >= k - 1 时才会记录答案。
            int left = right - k + 1;

            // 第一步：删除已经滑出窗口左侧的下标。
            //
            // 如果 window.front() < left，说明这个下标已经不在 [left, right] 中，
            // 它不能再参与当前窗口最大值的判断。
            if (!window.empty() && window.front() < left) {
                window.pop_front();
            }

            // 第二步：维护单调递减。
            //
            // 当前新元素 nums[right] 即将进入队列。
            // 如果队尾下标 last 对应的值 nums[last] <= nums[right]，
            // 那么 last 永远不可能成为之后窗口的最大值：
            // - nums[right] 更大或相等；
            // - right 的位置更靠右，生命周期更长。
            // 所以可以把队尾这些“没机会”的下标全部弹出。
            while (!window.empty() && nums[window.back()] <= nums[right]) {
                window.pop_back();
            }

            // 第三步：把当前元素下标加入队尾。
            // 经过上面的弹出后，加入 right 仍能保持 nums[window] 单调递减。
            window.push_back(right);

            // 第四步：当窗口已经形成，也就是 right >= k - 1 时，记录答案。
            //
            // 因为 window.front() 是当前窗口 [left, right] 中最大值的下标，
            // 所以 nums[window.front()] 就是当前窗口最大值。
            if (right >= k - 1) {
                answer.push_back(nums[window.front()]);
            }
        }

        return answer;
    }
};
