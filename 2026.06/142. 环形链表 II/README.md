# 142. 环形链表 II

题目链接：[LeetCode 142. 环形链表 II](https://leetcode.cn/problems/linked-list-cycle-ii/)

## 题目描述

给定一个链表的头节点 `head`，判断链表中是否存在环。

如果存在环，返回环开始的第一个节点，也就是环的入口节点。

如果不存在环，返回：

```cpp
nullptr
```

题目中的 `pos` 只用于描述尾节点连接到哪个下标，它不会作为参数传给函数。

例如：

```text
head = [3,2,0,-4]
pos = 1
```

表示尾节点 `-4` 的 `next` 指向下标 `1` 的节点 `2`：

```text
3 -> 2 -> 0 -> -4
     ^           |
     |___________|
```

环的入口是节点 `2`。

## 题目要求

函数签名：

```cpp
ListNode* detectCycle(ListNode* head)
```

需要返回的是：

```text
环入口节点的指针
```

不是入口节点的值，也不是入口下标。

如果无环：

```cpp
return nullptr;
```

## 核心解法：Floyd 快慢指针

Floyd 算法也叫：

```text
龟兔赛跑算法
```

它使用两个指针：

```cpp
ListNode* slow = head;
ListNode* fast = head;
```

移动速度：

```text
slow 每次走 1 步
fast 每次走 2 步
```

算法分为两个阶段：

```text
第一阶段：判断是否有环，并找到环中的第一次相遇点
第二阶段：从第一次相遇点推导并找到环入口
```

## 第一阶段：判断是否有环

代码：

```cpp
while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;

    if (slow == fast) {
        // 链表有环
    }
}
```

### 无环时会发生什么

如果链表没有环：

```text
1 -> 2 -> 3 -> 4 -> 5 -> null
```

`fast` 每次走两步，最终一定会遇到：

```text
fast == nullptr
```

或者：

```text
fast->next == nullptr
```

于是循环结束，返回 `nullptr`。

### 有环时为什么一定会相遇

假设两个指针都已经进入环。

每一轮：

```text
slow 前进 1 步
fast 前进 2 步
```

所以 `fast` 相对于 `slow` 每轮靠近 `1` 步。

环中的节点数量是有限的，因此 `fast` 不可能永远追不上 `slow`。

可以把环想象成圆形跑道：

```text
slow：每轮跑 1 格
fast：每轮跑 2 格
```

快指针会从后面追上慢指针。

当：

```cpp
slow == fast
```

说明链表一定有环。

## 第二阶段：寻找环入口

快慢指针第一次相遇后：

```cpp
ListNode* finder = head;
```

让：

```text
finder 从头节点出发
slow 留在第一次相遇点
```

然后两个指针都每次走一步：

```cpp
while (finder != slow) {
    finder = finder->next;
    slow = slow->next;
}
```

它们再次相遇的位置就是环入口。

## 变量说明

| 变量 | 含义 |
|---|---|
| `head` | 链表的头节点 |
| `slow` | 慢指针，第一阶段每轮走 1 步 |
| `fast` | 快指针，第一阶段每轮走 2 步 |
| `finder` | 第二阶段从头节点出发、寻找环入口的指针 |

第一阶段：

```text
slow：head -> 每次 1 步
fast：head -> 每次 2 步
```

第二阶段：

```text
finder：从 head 开始，每次 1 步
slow：从第一次相遇点开始，每次 1 步
```

## 数学图解

把链表分成三段：

```text
head ----a----> 环入口 ----b----> 第一次相遇点
                 ^                    |
                 |--------c-----------|
```

定义：

```text
a = 从 head 到环入口的距离
b = 从环入口到第一次相遇点的距离
c = 从第一次相遇点继续走回环入口的距离
```

环的长度：

```text
L = b + c
```

图解：

```text
非环部分：

head --(a 步)--> entry

环：

entry --(b 步)--> meeting --(c 步)--> entry
```

## 为什么相遇后从 head 出发能找到入口

第一次相遇时，慢指针走过：

```text
a + b
```

快指针速度是慢指针的两倍，所以快指针走过：

```text
2(a + b)
```

快指针比慢指针多走了若干个完整的环。

设多走了 `k` 圈：

```text
2(a + b) = a + b + kL
```

移项：

```text
a + b = kL
```

所以：

```text
a = kL - b
```

因为：

```text
L = b + c
```

所以：

```text
a = kL - b
  = (k - 1)L + (L - b)
  = (k - 1)L + c
```

最终得到最重要的关系：

```text
a = (k - 1) 个完整环 + c
```

这是什么意思？

- `finder` 从 `head` 走 `a` 步，正好到达环入口。
- `slow` 从相遇点走 `a` 步，相当于先走若干完整圈，再走 `c` 步，也正好到达环入口。

所以两个指针每次都走一步时，会在环入口相遇。

## 更直观的理解

相遇点到环入口的距离是：

```text
c
```

头节点到环入口的距离 `a`，在环中取模后也等于：

```text
c
```

因此：

```text
从 head 出发走 a 步
从 meeting 出发走 a 步
```

两个指针最终都落在入口。

## 带详细注释的代码

完整代码见：

[solution.cpp](D:\leetcode\2026.6\142. 环形链表 II\solution.cpp)

LeetCode 提交时只需要：

```cpp
class Solution {
public:
    ListNode* detectCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast) {
                ListNode* finder = head;

                while (finder != slow) {
                    finder = finder->next;
                    slow = slow->next;
                }

                return finder;
            }
        }

        return nullptr;
    }
};
```

## 示例 1：环入口在链表中间

输入：

```text
head = [3,2,0,-4]
pos = 1
```

给节点标上下标：

```text
下标： 0    1    2     3
值：   3 -> 2 -> 0 -> -4
           ^            |
           |____________|
```

尾节点下标 `3` 指向下标 `1`。

环：

```text
2 -> 0 -> -4 -> 2
```

环入口：

```text
下标 1，值为 2
```

### 第一阶段初始化

代码：

```cpp
ListNode* slow = head;
ListNode* fast = head;
```

状态：

```text
slow = 下标 0，值 3
fast = 下标 0，值 3
```

图解：

```text
3 -> 2 -> 0 -> -4
S
F
```

### 第一阶段第 1 轮

执行：

```cpp
slow = slow->next;
fast = fast->next->next;
```

`slow` 走一步：

```text
下标 0 -> 下标 1
值 3 -> 值 2
```

`fast` 走两步：

```text
下标 0 -> 下标 1 -> 下标 2
值 3 -> 值 2 -> 值 0
```

结果：

```text
slow = 下标 1，值 2
fast = 下标 2，值 0
```

图解：

```text
3 -> 2 -> 0 -> -4
     S    F
```

判断：

```text
slow != fast
```

继续循环。

### 第一阶段第 2 轮

循环前：

```text
slow = 下标 1
fast = 下标 2
```

`slow` 走一步：

```text
下标 1 -> 下标 2
```

`fast` 走两步：

```text
下标 2 -> 下标 3 -> 下标 1
```

结果：

```text
slow = 下标 2，值 0
fast = 下标 1，值 2
```

图解：

```text
3 -> 2 -> 0 -> -4
     F    S
```

仍然没有相遇。

### 第一阶段第 3 轮

循环前：

```text
slow = 下标 2
fast = 下标 1
```

`slow` 走一步：

```text
下标 2 -> 下标 3
```

`fast` 走两步：

```text
下标 1 -> 下标 2 -> 下标 3
```

结果：

```text
slow = 下标 3，值 -4
fast = 下标 3，值 -4
```

此时：

```cpp
slow == fast
```

快慢指针第一次相遇，相遇点是下标 `3`。

### 第一阶段变量总表

| 轮次 | `slow` 移动后下标 | `slow` 值 | `fast` 移动后下标 | `fast` 值 | 是否相遇 |
|---:|---:|---:|---:|---:|---|
| 初始 | 0 | 3 | 0 | 3 | 不在循环内判断 |
| 1 | 1 | 2 | 2 | 0 | 否 |
| 2 | 2 | 0 | 1 | 2 | 否 |
| 3 | 3 | -4 | 3 | -4 | 是 |

### 第二阶段初始化

代码：

```cpp
ListNode* finder = head;
```

现在：

```text
finder = 下标 0，值 3
slow = 下标 3，值 -4
```

图解：

```text
3 -> 2 -> 0 -> -4
P                   S
```

这里用 `P` 表示 `finder`。

### 第二阶段第 1 轮

执行：

```cpp
finder = finder->next;
slow = slow->next;
```

`finder`：

```text
下标 0 -> 下标 1
```

`slow`：

```text
下标 3 -> 下标 1
```

结果：

```text
finder = 下标 1，值 2
slow = 下标 1，值 2
```

二者相遇。

返回：

```cpp
return finder;
```

返回的就是环入口节点 `2`。

## 示例 1 的数学变量

对于这个例子：

```text
a = head 到入口的距离 = 1
b = 入口到相遇点的距离 = 2
c = 相遇点回入口的距离 = 1
L = b + c = 3
```

慢指针第一次相遇时走了：

```text
a + b = 1 + 2 = 3
```

快指针走了：

```text
6
```

快指针比慢指针多走一圈：

```text
6 - 3 = 3 = L
```

并且：

```text
a = 1
c = 1
```

所以：

- `finder` 从头走 1 步到入口。
- `slow` 从相遇点走 1 步也到入口。

## 示例 2：环入口就是头节点

输入：

```text
head = [1,2]
pos = 0
```

图解：

```text
1 -> 2
^    |
|____|
```

环入口是头节点 `1`。

### 第一阶段

初始：

```text
slow = 下标 0
fast = 下标 0
```

第 1 轮：

```text
slow：0 -> 1
fast：0 -> 1 -> 0
```

结果：

```text
slow = 1
fast = 0
```

第 2 轮：

```text
slow：1 -> 0
fast：0 -> 1 -> 0
```

结果：

```text
slow = 0
fast = 0
```

相遇点就是头节点。

### 第二阶段

```text
finder = head = 下标 0
slow = 相遇点 = 下标 0
```

此时一开始就满足：

```cpp
finder == slow
```

所以第二阶段循环不执行，直接返回头节点。

## 示例 3：单节点无环

输入：

```text
head = [1]
pos = -1
```

图解：

```text
1 -> null
```

初始化：

```text
slow = head
fast = head
```

循环条件：

```cpp
fast != nullptr && fast->next != nullptr
```

此时：

```text
fast != nullptr
fast->next == nullptr
```

条件不成立，循环一次也不执行。

最终：

```cpp
return nullptr;
```

## 示例 4：单节点自环

输入：

```text
head = [7]
pos = 0
```

图解：

```text
  ┌───┐
  ↓   |
 [7]──┘
```

节点 `7` 的 `next` 指向自己。

初始化：

```text
slow = 7
fast = 7
```

第 1 轮：

```text
slow = slow->next = 7
fast = fast->next->next = 7
```

所以：

```text
slow == fast
```

第二阶段：

```text
finder = head = 7
slow = 7
```

一开始就相等，返回节点 `7`。

## 示例 5：普通无环链表

输入：

```text
head = [1,2,3,4,5]
pos = -1
```

图解：

```text
1 -> 2 -> 3 -> 4 -> 5 -> null
```

变量过程：

| 轮次 | `slow` | `fast` | 说明 |
|---:|---:|---:|---|
| 初始 | 1 | 1 | 都从头开始 |
| 1 | 2 | 3 | 未相遇 |
| 2 | 3 | 5 | 未相遇 |

下一次判断循环条件时：

```text
fast 指向 5
fast->next == nullptr
```

循环结束，返回 `nullptr`。

## 示例 6：环入口在下标 2

输入：

```text
values = [1,2,3,4,5,6]
pos = 2
```

图解：

```text
1 -> 2 -> 3 -> 4 -> 5 -> 6
          ^              |
          |______________|
```

环入口是节点 `3`。

环为：

```text
3 -> 4 -> 5 -> 6 -> 3
```

### 第一阶段变量表

| 轮次 | `slow` 下标 | `slow` 值 | `fast` 下标 | `fast` 值 | 是否相遇 |
|---:|---:|---:|---:|---:|---|
| 初始 | 0 | 1 | 0 | 1 | - |
| 1 | 1 | 2 | 2 | 3 | 否 |
| 2 | 2 | 3 | 4 | 5 | 否 |
| 3 | 3 | 4 | 2 | 3 | 否 |
| 4 | 4 | 5 | 4 | 5 | 是 |

第一次相遇在下标 `4`，值为 `5`。

### 第二阶段变量表

初始化：

```text
finder = 下标 0，值 1
slow = 下标 4，值 5
```

| 轮次 | `finder` 下标 | `finder` 值 | `slow` 下标 | `slow` 值 | 是否相遇 |
|---:|---:|---:|---:|---:|---|
| 初始 | 0 | 1 | 4 | 5 | 否 |
| 1 | 1 | 2 | 5 | 6 | 否 |
| 2 | 2 | 3 | 2 | 3 | 是 |

返回下标 `2` 的节点 `3`。

## 为什么循环条件这样写

代码：

```cpp
while (fast != nullptr && fast->next != nullptr)
```

循环体会执行：

```cpp
fast = fast->next->next;
```

所以必须保证：

```text
fast 不是空指针
fast->next 也不是空指针
```

如果只写：

```cpp
while (fast != nullptr)
```

当 `fast->next == nullptr` 时，再访问：

```cpp
fast->next->next
```

就会发生空指针访问。

## 为什么第一阶段不能在移动前直接判断

初始时：

```text
slow == fast == head
```

如果一开始就用：

```cpp
while (slow != fast)
```

循环会因为二者初始相等而完全不执行，错误地认为已经相遇。

常见正确写法有两种：

### 写法一：先移动，再判断

```cpp
while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;

    if (slow == fast) {
        ...
    }
}
```

### 写法二：使用 `do-while`

但 C++ 中为了同时处理空指针，第一种通常更直观。

## 为什么返回节点而不是值

链表中可能有多个值相同的节点。

例如：

```text
1 -> 2 -> 1 -> 3
          ^       |
          |_______|
```

即使入口节点的值是 `1`，仅返回值无法区分它是第一个 `1` 还是第二个 `1`。

题目要求返回：

```text
节点地址
```

所以代码返回：

```cpp
return finder;
```

## 正确性证明

### 第一部分：有环时快慢指针一定相遇

当两个指针进入环后，`fast` 每轮比 `slow` 多走一步。

环长有限，所以二者之间的环内距离每轮减少一格并循环变化，最终一定变为 `0`，即二者相遇。

### 第二部分：无环时一定返回空

无环链表有明确末尾。

`fast` 每轮走两步，最终会使：

```text
fast == nullptr
```

或：

```text
fast->next == nullptr
```

循环结束并返回 `nullptr`。

### 第三部分：第二阶段一定在环入口相遇

设：

```text
a = 头到入口距离
b = 入口到第一次相遇点距离
L = 环长
```

第一次相遇时：

```text
a + b = kL
```

所以：

```text
a = kL - b
```

从相遇点再走 `a` 步，相当于沿环走：

```text
kL - b
```

去掉完整圈后，就是从相遇点走回入口的距离。

因此：

- `finder` 从头走 `a` 步到入口。
- `slow` 从相遇点走 `a` 步也到入口。

二者在环入口相遇，返回值正确。

## 复杂度分析

设链表节点数为 `n`。

时间复杂度：

```text
O(n)
```

第一阶段最多走线性数量的步数。

第二阶段也最多走线性数量的步数。

总时间仍然是：

```text
O(n)
```

空间复杂度：

```text
O(1)
```

只使用：

```text
slow
fast
finder
```

三个指针，没有使用与节点数量相关的额外容器。

## 另一种方法：哈希集合

可以用哈希集合记录访问过的节点地址：

```cpp
unordered_set<ListNode*> visited;

while (head != nullptr) {
    if (visited.count(head)) {
        return head;
    }

    visited.insert(head);
    head = head->next;
}

return nullptr;
```

思路：

```text
第一次访问节点：放入集合
再次访问同一节点：这个节点就是环入口
```

复杂度：

```text
时间复杂度：O(n)
空间复杂度：O(n)
```

哈希法更直观，但 Floyd 算法只需要 `O(1)` 额外空间，因此更优。

## 两种方法对比

| 方法 | 时间复杂度 | 空间复杂度 | 特点 |
|---|---:|---:|---|
| 哈希集合 | `O(n)` | `O(n)` | 容易理解 |
| Floyd 快慢指针 | `O(n)` | `O(1)` | 空间最优，需要数学推导 |

## 常见错误

### 错误 1：只判断有没有环，没有寻找入口

`slow == fast` 只能说明存在环。

第一次相遇点不一定是环入口。

例如官方示例中：

```text
入口是节点 2
第一次相遇点是节点 -4
```

还需要第二阶段。

### 错误 2：相遇后让 `fast` 回到头，但仍每次走两步

第二阶段两个指针必须都每次走一步。

正确：

```cpp
finder = finder->next;
slow = slow->next;
```

### 错误 3：返回节点值

题目要求返回 `ListNode*`，不是 `int`。

正确：

```cpp
return finder;
```

### 错误 4：循环条件缺少 `fast->next`

错误：

```cpp
while (fast != nullptr)
```

正确：

```cpp
while (fast != nullptr && fast->next != nullptr)
```

### 错误 5：用节点值判断相遇

错误：

```cpp
slow->val == fast->val
```

不同节点可能拥有相同值。

必须比较节点地址：

```cpp
slow == fast
```

### 错误 6：修改链表来做标记

题目要求不能修改链表。

不要通过修改 `next` 或节点值来记录访问状态。

### 错误 7：空链表处理不当

当：

```text
head == nullptr
```

代码中的：

```cpp
while (fast != nullptr && fast->next != nullptr)
```

会直接失败并返回 `nullptr`，无需单独特判。

## 通过这道题可以学习到什么

### 1. 学会快慢指针

快慢指针常用于处理：

- 链表是否有环
- 环入口
- 链表中点
- 倒数第 k 个节点
- 重复状态检测

不同速度可以制造“追赶关系”。

### 2. 学会把问题拆成两个阶段

本题不是一步完成：

```text
第一阶段：证明有环并找到相遇点
第二阶段：利用相遇点找到入口
```

面对复杂问题时，先解决中间目标，再利用中间结果解决最终目标，是非常重要的解题方法。

### 3. 学会比较指针而不是值

链表问题中，节点身份由地址决定。

两个节点值相同，不代表它们是同一个节点。

所以要写：

```cpp
slow == fast
```

而不是：

```cpp
slow->val == fast->val
```

### 4. 学会安全访问指针

访问：

```cpp
fast->next->next
```

之前必须保证：

```text
fast != nullptr
fast->next != nullptr
```

这能训练空指针保护意识。

### 5. 学会使用数学证明算法

第二阶段看起来很神奇，但可以通过路程关系证明：

```text
快指针路程 = 2 × 慢指针路程
快指针多走整数圈
```

最终推导出头到入口距离和相遇点到入口距离的关系。

### 6. 学会循环不变量

第一阶段始终保持：

```text
slow 每轮走 1 步
fast 每轮走 2 步
```

第二阶段始终保持：

```text
finder 和 slow 每轮都走 1 步
```

理解这些不变量有助于证明算法和调试代码。

### 7. 学会用 O(1) 空间替代哈希表

哈希集合可以解决问题，但需要 `O(n)` 空间。

Floyd 算法只使用几个指针，就把空间降为：

```text
O(1)
```

这是典型的时间不变、空间优化。

## 还可以延伸学习什么知识

### 1. 141. 环形链表

141 只要求判断是否有环。

只需要 Floyd 的第一阶段：

```text
相遇 -> 有环
fast 到 null -> 无环
```

142 在此基础上继续寻找入口。

### 2. 287. 寻找重复数

数组中的值可以被看作“下一个位置”，从而形成函数图。

寻找重复数也可以转化为寻找环入口，并使用 Floyd 算法。

这是快慢指针从链表扩展到数组的重要应用。

### 3. 函数图

如果每个状态都有唯一的下一个状态：

```text
x -> f(x)
```

不断迭代后可能进入循环。

这种结构叫函数图。

链表只是函数图的一种简单形式。

### 4. 环的长度

找到第一次相遇点后，可以让一个指针绕环走一圈并计数：

```cpp
int cycleLength = 1;
ListNode* current = slow->next;

while (current != slow) {
    current = current->next;
    cycleLength++;
}
```

这样可以得到环长。

### 5. 环前长度

第二阶段中，`finder` 和 `slow` 每走一步就计数。

相遇时的步数就是：

```text
从 head 到环入口的距离
```

### 6. 链表相交

链表相交问题也常使用：

```text
两个指针改变起点
利用路程补偿
```

与本题第二阶段“让两段路程对齐”的思想有相似之处。

### 7. 模运算

在环上移动可以使用模运算理解：

```text
位置 = 已走步数 mod 环长
```

快慢指针相遇的本质是：

```text
两者在模 L 意义下位置相同
```

## 学完后的自查问题

1. `slow` 和 `fast` 每轮分别走几步？
2. 无环时为什么 `fast` 一定会到达空指针？
3. 第一次相遇点一定是环入口吗？
4. 相遇后 `finder` 从哪里出发？
5. 第二阶段 `finder` 和 `slow` 每轮各走几步？
6. 为什么比较的是 `slow == fast`，而不是节点值？
7. 循环条件为什么要检查 `fast->next`？
8. Floyd 算法的时间和空间复杂度分别是多少？
9. 哈希集合方法有什么缺点？
10. 单节点自环能否被正确检测？

参考答案：

```text
1. slow 走 1 步，fast 走 2 步。
2. 无环链表有明确末尾，fast 不断前进最终会到达 nullptr。
3. 不一定，它只是环中的某个相遇点。
4. 从 head 出发。
5. 都走 1 步。
6. 节点身份由地址决定，不同节点可能有相同值。
7. 因为代码需要访问 fast->next->next。
8. 时间 O(n)，空间 O(1)。
9. 需要 O(n) 额外空间。
10. 可以，第一轮移动后 slow 和 fast 都仍指向该节点。
```

## 总结

这道题可以记成：

```text
一快一慢找相遇
一头一会合找入口
```

第一阶段：

```cpp
slow = slow->next;
fast = fast->next->next;
```

第二阶段：

```cpp
finder = head;

while (finder != slow) {
    finder = finder->next;
    slow = slow->next;
}
```

核心数学关系：

```text
a + b = kL
a = (k - 1)L + c
```

复杂度：

```text
时间复杂度：O(n)
空间复杂度：O(1)
```

通过这道题应重点掌握：

```text
快慢指针
环检测
环入口推导
指针安全访问
节点地址比较
模运算
函数图
用数学关系证明算法
```
