# 42. 接雨水

题目链接：[LeetCode 42. 接雨水](https://leetcode.cn/problems/trapping-rain-water/)

## 题目描述

给定 `n` 个非负整数表示柱子的高度，每根柱子的宽度都是 `1`。

下雨以后，某些柱子之间会形成低洼区域并储存雨水。请计算这些柱子一共能够接多少单位的雨水。

例如：

```text
height = [0,1,0,2,1,0,1,3,2,1,2,1]
```

最终可以接到：

```text
6
```

单位的雨水。

## 图中符号说明

后面的 ASCII 图解使用：

```text
█  表示柱子
~  表示雨水
.  表示空白
```

数组下标从 `0` 开始。

## 示例 1 总体图解

输入：

```text
height = [0,1,0,2,1,0,1,3,2,1,2,1]
```

柱子和雨水的最终分布可以画成：

```text
高度 3 | . . . . . . . █ . . . .
高度 2 | . . . █ ~ ~ ~ █ █ ~ █ .
高度 1 | . █ ~ █ █ ~ █ █ █ █ █ █
       +-------------------------
下标     0 1 2 3 4 5 6 7 8 9 10 11
柱高     0 1 0 2 1 0 1 3 2 1  2  1
```

逐列看雨水：

```text
下标 2：1 单位
下标 4：1 单位
下标 5：2 单位
下标 6：1 单位
下标 9：1 单位
```

总雨水：

```text
1 + 1 + 2 + 1 + 1 = 6
```

## 最重要的观察

一根柱子上方能不能接水，不取决于它自己有多矮，而取决于它左右两边有没有更高的边界。

对于位置 `i`：

```text
leftMax[i]  = i 左边（包括 i）最高柱子的高度
rightMax[i] = i 右边（包括 i）最高柱子的高度
```

位置 `i` 的水面高度由较矮的边界决定：

```text
水面高度 = min(leftMax[i], rightMax[i])
```

因此位置 `i` 的雨水量是：

```text
water[i] = min(leftMax[i], rightMax[i]) - height[i]
```

如果结果为 `0`，说明这个位置没有水。

## 为什么取左右最高值中的较小值

看一个简单例子：

```text
height = [4, 0, 2]
```

图解：

```text
高度 4 | █ . .
高度 3 | █ . .
高度 2 | █ ~ █
高度 1 | █ ~ █
       +------
下标     0 1 2
```

中间位置左边最高是 `4`，右边最高是 `2`。

雨水不可能达到高度 `4`，因为右边界只有 `2`，超过 `2` 的水会从右边流走。

所以水面高度只能是：

```text
min(4, 2) = 2
```

中间柱高是 `0`，接水量为：

```text
2 - 0 = 2
```

这说明：

```text
装水能力由较短的边界决定。
```

这和木桶原理相同：木桶能装多少水，由最短的木板决定。

## 解法选择

这道题有多种经典解法：

| 方法 | 时间复杂度 | 空间复杂度 | 特点 |
|---|---:|---:|---|
| 每个位置向两边寻找最高值 | `O(n^2)` | `O(1)` | 最直观，但较慢 |
| 前缀最大值 + 后缀最大值 | `O(n)` | `O(n)` | 容易理解 |
| 单调栈 | `O(n)` | `O(n)` | 按横向区域计算 |
| 双指针 | `O(n)` | `O(1)` | 时间和空间都最优 |

本文主讲双指针，同时在后面介绍其他方法，帮助理解它们之间的联系。

## 双指针核心思路

使用两个指针：

```cpp
int left = 0;
int right = height.size() - 1;
```

它们分别从数组两端向中间移动。

同时维护：

```cpp
int leftMax = 0;
int rightMax = 0;
int water = 0;
```

变量含义：

| 变量 | 含义 |
|---|---|
| `left` | 当前等待处理的左侧下标 |
| `right` | 当前等待处理的右侧下标 |
| `height[left]` | 当前左侧柱高 |
| `height[right]` | 当前右侧柱高 |
| `leftMax` | 从下标 `0` 到 `left` 的最高柱高 |
| `rightMax` | 从 `right` 到末尾的最高柱高 |
| `water` | 已经确定并累加的总雨水 |

循环：

```cpp
while (left < right)
```

每轮比较：

```cpp
height[left]
height[right]
```

### 当左柱不高于右柱

条件：

```cpp
height[left] <= height[right]
```

当前右边至少有 `height[right]` 这根柱子作为边界，并且它不低于当前左柱。

因此，可以确定当前 `left` 位置的水量：

```cpp
leftMax = max(leftMax, height[left]);
water += leftMax - height[left];
left++;
```

### 当左柱高于右柱

条件：

```cpp
height[left] > height[right]
```

当前左边至少有 `height[left]` 这根柱子作为边界，并且它高于当前右柱。

因此，可以确定当前 `right` 位置的水量：

```cpp
rightMax = max(rightMax, height[right]);
water += rightMax - height[right];
right--;
```

## 为什么可以先处理较矮的一侧

这是双指针解法最关键、也最容易困惑的地方。

假设：

```text
height[left] <= height[right]
```

这说明 `left` 右侧至少存在一根高度为 `height[right]` 的柱子。

对于当前 `left`：

- 如果 `leftMax <= height[left]`，当前柱刷新了左侧最高值，不能接水。
- 如果 `leftMax > height[left]`，左边界高于当前柱。
- 右边又明确存在一根不低于 `height[left]` 的柱子。

所以当前位置不会因为“右边没有边界”而无法接水，可以安全计算：

```text
leftMax - height[left]
```

右侧同理。

可以把它记成：

```text
哪一侧当前更矮，就先结算哪一侧。
```

## 带详细注释的代码

完整代码见 [solution.cpp](D:\leetcode\2026.6\42. 接雨水\solution.cpp)。

LeetCode 提交时只需要：

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        if (height.size() < 3) {
            return 0;
        }

        int left = 0;
        int right = static_cast<int>(height.size()) - 1;
        int leftMax = 0;
        int rightMax = 0;
        int water = 0;

        while (left < right) {
            if (height[left] <= height[right]) {
                leftMax = max(leftMax, height[left]);
                water += leftMax - height[left];
                left++;
            } else {
                rightMax = max(rightMax, height[right]);
                water += rightMax - height[right];
                right--;
            }
        }

        return water;
    }
};
```

## 示例 1：完整逐轮推演

输入：

```text
height = [0,1,0,2,1,0,1,3,2,1,2,1]
```

初始变量：

```text
left = 0
right = 11
leftMax = 0
rightMax = 0
water = 0
```

### 第 1 轮

循环前：

```text
left = 0,  height[left] = 0
right = 11, height[right] = 1
leftMax = 0
rightMax = 0
water = 0
```

比较：

```text
height[left] <= height[right]
0 <= 1，成立
```

进入左侧分支：

```cpp
leftMax = max(leftMax, height[left]);
```

得到：

```text
leftMax = max(0, 0) = 0
```

当前下标 `0` 的水量：

```cpp
leftMax - height[left]
```

即：

```text
0 - 0 = 0
```

累加：

```text
water = 0 + 0 = 0
```

移动：

```text
left = 1
```

### 第 2 轮

循环前：

```text
left = 1, height[left] = 1
right = 11, height[right] = 1
leftMax = 0
rightMax = 0
water = 0
```

因为：

```text
1 <= 1
```

处理左侧：

```text
leftMax = max(0, 1) = 1
当前水量 = 1 - 1 = 0
water = 0
left = 2
```

### 第 3 轮

循环前：

```text
left = 2, height[left] = 0
right = 11, height[right] = 1
leftMax = 1
rightMax = 0
water = 0
```

因为：

```text
0 <= 1
```

处理左侧：

```text
leftMax = max(1, 0) = 1
当前水量 = 1 - 0 = 1
water = 0 + 1 = 1
left = 3
```

这 `1` 单位水就是下标 `2` 上方的水：

```text
下标：1 2 3
柱高：1 0 2
      █ ~ █
```

### 第 4 轮

循环前：

```text
left = 3, height[left] = 2
right = 11, height[right] = 1
leftMax = 1
rightMax = 0
water = 1
```

因为：

```text
2 <= 1，不成立
```

处理右侧：

```text
rightMax = max(0, 1) = 1
当前水量 = 1 - 1 = 0
water = 1
right = 10
```

### 第 5 轮

循环前：

```text
left = 3, height[left] = 2
right = 10, height[right] = 2
leftMax = 1
rightMax = 1
water = 1
```

因为：

```text
2 <= 2
```

处理左侧：

```text
leftMax = max(1, 2) = 2
当前水量 = 2 - 2 = 0
water = 1
left = 4
```

### 第 6 轮

循环前：

```text
left = 4, height[left] = 1
right = 10, height[right] = 2
leftMax = 2
rightMax = 1
water = 1
```

处理左侧：

```text
leftMax = max(2, 1) = 2
当前水量 = 2 - 1 = 1
water = 1 + 1 = 2
left = 5
```

### 第 7 轮

循环前：

```text
left = 5, height[left] = 0
right = 10, height[right] = 2
leftMax = 2
rightMax = 1
water = 2
```

处理左侧：

```text
leftMax = max(2, 0) = 2
当前水量 = 2 - 0 = 2
water = 2 + 2 = 4
left = 6
```

下标 `5` 上方有两层水：

```text
高度 2 | █ ~ .
高度 1 | █ ~ █
       +------
下标     3 5 6
```

### 第 8 轮

循环前：

```text
left = 6, height[left] = 1
right = 10, height[right] = 2
leftMax = 2
rightMax = 1
water = 4
```

处理左侧：

```text
leftMax = max(2, 1) = 2
当前水量 = 2 - 1 = 1
water = 4 + 1 = 5
left = 7
```

### 第 9 轮

循环前：

```text
left = 7, height[left] = 3
right = 10, height[right] = 2
leftMax = 2
rightMax = 1
water = 5
```

因为：

```text
3 <= 2，不成立
```

处理右侧：

```text
rightMax = max(1, 2) = 2
当前水量 = 2 - 2 = 0
water = 5
right = 9
```

### 第 10 轮

循环前：

```text
left = 7, height[left] = 3
right = 9, height[right] = 1
leftMax = 2
rightMax = 2
water = 5
```

处理右侧：

```text
rightMax = max(2, 1) = 2
当前水量 = 2 - 1 = 1
water = 5 + 1 = 6
right = 8
```

### 第 11 轮

循环前：

```text
left = 7, height[left] = 3
right = 8, height[right] = 2
leftMax = 2
rightMax = 2
water = 6
```

处理右侧：

```text
rightMax = max(2, 2) = 2
当前水量 = 2 - 2 = 0
water = 6
right = 7
```

现在：

```text
left = 7
right = 7
```

循环条件 `left < right` 不成立，结束。

最终答案：

```text
water = 6
```

### 示例 1 变量总表

| 轮次 | 处理侧 | 处理下标 | 柱高 | 更新后 `leftMax` | 更新后 `rightMax` | 本轮加水 | 更新后 `water` |
|---:|---|---:|---:|---:|---:|---:|---:|
| 1 | 左 | 0 | 0 | 0 | 0 | 0 | 0 |
| 2 | 左 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3 | 左 | 2 | 0 | 1 | 0 | 1 | 1 |
| 4 | 右 | 11 | 1 | 1 | 1 | 0 | 1 |
| 5 | 左 | 3 | 2 | 2 | 1 | 0 | 1 |
| 6 | 左 | 4 | 1 | 2 | 1 | 1 | 2 |
| 7 | 左 | 5 | 0 | 2 | 1 | 2 | 4 |
| 8 | 左 | 6 | 1 | 2 | 1 | 1 | 5 |
| 9 | 右 | 10 | 2 | 2 | 2 | 0 | 5 |
| 10 | 右 | 9 | 1 | 2 | 2 | 1 | 6 |
| 11 | 右 | 8 | 2 | 2 | 2 | 0 | 6 |

## 示例 2：`[4,2,0,3,2,5]`

输入：

```text
height = [4,2,0,3,2,5]
```

图解：

```text
高度 5 | . . . . . █
高度 4 | █ ~ ~ ~ ~ █
高度 3 | █ ~ ~ █ ~ █
高度 2 | █ █ ~ █ █ █
高度 1 | █ █ ~ █ █ █
       +------------
下标     0 1 2 3 4 5
```

逐列水量：

```text
下标 0：0
下标 1：4 - 2 = 2
下标 2：4 - 0 = 4
下标 3：4 - 3 = 1
下标 4：4 - 2 = 2
下标 5：0
```

总水量：

```text
2 + 4 + 1 + 2 = 9
```

### 双指针逐轮过程

初始：

```text
left = 0
right = 5
leftMax = 0
rightMax = 0
water = 0
```

| 轮次 | `left` | `right` | `height[left]` | `height[right]` | 处理侧 | 本轮计算 | `water` |
|---:|---:|---:|---:|---:|---|---|---:|
| 1 | 0 | 5 | 4 | 5 | 左 | `leftMax=4`, 加 `4-4=0` | 0 |
| 2 | 1 | 5 | 2 | 5 | 左 | `leftMax=4`, 加 `4-2=2` | 2 |
| 3 | 2 | 5 | 0 | 5 | 左 | `leftMax=4`, 加 `4-0=4` | 6 |
| 4 | 3 | 5 | 3 | 5 | 左 | `leftMax=4`, 加 `4-3=1` | 7 |
| 5 | 4 | 5 | 2 | 5 | 左 | `leftMax=4`, 加 `4-2=2` | 9 |

最后：

```text
left = 5
right = 5
water = 9
```

## 示例 3：最小蓄水结构 `[2,0,2]`

图解：

```text
高度 2 | █ ~ █
高度 1 | █ ~ █
       +------
下标     0 1 2
```

中间位置：

```text
左边最高 = 2
右边最高 = 2
水面高度 = min(2, 2) = 2
柱高 = 0
雨水 = 2 - 0 = 2
```

双指针过程：

| 轮次 | `left` | `right` | 处理侧 | `leftMax` | `rightMax` | 本轮加水 | `water` |
|---:|---:|---:|---|---:|---:|---:|---:|
| 1 | 0 | 2 | 左 | 2 | 0 | 0 | 0 |
| 2 | 1 | 2 | 左 | 2 | 0 | 2 | 2 |

答案：

```text
2
```

## 示例 4：严格递减 `[3,2,1]`

图解：

```text
高度 3 | █ . .
高度 2 | █ █ .
高度 1 | █ █ █
       +------
下标     0 1 2
```

虽然左边很高，但右边没有更高的柱子封住水，所以水会全部流走。

双指针变量：

| 轮次 | `left` | `right` | 处理侧 | 更新后 `rightMax` | 本轮加水 |
|---:|---:|---:|---|---:|---:|
| 1 | 0 | 2 | 右 | 1 | `1 - 1 = 0` |
| 2 | 0 | 1 | 右 | 2 | `2 - 2 = 0` |

结果：

```text
water = 0
```

## 示例 5：严格递增 `[1,2,3,4]`

图解：

```text
高度 4 | . . . █
高度 3 | . . █ █
高度 2 | . █ █ █
高度 1 | █ █ █ █
       +--------
下标     0 1 2 3
```

每根柱子都比左边更高，没有低洼区域。

双指针每轮都更新 `leftMax`：

```text
leftMax：1 -> 2 -> 3
每轮水量：0 -> 0 -> 0
```

最终：

```text
water = 0
```

## 示例 6：相同高度 `[3,3,3]`

```text
高度 3 | █ █ █
高度 2 | █ █ █
高度 1 | █ █ █
       +------
下标     0 1 2
```

没有任何低洼位置，所以：

```text
water = 0
```

## 正确性说明

我们证明双指针每次计算的水量都是正确的。

### 情况一：`height[left] <= height[right]`

当前 `right` 本身就是 `left` 右侧的一根柱子，并且：

```text
height[right] >= height[left]
```

此时处理 `left`。

如果：

```text
height[left] >= leftMax
```

当前柱子成为新的左侧最高柱，当前位置不能接水：

```text
leftMax - height[left] = 0
```

如果：

```text
height[left] < leftMax
```

左侧存在高度为 `leftMax` 的边界，右侧也存在足够的边界。当前位置水量为：

```text
leftMax - height[left]
```

所以左侧计算正确。

### 情况二：`height[left] > height[right]`

与情况一对称。

当前 `left` 是 `right` 左侧的一根更高柱子，因此可以根据 `rightMax` 确定 `right` 位置水量：

```text
rightMax - height[right]
```

所以右侧计算正确。

每一轮都会正确处理一个位置，并将指针向中间移动。所有可能接水的位置最终都会被处理一次，因此最后得到的 `water` 是全部雨水总量。

## 复杂度分析

设柱子数量为 `n`。

时间复杂度：

```text
O(n)
```

`left` 只向右移动，`right` 只向左移动，每个位置最多处理一次。

空间复杂度：

```text
O(1)
```

只使用了固定数量的变量：

```text
left
right
leftMax
rightMax
water
```

额外空间不会随着 `n` 增长。

## 方法二：前缀最大值和后缀最大值

这是理解公式最直接的方法。

建立两个数组：

```cpp
leftMax[i]
rightMax[i]
```

其中：

```text
leftMax[i] = 下标 0 到 i 的最高柱子
rightMax[i] = 下标 i 到 n-1 的最高柱子
```

然后逐位置计算：

```cpp
water += min(leftMax[i], rightMax[i]) - height[i];
```

代码：

```cpp
int trap(vector<int>& height) {
    int n = height.size();
    if (n < 3) {
        return 0;
    }

    vector<int> leftMax(n);
    vector<int> rightMax(n);

    leftMax[0] = height[0];
    for (int i = 1; i < n; i++) {
        leftMax[i] = max(leftMax[i - 1], height[i]);
    }

    rightMax[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        rightMax[i] = max(rightMax[i + 1], height[i]);
    }

    int water = 0;
    for (int i = 0; i < n; i++) {
        water += min(leftMax[i], rightMax[i]) - height[i];
    }

    return water;
}
```

对于示例 1：

```text
height   = [0,1,0,2,1,0,1,3,2,1,2,1]
leftMax  = [0,1,1,2,2,2,2,3,3,3,3,3]
rightMax = [3,3,3,3,3,3,3,3,2,2,2,1]
```

逐列雨水：

```text
min(leftMax, rightMax) - height

下标 0：min(0,3) - 0 = 0
下标 1：min(1,3) - 1 = 0
下标 2：min(1,3) - 0 = 1
下标 3：min(2,3) - 2 = 0
下标 4：min(2,3) - 1 = 1
下标 5：min(2,3) - 0 = 2
下标 6：min(2,3) - 1 = 1
下标 7：min(3,3) - 3 = 0
下标 8：min(3,2) - 2 = 0
下标 9：min(3,2) - 1 = 1
下标 10：min(3,2) - 2 = 0
下标 11：min(3,1) - 1 = 0
```

总和：

```text
6
```

这种方法时间复杂度为 `O(n)`，空间复杂度为 `O(n)`。

双指针可以理解为：不再完整保存 `leftMax[]` 和 `rightMax[]`，而是一边移动一边维护当前需要的最大值。

## 方法三：单调栈

前缀数组和双指针都是“按列”计算雨水。

单调栈则是“按横向区域”计算雨水。

维护一个从栈底到栈顶柱高单调不增的下标栈。

当遇到一根更高柱子时，说明可能形成凹槽：

```text
左边界：栈中新的栈顶
凹槽底：刚刚弹出的下标
右边界：当前下标
```

横向宽度：

```text
width = right - left - 1
```

有效高度：

```text
boundedHeight =
    min(height[left], height[right]) - height[bottom]
```

本层雨水：

```text
width * boundedHeight
```

单调栈适合帮助理解“遇到右边界后，结算之前的凹槽”。

## 常见错误

### 错误 1：只看左右相邻柱子

位置 `i` 的水量不是由 `height[i - 1]` 和 `height[i + 1]` 决定，而是由整个左侧和右侧的最高柱子决定。

例如：

```text
[4,2,0,3,2,5]
```

下标 `2` 左右相邻柱是 `2` 和 `3`，但真正水面高度是 `4`，因为更远处有左右高墙 `4` 和 `5`。

### 错误 2：使用较高边界

错误公式：

```text
max(leftMax, rightMax) - height[i]
```

正确公式：

```text
min(leftMax, rightMax) - height[i]
```

水会从较矮的一边流走。

### 错误 3：忘记更新最大值

必须先执行：

```cpp
leftMax = max(leftMax, height[left]);
```

再计算：

```cpp
water += leftMax - height[left];
```

这样结果才不会是负数。

### 错误 4：两个指针移动方向写反

正确方向：

```text
left 向右移动：left++
right 向左移动：right--
```

### 错误 5：认为最高柱子本身能接水

当某根柱子刷新 `leftMax` 或 `rightMax` 时：

```text
最大值 - 当前高度 = 0
```

所以边界柱本身不会贡献雨水。

### 错误 6：少于三根柱子还继续计算

形成蓄水区域至少需要：

```text
左边界 + 中间低洼 + 右边界
```

少于三根柱子一定接不到水。

## 通过这道题可以学习到什么

### 1. 学会把二维图形转换成一维计算

题目看起来像一个二维蓄水图形，但输入只是一个一维数组。

关键是按列思考：

```text
每一列能装多少水？
```

然后把所有列的水量相加。

这种“把整体问题拆成每个位置的贡献”的思想很常见。

### 2. 学会木桶原理

一个位置的水面高度由左右边界中的较矮者决定：

```text
min(leftMax, rightMax)
```

这就是木桶原理在算法中的体现。

### 3. 学会双指针

双指针不只是“两个下标一起移动”。

本题的双指针还包含一个重要决策：

```text
根据两端高度，决定哪一侧的信息已经足够，可以先结算。
```

这比普通的左右夹逼更进一步。

### 4. 学会维护动态边界

`leftMax` 和 `rightMax` 是不断变化的边界信息。

移动指针时同步更新：

```cpp
leftMax = max(leftMax, height[left]);
rightMax = max(rightMax, height[right]);
```

这种“扫描过程中维护最优值”的技巧还会出现在：

- 最大利润
- 前缀最大值
- 滑动窗口
- 动态规划状态压缩

### 5. 学会从空间 `O(n)` 优化到 `O(1)`

前缀/后缀数组方法需要：

```text
leftMax[]
rightMax[]
```

双指针发现每一轮只需要当前的：

```text
leftMax
rightMax
```

于是把额外空间从 `O(n)` 降到 `O(1)`。

这能训练你思考：

```text
之前保存的所有信息真的都需要吗？
还是只需要一个滚动状态？
```

### 6. 学会证明贪心选择

双指针每次选择较矮的一侧进行结算，本质上带有贪心思想。

要相信这个选择，需要证明：

```text
当前较矮一侧的水量已经能确定，不会被中间未知柱子改变。
```

学习这题可以帮助你从“代码看起来能过”提升到“我知道为什么一定正确”。

### 7. 学会用变量表调试

本题适合使用以下表头逐轮检查：

```text
left
right
height[left]
height[right]
leftMax
rightMax
本轮加水
water
```

如果代码答案不对，手动走几轮通常很快就能发现：

- 最大值更新顺序错误
- 指针移动错误
- 分支条件错误
- 水量公式错误

## 还可以延伸学习什么知识

### 1. 前缀最大值与后缀最大值

本题公式直接依赖：

```text
左侧最大值
右侧最大值
```

这是一类典型的前缀/后缀预处理问题。

类似技巧还用于：

- 除自身以外数组的乘积
- 分割数组
- 左右两侧最大值比较
- 前缀和与后缀和

### 2. 单调栈

单调栈可以快速找到：

- 左边第一个更高元素
- 右边第一个更高元素
- 柱状图中的矩形
- 能够形成凹槽的左右边界

接雨水和“柱状图中最大的矩形”是学习单调栈时经常一起练习的两道经典题。

### 3. 按列计算与按层计算

同一个雨水区域可以有两种计算方式：

```text
按列：每个下标上方有多少水
按层：左右边界之间这一层有多少水
```

双指针和前缀数组是按列计算。

单调栈是按层计算。

理解两种视角有助于你面对新问题时选择更自然的建模方式。

### 4. 不变量

双指针算法中可以关注这些不变量：

```text
left 左边的位置都已经计算完成
right 右边的位置都已经计算完成
leftMax 是左侧扫描范围内最高值
rightMax 是右侧扫描范围内最高值
water 是所有已处理位置的雨水总和
```

不变量是证明循环算法正确的重要工具。

### 5. 二维接雨水

这道题是一维接雨水。

进一步可以学习：

```text
407. 接雨水 II
```

二维版本不能简单使用左右双指针，通常需要：

- 最小堆
- 从边界向内部扩展
- 类似 Dijkstra 的思想

### 6. 盛最多水的容器

相关题目：

```text
11. 盛最多水的容器
```

两道题都使用双指针，但目标不同：

```text
接雨水：计算所有低洼位置的水量总和
盛水容器：选择两根柱子，使单个容器面积最大
```

比较这两道题，可以帮助你理解不同双指针策略的依据。

## 学完后的自查问题

1. 为什么某列水量要使用 `min(leftMax, rightMax)`？
2. 为什么双指针每次可以先处理较矮的一侧？
3. `leftMax` 和 `rightMax` 分别表示什么？
4. 为什么更新最大值后再计算水量不会得到负数？
5. `[2,0,2]` 能接多少水？
6. `[3,2,1]` 为什么接不到水？
7. 前缀数组方法为什么需要 `O(n)` 额外空间？
8. 单调栈是按列计算还是按横向区域计算？

参考答案：

```text
1. 水会从较矮边界流走，水面不能超过较矮边界。
2. 较矮一侧已经能确认另一边存在足够的边界，可以结算当前位置。
3. 它们分别是当前左侧和右侧扫描范围内的最高柱高。
4. 更新后最大值一定不小于当前柱高。
5. 2 单位。
6. 右侧没有能封住水的边界，水会流走。
7. 需要保存每个下标对应的左右最高值。
8. 按横向区域计算。
```

## 总结

接雨水最核心的公式是：

```text
water[i] = min(leftMax[i], rightMax[i]) - height[i]
```

双指针通过比较两端高度，决定先计算哪一侧，并动态维护：

```text
leftMax
rightMax
```

最终实现：

```text
时间复杂度 O(n)
空间复杂度 O(1)
```

真正需要掌握的不只是代码，还包括：

```text
按列拆分贡献
木桶原理
动态边界
双指针选择依据
循环不变量
从 O(n) 空间优化到 O(1)
```
