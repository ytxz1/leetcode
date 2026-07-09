#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

/*
 * 2130. 链表最大孪生和
 *
 * 题目：
 *   给定一个长度为偶数的链表，链表第 i 个节点和第 n - 1 - i 个节点互为孪生节点。
 *   两个孪生节点的值之和叫做孪生和。
 *   返回链表中的最大孪生和。
 *
 * 核心思路：
 *   1. 用快慢指针找到链表后半段的起点。
 *   2. 原地反转后半段链表。
 *   3. 前半段从 head 开始，反转后的后半段从 secondHalf 开始，同时向后走。
 *      每一步计算 first->val + second->val，并更新最大值。
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
public:
    int pairSum(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        // 链表长度为偶数。fast 每次走两步，slow 每次走一步。
        // 当 fast 走到末尾时，slow 正好指向后半段的第一个节点。
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // 反转后半段链表。
        ListNode* secondHalf = reverseList(slow);

        int ans = 0;
        ListNode* first = head;
        ListNode* second = secondHalf;

        // second 只覆盖后半段，长度正好是 n / 2。
        // 每一步对应一组孪生节点：
        // first:  第 0, 1, 2, ... 个节点
        // second: 第 n-1, n-2, n-3, ... 个节点
        while (second != nullptr) {
            ans = max(ans, first->val + second->val);
            first = first->next;
            second = second->next;
        }

        return ans;
    }

private:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* cur = head;

        while (cur != nullptr) {
            ListNode* nextNode = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nextNode;
        }

        return prev;
    }
};

ListNode* buildList(const vector<int>& nums) {
    ListNode dummy(0);
    ListNode* tail = &dummy;

    for (int x : nums) {
        tail->next = new ListNode(x);
        tail = tail->next;
    }

    return dummy.next;
}

void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* nextNode = head->next;
        delete head;
        head = nextNode;
    }
}

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

void runTest(const vector<int>& nums, int expected) {
    ListNode* head = buildList(nums);
    Solution sol;
    int actual = sol.pairSum(head);

    cout << "head = ";
    printVector(nums);
    cout << ", expected = " << expected << ", actual = " << actual;
    cout << (actual == expected ? "  OK" : "  FAIL") << endl;

    // pairSum 会反转后半段，链表结构已被修改。
    // 这里的测试程序很短，操作系统会在进程结束时回收剩余内存。
}

int main() {
    runTest({5, 4, 2, 1}, 6);
    runTest({4, 2, 2, 3}, 7);
    runTest({1, 100000}, 100001);
    runTest({1, 2, 3, 4, 5, 6}, 7);

    return 0;
}
