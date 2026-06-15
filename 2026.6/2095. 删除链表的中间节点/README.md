# 2095. 删除链表的中间节点

题目链接：[LeetCode 2095. 删除链表的中间节点](https://leetcode.cn/problems/delete-the-middle-node-of-a-linked-list/)

## 题目描述

给你一个链表的头节点 `head`。

删除链表的中间节点，并返回修改后的链表头节点。

如果链表有 `n` 个节点，下标从 `0` 开始，那么中间节点的下标是：

```text
floor(n / 2)
```

也就是说：

- `n = 1` 时，中间节点下标是 `0`
- `n = 2` 时，中间节点下标是 `1`
- `n = 4` 时，中间节点下标是 `2`
- `n = 7` 时，中间节点下标是 `3`

注意：这是删除“偏右”的中间节点。比如长度为 `4` 的链表，中间节点是下标 `2`，不是下标 `1`。

## 核心思路

链表不能像数组一样通过下标直接访问第 `floor(n / 2)` 个节点，所以需要一边遍历一边定位中间节点。

本题最常用的方法是快慢指针：

- `slow`：慢指针，每轮走 `1` 步
- `fast`：快指针，每轮走 `2` 步
- `prev`：始终记录 `slow` 的前一个节点，方便删除 `slow`

当 `fast` 走到链表末尾时，`slow` 正好停在中间节点。

然后执行：

```cpp
prev->next = slow->next;
```

这行代码的意思是：让 `slow` 前面的节点跳过 `slow`，直接连接到 `slow` 后面的节点。这样 `slow` 就从链表中被删除了。

## 为什么需要 `prev`

假设链表是：

```text
1 -> 3 -> 4 -> 7 -> 1 -> 2 -> 6 -> null
```

要删除的中间节点是 `7`。

如果只有 `slow` 指向 `7`：

```text
1 -> 3 -> 4 -> 7 -> 1 -> 2 -> 6 -> null
              S
```

我们知道要删 `7`，但是单链表没有从当前节点回到前一个节点的能力。

真正能完成删除的是 `7` 前面的节点 `4`：

```text
4->next = 7->next
```

也就是：

```text
4->next = 1
```

所以遍历时必须额外维护一个 `prev`，让它始终指向 `slow` 的前一个节点。

## 为什么使用虚拟头节点 `dummy`

代码中用了：

```cpp
ListNode dummy(0, head);
ListNode* prev = &dummy;
```

`dummy` 是一个虚拟头节点，它放在真正的 `head` 前面：

```text
dummy -> head -> ...
```

这样做的好处是可以统一处理只有一个节点的情况。

比如：

```text
head = [1]
```

此时要删除的节点就是 `head` 本身。因为 `prev` 初始指向 `dummy`，`slow` 初始指向 `head`，删除时执行：

```cpp
prev->next = slow->next;
```

对应到这个例子就是：

```text
dummy->next = null
```

最后返回：

```cpp
return dummy.next;
```

结果就是空链表，正好符合题意。

## 代码实现

核心代码见 [solution.cpp](D:\leetcode\2026.6\2095. 删除链表的中间节点\solution.cpp)。

LeetCode 提交时只需要提交 `Solution` 类：

```cpp
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
```

## 变量含义逐行解释

```cpp
ListNode dummy(0, head);
```

创建虚拟头节点。它的值是 `0`，值本身不重要；它的 `next` 指向原来的头节点 `head`。

```cpp
ListNode* prev = &dummy;
```

`prev` 指向 `slow` 的前一个节点。初始时 `slow` 指向 `head`，所以 `prev` 指向 `head` 前面的虚拟头节点 `dummy`。

```cpp
ListNode* slow = head;
```

`slow` 是慢指针，最终会停在要删除的中间节点。

```cpp
ListNode* fast = head;
```

`fast` 是快指针，每次比 `slow` 多走一步，用来帮助判断 `slow` 什么时候到达中间节点。

```cpp
while (fast != nullptr && fast->next != nullptr)
```

只要 `fast` 当前不为空，并且 `fast` 还能继续往后走一步，就继续循环。

循环体：

```cpp
prev = slow;
slow = slow->next;
fast = fast->next->next;
```

每一轮中：

- `prev = slow`：先让 `prev` 记录 `slow` 当前的位置
- `slow = slow->next`：`slow` 向后走 `1` 步
- `fast = fast->next->next`：`fast` 向后走 `2` 步

因此循环结束后，`slow` 指向中间节点，`prev` 指向中间节点的前一个节点。

删除操作：

```cpp
prev->next = slow->next;
```

让 `prev` 直接连接到 `slow` 后面的节点，从而跳过 `slow`。

返回答案：

```cpp
return dummy.next;
```

因为头节点有可能被删除，所以返回 `dummy.next` 比直接返回 `head` 更稳。

## 示例 1：奇数长度链表

```text
输入：head = [1,3,4,7,1,2,6]
输出：[1,3,4,1,2,6]
```

链表长度是 `7`，中间节点下标是：

```text
floor(7 / 2) = 3
```

下标和值对应如下：

```text
下标：0   1   2   3   4   5   6
值：  1 ->3 ->4 ->7 ->1 ->2 ->6 -> null
```

要删除下标 `3` 的节点，也就是值为 `7` 的节点。

### 初始化

代码变量：

```cpp
ListNode dummy(0, head);
ListNode* prev = &dummy;
ListNode* slow = head;
ListNode* fast = head;
```

对应链表：

```text
dummy -> 1 -> 3 -> 4 -> 7 -> 1 -> 2 -> 6 -> null
  P      S
         F
```

此时：

```text
prev 指向 dummy
slow 指向节点 1
fast 指向节点 1
```

### 第 1 轮循环

循环前：

```text
prev = dummy
slow = 1
fast = 1
```

执行：

```cpp
prev = slow;             // prev 从 dummy 移到 1
slow = slow->next;       // slow 从 1 移到 3
fast = fast->next->next; // fast 从 1 移到 4
```

循环后：

```text
dummy -> 1 -> 3 -> 4 -> 7 -> 1 -> 2 -> 6 -> null
         P    S    F
```

变量值：

```text
prev = 1
slow = 3
fast = 4
```

### 第 2 轮循环

循环前：

```text
prev = 1
slow = 3
fast = 4
```

执行：

```cpp
prev = slow;             // prev 从 1 移到 3
slow = slow->next;       // slow 从 3 移到 4
fast = fast->next->next; // fast 从 4 移到 1
```

循环后：

```text
dummy -> 1 -> 3 -> 4 -> 7 -> 1 -> 2 -> 6 -> null
              P    S         F
```

变量值：

```text
prev = 3
slow = 4
fast = 1
```

### 第 3 轮循环

循环前：

```text
prev = 3
slow = 4
fast = 1
```

执行：

```cpp
prev = slow;             // prev 从 3 移到 4
slow = slow->next;       // slow 从 4 移到 7
fast = fast->next->next; // fast 从 1 移到 6
```

循环后：

```text
dummy -> 1 -> 3 -> 4 -> 7 -> 1 -> 2 -> 6 -> null
                   P    S              F
```

变量值：

```text
prev = 4
slow = 7
fast = 6
```

### 循环结束判断

此时 `fast` 指向节点 `6`，但是：

```text
fast->next = null
```

所以循环条件：

```cpp
fast != nullptr && fast->next != nullptr
```

不成立，循环结束。

最终定位结果：

```text
prev = 4
slow = 7
fast = 6
```

`slow` 指向的 `7` 正是要删除的中间节点。

### 删除中间节点

执行：

```cpp
prev->next = slow->next;
```

对应本例：

```text
prev 是节点 4
slow 是节点 7
slow->next 是节点 1
```

所以：

```text
4->next = 1
```

删除前：

```text
1 -> 3 -> 4 -> 7 -> 1 -> 2 -> 6 -> null
```

删除后：

```text
1 -> 3 -> 4 -> 1 -> 2 -> 6 -> null
```

返回：

```text
[1,3,4,1,2,6]
```

## 示例 2：偶数长度链表

```text
输入：head = [1,2,3,4]
输出：[1,2,4]
```

链表长度是 `4`，中间节点下标是：

```text
floor(4 / 2) = 2
```

下标和值：

```text
下标：0   1   2   3
值：  1 ->2 ->3 ->4 -> null
```

所以要删除值为 `3` 的节点。

### 初始化

```text
dummy -> 1 -> 2 -> 3 -> 4 -> null
  P      S
         F
```

变量值：

```text
prev = dummy
slow = 1
fast = 1
```

### 第 1 轮循环

执行：

```cpp
prev = slow;             // prev 从 dummy 移到 1
slow = slow->next;       // slow 从 1 移到 2
fast = fast->next->next; // fast 从 1 移到 3
```

循环后：

```text
dummy -> 1 -> 2 -> 3 -> 4 -> null
         P    S    F
```

变量值：

```text
prev = 1
slow = 2
fast = 3
```

### 第 2 轮循环

执行：

```cpp
prev = slow;             // prev 从 1 移到 2
slow = slow->next;       // slow 从 2 移到 3
fast = fast->next->next; // fast 从 3 走两步，移到 null
```

循环后：

```text
dummy -> 1 -> 2 -> 3 -> 4 -> null
              P    S         F
```

变量值：

```text
prev = 2
slow = 3
fast = null
```

### 循环结束

因为 `fast == nullptr`，循环结束。

`slow` 指向值为 `3` 的节点，正好是下标 `2` 的中间节点。

### 删除

执行：

```cpp
prev->next = slow->next;
```

对应：

```text
2->next = 4
```

删除前：

```text
1 -> 2 -> 3 -> 4 -> null
```

删除后：

```text
1 -> 2 -> 4 -> null
```

返回：

```text
[1,2,4]
```

## 示例 3：两个节点

```text
输入：head = [2,1]
输出：[2]
```

链表长度是 `2`，中间节点下标是：

```text
floor(2 / 2) = 1
```

所以要删除第 `1` 个节点，也就是值为 `1` 的节点。

### 初始化

```text
dummy -> 2 -> 1 -> null
  P      S
         F
```

变量值：

```text
prev = dummy
slow = 2
fast = 2
```

### 第 1 轮循环

执行：

```cpp
prev = slow;             // prev 从 dummy 移到 2
slow = slow->next;       // slow 从 2 移到 1
fast = fast->next->next; // fast 从 2 走两步，移到 null
```

循环后：

```text
dummy -> 2 -> 1 -> null
         P    S    F
```

变量值：

```text
prev = 2
slow = 1
fast = null
```

### 删除

执行：

```cpp
prev->next = slow->next;
```

对应：

```text
2->next = null
```

删除前：

```text
2 -> 1 -> null
```

删除后：

```text
2 -> null
```

返回：

```text
[2]
```

## 示例 4：只有一个节点

```text
输入：head = [1]
输出：[]
```

链表长度是 `1`，中间节点下标是：

```text
floor(1 / 2) = 0
```

所以要删除唯一的节点。

### 初始化

```text
dummy -> 1 -> null
  P      S
         F
```

变量值：

```text
prev = dummy
slow = 1
fast = 1
```

### 循环判断

循环条件是：

```cpp
fast != nullptr && fast->next != nullptr
```

此时：

```text
fast = 1
fast->next = null
```

所以条件不成立，循环一次也不会执行。

变量仍然是：

```text
prev = dummy
slow = 1
fast = 1
```

### 删除

执行：

```cpp
prev->next = slow->next;
```

对应：

```text
dummy->next = null
```

返回：

```cpp
return dummy.next;
```

此时 `dummy.next` 是 `null`，所以返回空链表：

```text
[]
```

## 示例 5：五个节点

```text
输入：head = [10,20,30,40,50]
输出：[10,20,40,50]
```

长度是 `5`，中间节点下标是：

```text
floor(5 / 2) = 2
```

也就是要删除值为 `30` 的节点。

### 快慢指针过程表

| 轮次 | 操作前 prev | 操作前 slow | 操作前 fast | 操作后 prev | 操作后 slow | 操作后 fast |
|---:|---:|---:|---:|---:|---:|---:|
| 初始化 | dummy | 10 | 10 | dummy | 10 | 10 |
| 第 1 轮 | dummy | 10 | 10 | 10 | 20 | 30 |
| 第 2 轮 | 10 | 20 | 30 | 20 | 30 | 50 |

第 `2` 轮结束后，`fast` 指向 `50`，而 `fast->next` 是 `null`，循环结束。

最终：

```text
prev = 20
slow = 30
fast = 50
```

执行删除：

```cpp
prev->next = slow->next;
```

对应：

```text
20->next = 40
```

删除后：

```text
10 -> 20 -> 40 -> 50 -> null
```

返回：

```text
[10,20,40,50]
```

## 正确性说明

设链表长度为 `n`，中间节点下标为 `floor(n / 2)`。

每一轮循环中：

- `slow` 走 `1` 步
- `fast` 走 `2` 步
- `prev` 变成移动前的 `slow`

循环执行 `k` 轮后：

```text
slow 位于下标 k
fast 位于下标 2k，或者已经走到 null
prev 位于下标 k - 1
```

当循环结束时，有两种情况。

第一种：`n` 是奇数。比如 `n = 7`。

```text
fast 最后停在最后一个节点
slow 停在下标 3
floor(7 / 2) = 3
```

第二种：`n` 是偶数。比如 `n = 4`。

```text
fast 最后变成 null
slow 停在下标 2
floor(4 / 2) = 2
```

所以无论链表长度是奇数还是偶数，循环结束后 `slow` 都指向题目要求删除的中间节点。

又因为 `prev` 始终记录 `slow` 的前一个节点，所以：

```cpp
prev->next = slow->next;
```

可以正确删除 `slow` 指向的节点。

最后返回 `dummy.next`，可以覆盖头节点被删除的情况，因此算法正确。

## 复杂度分析

设链表长度为 `n`。

时间复杂度：

```text
O(n)
```

原因是 `fast` 每次走两步，最多遍历整条链表一次。

空间复杂度：

```text
O(1)
```

原因是只使用了 `dummy`、`prev`、`slow`、`fast` 这几个额外变量，没有使用数组、栈、哈希表等额外数据结构。

## 常见错误

### 错误 1：只用 `slow`，没有保存前驱节点

如果只找到中间节点：

```cpp
ListNode* slow = head;
```

最后 `slow` 指向了要删除的节点，但你无法从 `slow` 回到它前面的节点。

单链表删除某个节点，通常需要知道它的前一个节点。

### 错误 2：偶数长度时删错节点

对于：

```text
[1,2,3,4]
```

题目要求删除下标：

```text
floor(4 / 2) = 2
```

也就是删除 `3`，不是删除 `2`。

本题的快慢指针写法：

```cpp
while (fast != nullptr && fast->next != nullptr)
```

会让 `slow` 停在下标 `2`，符合题意。

### 错误 3：没有处理单节点

如果不用 `dummy`，很多人会单独写：

```cpp
if (head->next == nullptr) {
    return nullptr;
}
```

这当然可以，但用了 `dummy` 后，单节点也能被统一处理，不需要额外分支。

### 错误 4：返回了旧的 `head`

当链表只有一个节点时，旧的 `head` 被删除了。如果仍然返回 `head`，答案就是错的。

所以本解法返回：

```cpp
return dummy.next;
```

这样即使头节点被删，也能返回正确的新头节点。

## 总结

这道题的关键是：

1. 用 `slow` 和 `fast` 找到中间节点。
2. 用 `prev` 记录中间节点的前一个节点。
3. 用 `prev->next = slow->next` 删除中间节点。
4. 用 `dummy` 统一处理单节点和普通情况。

整套写法只遍历一次链表，时间复杂度 `O(n)`，额外空间复杂度 `O(1)`。
