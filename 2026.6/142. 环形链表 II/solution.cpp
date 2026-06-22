#include <iostream>
#include <vector>
using namespace std;

/*
 * 142. 环形链表 II
 *
 * 目标：
 *   如果链表存在环，返回环的入口节点。
 *   如果链表不存在环，返回 nullptr。
 *
 * 限制：
 *   不能修改链表。
 *   希望使用 O(1) 额外空间。
 *
 * 主解法：Floyd 快慢指针，也叫“龟兔赛跑算法”。
 *
 * 算法分为两个阶段。
 *
 * 第一阶段：判断是否存在环
 *
 *   slow 每次走 1 步。
 *   fast 每次走 2 步。
 *
 *   如果没有环，fast 最终会走到 nullptr。
 *   如果存在环，fast 会在环中追上 slow，二者一定会相遇。
 *
 * 第二阶段：寻找环的入口
 *
 *   当 slow 和 fast 在环中第一次相遇后：
 *
 *   1. 新建 finder，让 finder 从 head 开始。
 *   2. slow 保持在相遇点。
 *   3. finder 和 slow 每次都走 1 步。
 *   4. 它们下一次相遇的位置就是环的入口。
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */

struct ListNode {
    int val;
    ListNode* next;

    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    ListNode* detectCycle(ListNode* head) {
        /*
         * slow 是慢指针，每一轮向后走 1 步。
         * fast 是快指针，每一轮向后走 2 步。
         *
         * 两个指针都从头节点 head 出发。
         */
        ListNode* slow = head;
        ListNode* fast = head;

        /*
         * 第一阶段：判断链表是否有环，并寻找快慢指针的相遇点。
         *
         * fast 每轮需要访问：
         *
         *   fast->next->next
         *
         * 所以循环前必须同时保证：
         *
         *   fast != nullptr
         *   fast->next != nullptr
         *
         * 否则会访问空指针。
         */
        while (fast != nullptr && fast->next != nullptr) {
            /*
             * slow 每次走 1 步。
             */
            slow = slow->next;

            /*
             * fast 每次走 2 步。
             */
            fast = fast->next->next;

            /*
             * 如果 slow == fast，说明两个指针指向同一个节点。
             *
             * 因为 fast 比 slow 走得快，并且二者能够再次相遇，
             * 所以链表中一定存在环。
             */
            if (slow == fast) {
                /*
                 * 第二阶段：寻找环的入口。
                 *
                 * finder 从链表头节点 head 出发。
                 * slow 留在第一阶段的相遇点。
                 */
                ListNode* finder = head;

                /*
                 * finder 和 slow 现在都改为每次走 1 步。
                 *
                 * 根据 Floyd 算法的数学关系，它们会在环入口相遇。
                 */
                while (finder != slow) {
                    finder = finder->next;
                    slow = slow->next;
                }

                /*
                 * 此时：
                 *
                 *   finder == slow
                 *
                 * 并且二者都指向环的入口节点。
                 */
                return finder;
            }
        }

        /*
         * 如果第一阶段循环结束时仍然没有相遇，说明：
         *
         *   fast == nullptr
         * 或者
         *   fast->next == nullptr
         *
         * 即快指针走到了链表末尾，链表不存在环。
         */
        return nullptr;
    }
};

/*
 * 以下是本地测试辅助代码。
 *
 * LeetCode 提交时，只需要提交上面的 Solution 类。
 */

/*
 * 根据数组 values 创建普通单链表。
 *
 * 如果 pos >= 0，则让最后一个节点的 next 指向下标 pos 对应的节点，
 * 从而创建一个环。
 *
 * pos == -1 表示链表无环。
 */
ListNode* buildListWithCycle(const vector<int>& values, int pos) {
    if (values.empty()) {
        return nullptr;
    }

    vector<ListNode*> nodes;

    for (int value : values) {
        nodes.push_back(new ListNode(value));
    }

    for (int i = 0; i + 1 < static_cast<int>(nodes.size()); i++) {
        nodes[i]->next = nodes[i + 1];
    }

    if (pos >= 0 && pos < static_cast<int>(nodes.size())) {
        nodes.back()->next = nodes[pos];
    }

    return nodes[0];
}

/*
 * 测试结束后释放链表。
 *
 * 如果链表有环，直接遍历释放会无限循环，所以先根据 pos 找到最后节点，
 * 将它的 next 改为 nullptr，断开测试代码创建的环，然后再正常释放。
 */
void freeList(ListNode* head, int nodeCount, int pos) {
    if (head == nullptr) {
        return;
    }

    if (pos >= 0) {
        ListNode* tail = head;

        for (int i = 1; i < nodeCount; i++) {
            tail = tail->next;
        }

        tail->next = nullptr;
    }

    while (head != nullptr) {
        ListNode* nextNode = head->next;
        delete head;
        head = nextNode;
    }
}

void printVector(const vector<int>& values) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        cout << values[i];

        if (i + 1 < static_cast<int>(values.size())) {
            cout << ",";
        }
    }

    cout << "]";
}

void runTest(const vector<int>& values, int pos, int expectedPos) {
    ListNode* head = buildListWithCycle(values, pos);

    Solution solution;
    ListNode* actualEntry = solution.detectCycle(head);

    /*
     * 为了便于测试，把返回的节点地址转换成数组下标。
     */
    int actualPos = -1;
    ListNode* current = head;

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        if (current == actualEntry) {
            actualPos = i;
            break;
        }

        current = current->next;
    }

    cout << "values = ";
    printVector(values);
    cout << ", pos = " << pos;
    cout << ", expectedPos = " << expectedPos;
    cout << ", actualPos = " << actualPos;
    cout << (actualPos == expectedPos ? "  OK" : "  FAIL") << '\n';

    freeList(head, static_cast<int>(values.size()), pos);
}

int main() {
    // 官方示例 1：尾节点连接到下标 1，环入口值为 2。
    runTest({3, 2, 0, -4}, 1, 1);

    // 官方示例 2：两个节点成环，入口是头节点。
    runTest({1, 2}, 0, 0);

    // 官方示例 3：单节点无环。
    runTest({1}, -1, -1);

    // 单节点自环，入口就是该节点。
    runTest({7}, 0, 0);

    // 普通无环链表。
    runTest({1, 2, 3, 4, 5}, -1, -1);

    // 环入口在链表中间。
    runTest({1, 2, 3, 4, 5, 6}, 2, 2);

    // 环入口就是头节点，整个链表形成一个环。
    runTest({1, 2, 3, 4}, 0, 0);

    // 空链表。
    runTest({}, -1, -1);

    return 0;
}
