#include <iostream>
#include <vector>
using namespace std;

/*
 * 2095. 删除链表的中间节点
 *
 * 思路：
 *   使用快慢指针找到中间节点。
 *   slow 每次走 1 步，fast 每次走 2 步。
 *   当 fast 走到链表末尾时，slow 正好指向要删除的中间节点。
 *   同时用 prev 记录 slow 的前一个节点，方便执行删除操作。
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
    ListNode* deleteMiddle(ListNode* head) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy;
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast != nullptr && fast->next != nullptr) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }

        prev->next = slow->next;
        return dummy.next;
    }
};

ListNode* buildList(const vector<int>& nums) {
    ListNode dummy;
    ListNode* tail = &dummy;

    for (int x : nums) {
        tail->next = new ListNode(x);
        tail = tail->next;
    }

    return dummy.next;
}

vector<int> toVector(ListNode* head) {
    vector<int> result;

    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }

    return result;
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

void runTest(const vector<int>& nums, const vector<int>& expected) {
    ListNode* head = buildList(nums);
    Solution sol;
    ListNode* newHead = sol.deleteMiddle(head);
    vector<int> actual = toVector(newHead);

    cout << "head = ";
    printVector(nums);
    cout << ", expected = ";
    printVector(expected);
    cout << ", actual = ";
    printVector(actual);
    cout << (actual == expected ? "  OK" : "  FAIL") << endl;

    freeList(newHead);
}

int main() {
    runTest({1, 3, 4, 7, 1, 2, 6}, {1, 3, 4, 1, 2, 6});
    runTest({1, 2, 3, 4}, {1, 2, 4});
    runTest({2, 1}, {2});
    runTest({1}, {});
    runTest({10, 20, 30, 40, 50}, {10, 20, 40, 50});

    return 0;
}
