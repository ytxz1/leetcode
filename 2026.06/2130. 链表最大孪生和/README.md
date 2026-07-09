# 2130. 链表最大孪生和

## 题目描述

给你一个长度为偶数的链表 `head`，链表中共有 `n` 个节点。

对于第 `i` 个节点，和它对应的孪生节点是第 `n - 1 - i` 个节点，注意下标从 `0` 开始。

两个孪生节点的值之和叫做孪生和。要求返回所有孪生和中的最大值。

## 示例

### 示例 1

```text
输入：head = [5,4,2,1]
输出：6
```

孪生节点配对如下：

```text
第 0 个节点 5 和第 3 个节点 1 配对，孪生和 = 5 + 1 = 6
第 1 个节点 4 和第 2 个节点 2 配对，孪生和 = 4 + 2 = 6

最大孪生和 = max(6, 6) = 6
```

### 示例 2

```text
输入：head = [4,2,2,3]
输出：7
```

孪生节点配对如下：

```text
第 0 个节点 4 和第 3 个节点 3 配对，孪生和 = 4 + 3 = 7
第 1 个节点 2 和第 2 个节点 2 配对，孪生和 = 2 + 2 = 4

最大孪生和 = max(7, 4) = 7
```

### 示例 3

```text
输入：head = [1,100000]
输出：100001
```

只有两个节点，所以它们互为孪生节点：

```text
孪生和 = 1 + 100000 = 100001
```

## 核心观察

链表的孪生节点是首尾对应的：

```text
原链表：
下标： 0   1   2   3   4   5
值：   1 ->2 ->3 ->4 ->5 ->6

孪生配对：
0 和 5 配对：1 + 6
1 和 4 配对：2 + 5
2 和 3 配对：3 + 4
```

如果这是数组，可以直接用两个下标 `left` 和 `right`：

```cpp
left = 0, right = n - 1
nums[left] + nums[right]
```

但是链表不能从尾部往前走，所以不能直接拿到“尾节点、倒数第二个节点、倒数第三个节点”。

解决办法是：

1. 找到链表的后半段。
2. 把后半段反转。
3. 这样后半段就从“尾节点”开始向前排列。
4. 前半段和反转后的后半段同时向后走，就能一一得到孪生节点。

## 解题思路

以 `head = [5,4,2,1]` 为例。

原链表：

```text
5 -> 4 -> 2 -> 1 -> null
```

目标配对是：

```text
5 和 1
4 和 2
```

如果把后半段 `[2,1]` 反转成 `[1,2]`：

```text
前半段：        5 -> 4
反转后的后半段：1 -> 2
```

然后两个指针一起走：

```text
第一步：5 + 1 = 6
第二步：4 + 2 = 6
答案：6
```

## 代码实现

核心代码在 [solution.cpp](D:\leetcode\2026.6\2130. 链表最大孪生和\solution.cpp)。

LeetCode 提交时只需要提交 `ListNode` 已经给出的情况下的 `Solution` 类：

```cpp
class Solution {
public:
    int pairSum(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode* secondHalf = reverseList(slow);

        int ans = 0;
        ListNode* first = head;
        ListNode* second = secondHalf;

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
```

## 逐步详解

### 第一步：用快慢指针找到后半段起点

代码：

```cpp
ListNode* slow = head;
ListNode* fast = head;

while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;
}
```

含义：

- `slow` 每次走 1 步。
- `fast` 每次走 2 步。
- 链表长度是偶数，所以当 `fast` 走到 `nullptr` 时，`slow` 正好来到后半段的第一个节点。

用 `head = [5,4,2,1]` 演示：

```text
初始：
slow 指向 5
fast 指向 5

链表：5 -> 4 -> 2 -> 1 -> null
      S
      F
```

第 1 轮循环：

```cpp
slow = slow->next;       // slow 从 5 走到 4
fast = fast->next->next; // fast 从 5 走到 2
```

状态变成：

```text
链表：5 -> 4 -> 2 -> 1 -> null
           S    F
```

第 2 轮循环：

```cpp
slow = slow->next;       // slow 从 4 走到 2
fast = fast->next->next; // fast 从 2 走两步，到 null
```

状态变成：

```text
链表：5 -> 4 -> 2 -> 1 -> null
                S         F
```

循环结束，此时：

```text
slow 指向节点 2
后半段是：2 -> 1 -> null
```

这正好是原链表的后半段。

### 第二步：反转后半段

代码：

```cpp
ListNode* secondHalf = reverseList(slow);
```

`slow` 指向后半段的起点，所以这行代码表示：

```text
把 2 -> 1 -> null
反转成 1 -> 2 -> null
```

反转函数：

```cpp
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
```

变量含义：

- `prev`：已经反转好的链表头部。
- `cur`：当前正在处理的节点。
- `nextNode`：提前保存 `cur` 的下一个节点，避免断链后找不到后续节点。

以 `2 -> 1 -> null` 为例。

初始状态：

```text
prev = null
cur  = 2

原后半段：2 -> 1 -> null
```

第 1 轮：

```cpp
ListNode* nextNode = cur->next; // nextNode = 1
cur->next = prev;               // 2->next 改成 null
prev = cur;                     // prev 指向 2
cur = nextNode;                 // cur 指向 1
```

状态：

```text
已反转部分：2 -> null
未处理部分：1 -> null

prev = 2
cur  = 1
```

第 2 轮：

```cpp
ListNode* nextNode = cur->next; // nextNode = null
cur->next = prev;               // 1->next 改成 2
prev = cur;                     // prev 指向 1
cur = nextNode;                 // cur 指向 null
```

状态：

```text
已反转部分：1 -> 2 -> null
未处理部分：空

prev = 1
cur  = null
```

循环结束，返回 `prev`，也就是反转后链表的头节点 `1`。

所以：

```cpp
ListNode* secondHalf = reverseList(slow);
```

执行完成后：

```text
secondHalf 指向 1
反转后的后半段：1 -> 2 -> null
```

### 第三步：前半段和后半段同时走，计算最大孪生和

代码：

```cpp
int ans = 0;
ListNode* first = head;
ListNode* second = secondHalf;

while (second != nullptr) {
    ans = max(ans, first->val + second->val);
    first = first->next;
    second = second->next;
}
```

变量含义：

- `first` 从原链表头节点开始走。
- `second` 从反转后的后半段头节点开始走。
- `second` 的长度是 `n / 2`，所以循环 `n / 2` 次，刚好计算所有孪生和。

以 `[5,4,2,1]` 为例。

此时可以理解成两条链：

```text
first:  5 -> 4 -> ...
second: 1 -> 2 -> null
```

初始：

```text
ans = 0
first 指向 5
second 指向 1
```

第 1 轮：

```cpp
ans = max(ans, first->val + second->val);
```

对应：

```text
first->val = 5
second->val = 1
当前孪生和 = 5 + 1 = 6
ans = max(0, 6) = 6
```

然后移动指针：

```cpp
first = first->next;   // first 从 5 走到 4
second = second->next; // second 从 1 走到 2
```

第 2 轮：

```text
first->val = 4
second->val = 2
当前孪生和 = 4 + 2 = 6
ans = max(6, 6) = 6
```

然后移动指针：

```text
second 走到 null，循环结束
```

最终返回：

```text
ans = 6
```

## 更长例子

用 `head = [1,2,3,4,5,6]` 演示完整过程。

原链表：

```text
1 -> 2 -> 3 -> 4 -> 5 -> 6 -> null
```

孪生配对应该是：

```text
1 和 6，和为 7
2 和 5，和为 7
3 和 4，和为 7
```

### 找中点

快慢指针过程：

```text
初始：slow = 1, fast = 1
第 1 轮后：slow = 2, fast = 3
第 2 轮后：slow = 3, fast = 5
第 3 轮后：slow = 4, fast = null
```

因此后半段起点是 `4`：

```text
后半段：4 -> 5 -> 6 -> null
```

### 反转后半段

反转：

```text
4 -> 5 -> 6 -> null
```

变成：

```text
6 -> 5 -> 4 -> null
```

### 同步计算

现在：

```text
first:  1 -> 2 -> 3 -> ...
second: 6 -> 5 -> 4 -> null
```

逐步计算：

```text
第 1 轮：
first = 1, second = 6
sum = 1 + 6 = 7
ans = max(0, 7) = 7

第 2 轮：
first = 2, second = 5
sum = 2 + 5 = 7
ans = max(7, 7) = 7

第 3 轮：
first = 3, second = 4
sum = 3 + 4 = 7
ans = max(7, 7) = 7
```

答案是 `7`。

## 为什么这样配对是正确的

找到中点后，链表被分成前后两段：

```text
前半段：a0 -> a1 -> a2 -> ... -> a(n/2 - 1)
后半段：a(n/2) -> ... -> a(n-3) -> a(n-2) -> a(n-1)
```

反转后半段后：

```text
反转后的后半段：a(n-1) -> a(n-2) -> a(n-3) -> ... -> a(n/2)
```

然后前半段和反转后的后半段同步走：

```text
第 1 步：a0        + a(n-1)
第 2 步：a1        + a(n-2)
第 3 步：a2        + a(n-3)
...
```

这正好就是题目定义的孪生节点配对。

## 复杂度分析

设链表长度为 `n`。

| 步骤 | 时间复杂度 | 说明 |
|---|---:|---|
| 快慢指针找后半段起点 | O(n) | 实际走了约 `n / 2` 轮 |
| 反转后半段 | O(n) | 反转 `n / 2` 个节点 |
| 同步计算最大孪生和 | O(n) | 计算 `n / 2` 组配对 |

总时间复杂度：

```text
O(n)
```

额外空间复杂度：

```text
O(1)
```

因为整个过程只用了几个指针变量，没有额外数组或哈希表。

## 易错点

1. `slow` 停下的位置是后半段的第一个节点，不是前半段最后一个节点。
2. 反转链表时必须先保存 `nextNode = cur->next`，否则修改 `cur->next` 后会丢失后续节点。
3. 计算孪生和时循环条件应该用 `while (second != nullptr)`，因为反转后的后半段长度正好是 `n / 2`。
4. 题目保证链表长度是偶数，所以不需要单独处理奇数长度的中间节点。
5. 如果在真实工程里需要保留原链表结构，可以在计算完答案后再把后半段反转回来。LeetCode 本题通常不需要恢复。

## 可选方法：用数组

也可以先把链表所有值存进数组，然后用双指针计算：

```cpp
vector<int> nums;
while (head != nullptr) {
    nums.push_back(head->val);
    head = head->next;
}

int left = 0;
int right = nums.size() - 1;
int ans = 0;

while (left < right) {
    ans = max(ans, nums[left] + nums[right]);
    left++;
    right--;
}
```

这个方法更直观，但是空间复杂度是 `O(n)`。本题可以做到 `O(1)` 额外空间，所以更推荐快慢指针加反转链表。

## 总结

本题关键是把“链表首尾配对”的问题，转换成“两条链表从前往后同步配对”的问题。

具体做法：

1. 快慢指针找到后半段起点。
2. 反转后半段，让尾部节点变成后半段的头。
3. 前半段和反转后的后半段同步遍历，更新最大孪生和。

这套做法是链表题中非常常见的组合技巧，也适用于很多“后半段需要倒序比较”的题目。
