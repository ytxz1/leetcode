# 1840. 最高建筑高度

题目链接：[LeetCode 1840. 最高建筑高度](https://leetcode.cn/problems/maximum-building-height/)

## 题目描述

一共有 `n` 栋建筑，编号从 `1` 到 `n`。

每栋建筑都有一个非负整数高度，并且必须满足：

1. 第 `1` 栋建筑的高度是 `0`。
2. 相邻两栋建筑的高度差不能超过 `1`。
3. `restrictions[i] = [id, maxHeight]` 表示第 `id` 栋建筑的高度不能超过 `maxHeight`。

要求返回所有建筑中能够达到的最大高度。

## 题目中的三个关键限制

### 限制 1：第 1 栋建筑高度固定为 0

```text
building[1] = 0
```

这是一个隐含限制点：

```text
[1, 0]
```

后面的算法会主动把它加入限制数组。

### 限制 2：相邻高度差不能超过 1

如果第 `i` 栋建筑高度为 `h`，那么第 `i + 1` 栋只能是：

```text
h - 1
h
h + 1
```

并且高度不能小于 `0`。

例如：

```text
建筑编号：1  2  3  4  5
合法高度：0  1  2  3  4
```

因为每次只增加 `1`。

下面是不合法的：

```text
建筑编号：1  2
高度：    0  2
```

相邻高度差是 `2`，超过了 `1`。

### 限制 3：某些建筑有额外高度上限

例如：

```text
restrictions = [[2,1], [4,1]]
```

表示：

```text
第 2 栋高度 <= 1
第 4 栋高度 <= 1
```

注意，这只是上限，不是要求高度必须等于该值。

## 为什么不能只看限制点

假设：

```text
n = 7
restrictions = [[3,1], [7,1]]
```

限制点高度都不超过 `1`，但中间建筑可以更高：

```text
建筑编号：1  2  3  4  5  6  7
一种高度：0  1  1  2  3  2  1
```

图解：

```text
高度 3 | . . . . █ . .
高度 2 | . . . █ █ █ .
高度 1 | . █ █ █ █ █ █
       +--------------
编号     1 2 3 4 5 6 7
高度     0 1 1 2 3 2 1
```

最高建筑是第 `5` 栋，高度为 `3`。

所以答案不一定出现在限制点上，也可能出现在两个限制点之间。

## 解题总览

算法分为四步：

```text
1. 加入隐含限制点 [1, 0]
2. 按建筑编号排序并合并重复编号
3. 从左往右、从右往左收紧高度上限
4. 计算每两个相邻限制点之间的最高峰，并计算最后一段
```

代码中的核心数据：

```cpp
vector<vector<long long>> points;
```

其中：

```text
points[i][0] = 建筑编号 id
points[i][1] = 当前允许的最大高度 limit
```

## 第一步：加入 `[1,0]`

代码：

```cpp
points.push_back({1, 0});
```

原因是第 `1` 栋建筑高度固定为 `0`。

如果不加入这个限制，算法就无法把“第 1 栋必须为 0”的影响传播到后面的建筑。

例如没有其他限制时：

```text
n = 6
```

由于第 1 栋是 `0`，最高合法高度只能是：

```text
建筑编号：1 2 3 4 5 6
高度：    0 1 2 3 4 5
```

答案是 `5`，不是无限大。

## 第二步：排序

输入中的限制不一定按建筑编号排列。

例如：

```text
restrictions = [[7,4], [2,5], [10,3], [5,3]]
```

加入 `[1,0]` 后排序：

```text
[[1,0], [2,5], [5,3], [7,4], [10,3]]
```

排序后才能：

- 计算相邻限制点距离
- 从左向右传播约束
- 从右向左传播约束
- 计算相邻限制点之间的峰值

时间复杂度主要来自排序：

```text
O(m log m)
```

其中 `m` 是限制数量。

## 第三步：从左往右收紧上限

假设左边限制点是：

```text
(leftId, leftLimit)
```

当前限制点是：

```text
(curId, curLimit)
```

两点距离：

```text
distance = curId - leftId
```

因为相邻建筑每次最多升高 `1`，所以从左点走到当前点，最多升高：

```text
distance
```

因此当前点还必须满足：

```text
当前高度 <= leftLimit + distance
```

代码：

```cpp
long long distance = points[i][0] - points[i - 1][0];
long long limitFromLeft = points[i - 1][1] + distance;
points[i][1] = min(points[i][1], limitFromLeft);
```

### 左向右收紧示例

```text
限制点：[1,0], [2,5]
```

第 1 栋高度是 `0`，第 2 栋与它相邻。

第 2 栋最多只能是：

```text
0 + 1 = 1
```

所以输入中的上限 `5` 实际上不可能达到。

收紧后：

```text
[2,5] -> [2,1]
```

代码变量对应：

```text
i = 1
points[i - 1] = [1,0]
points[i] = [2,5]
distance = 2 - 1 = 1
limitFromLeft = 0 + 1 = 1
points[i][1] = min(5,1) = 1
```

## 为什么还要从右往左收紧

只从左向右传播还不够。

例如：

```text
限制点：[1,0], [5,100], [7,1]
```

从左向右处理 `[5,100]`：

```text
distance = 5 - 1 = 4
来自左边的上限 = 0 + 4 = 4
[5,100] 被收紧为 [5,4]
```

但是右边还有 `[7,1]`。

第 5 栋到第 7 栋距离是 `2`，第 7 栋最高只有 `1`，所以第 5 栋最多只能是：

```text
1 + 2 = 3
```

真实上限应该是 `3`，而不是 `4`。

所以必须再从右往左传播一次。

## 第四步：从右往左收紧上限

当前限制点：

```text
(curId, curLimit)
```

右边限制点：

```text
(rightId, rightLimit)
```

距离：

```text
distance = rightId - curId
```

当前点从右边限制点最多上升 `distance`，所以：

```text
当前高度 <= rightLimit + distance
```

代码：

```cpp
long long distance = points[i + 1][0] - points[i][0];
long long limitFromRight = points[i + 1][1] + distance;
points[i][1] = min(points[i][1], limitFromRight);
```

经过左右两次传播后，每个限制点都同时满足：

```text
输入给出的上限
左边所有约束传播来的上限
右边所有约束传播来的上限
```

## 区间最高峰怎么计算

考虑两个相邻限制点：

```text
左端点：(leftId, leftHeight)
右端点：(rightId, rightHeight)
```

距离：

```text
distance = rightId - leftId
```

从左边开始，每走一步最多升高 `1`：

```text
leftHeight
leftHeight + 1
leftHeight + 2
...
```

从右边向左看，每走一步最多升高 `1`：

```text
rightHeight
rightHeight + 1
rightHeight + 2
...
```

最高峰出现在两条上升线相遇的位置。

公式：

```text
peak = floor((leftHeight + rightHeight + distance) / 2)
```

代码：

```cpp
long long peak =
    (leftHeight + rightHeight + distance) / 2;
```

C++ 整数除法自动向下取整。

## 峰值公式详细推导

假设山峰距离左端点 `x` 步。

从左端点能够达到的高度：

```text
leftHeight + x
```

山峰距离右端点：

```text
distance - x
```

从右端点能够达到的高度：

```text
rightHeight + distance - x
```

山峰不能超过两边能够到达的较小值：

```text
min(
    leftHeight + x,
    rightHeight + distance - x
)
```

最高点大约出现在两者相等时：

```text
leftHeight + x
=
rightHeight + distance - x
```

移项：

```text
2x = rightHeight + distance - leftHeight
```

代回山峰高度：

```text
peak
= leftHeight + x
= leftHeight
  + (rightHeight + distance - leftHeight) / 2
= (leftHeight + rightHeight + distance) / 2
```

由于建筑高度和位置都是整数，需要向下取整：

```text
peak = floor((leftHeight + rightHeight + distance) / 2)
```

## 峰值图解 1：两端一样高

```text
leftHeight = 1
rightHeight = 1
distance = 4
```

峰值：

```text
peak = (1 + 1 + 4) / 2 = 3
```

可以构造：

```text
编号：3 4 5 6 7
高度：1 2 3 2 1
```

图解：

```text
高度 3 | . . █ . .
高度 2 | . █ █ █ .
高度 1 | █ █ █ █ █
       +----------
编号     3 4 5 6 7
```

## 峰值图解 2：两端高度不同

```text
leftHeight = 0
rightHeight = 1
distance = 3
```

峰值：

```text
peak = floor((0 + 1 + 3) / 2)
     = 2
```

一种合法高度：

```text
编号：1 2 3 4
高度：0 1 2 1
```

图解：

```text
高度 2 | . . █ .
高度 1 | . █ █ █
       +--------
编号     1 2 3 4
```

## 最后一个限制点右侧怎么处理

最后一个限制点之后没有新的高度上限。

假设最后限制点是：

```text
(lastId, lastHeight)
```

到第 `n` 栋还有：

```text
n - lastId
```

步。

每一步最多升高 `1`，所以尾部最高可以达到：

```text
tailPeak = lastHeight + (n - lastId)
```

代码：

```cpp
long long lastId = points.back()[0];
long long lastHeight = points.back()[1];
long long tailPeak =
    lastHeight + (static_cast<long long>(n) - lastId);

answer = max(answer, tailPeak);
```

### 尾部示例

```text
n = 8
restrictions = [[4,2]]
```

第 4 栋最高为 `2`，之后没有限制，可以继续上升：

```text
编号：1 2 3 4 5 6 7 8
高度：0 1 2 2 3 4 5 6
```

尾部计算：

```text
lastId = 4
lastHeight = 2
n - lastId = 8 - 4 = 4
tailPeak = 2 + 4 = 6
```

答案是 `6`。

## 完整代码

完整带详细注释的代码见：

[solution.cpp](D:\leetcode\2026.6\1840. 最高建筑高度\solution.cpp)

LeetCode 提交时只需要 `Solution` 类。

核心代码：

```cpp
class Solution {
public:
    int maxBuilding(int n, vector<vector<int>>& restrictions) {
        vector<vector<long long>> points;
        points.push_back({1, 0});

        for (const vector<int>& restriction : restrictions) {
            points.push_back({
                restriction[0],
                restriction[1]
            });
        }

        sort(points.begin(), points.end());

        vector<vector<long long>> merged;
        for (const vector<long long>& point : points) {
            if (!merged.empty() && merged.back()[0] == point[0]) {
                merged.back()[1] =
                    min(merged.back()[1], point[1]);
            } else {
                merged.push_back(point);
            }
        }
        points.swap(merged);

        int m = points.size();

        for (int i = 1; i < m; i++) {
            long long distance =
                points[i][0] - points[i - 1][0];
            long long limitFromLeft =
                points[i - 1][1] + distance;

            points[i][1] =
                min(points[i][1], limitFromLeft);
        }

        for (int i = m - 2; i >= 0; i--) {
            long long distance =
                points[i + 1][0] - points[i][0];
            long long limitFromRight =
                points[i + 1][1] + distance;

            points[i][1] =
                min(points[i][1], limitFromRight);
        }

        long long answer = 0;

        for (int i = 1; i < m; i++) {
            long long leftId = points[i - 1][0];
            long long leftHeight = points[i - 1][1];
            long long rightId = points[i][0];
            long long rightHeight = points[i][1];
            long long distance = rightId - leftId;

            long long peak =
                (leftHeight + rightHeight + distance) / 2;

            answer = max(answer, peak);
        }

        long long lastId = points.back()[0];
        long long lastHeight = points.back()[1];
        long long tailPeak =
            lastHeight + (static_cast<long long>(n) - lastId);

        answer = max(answer, tailPeak);

        return static_cast<int>(answer);
    }
};
```

## 示例 1：`n = 5`

输入：

```text
n = 5
restrictions = [[2,1], [4,1]]
```

### 1. 加入隐含限制

代码：

```cpp
points.push_back({1, 0});
```

得到：

```text
points = [[1,0], [2,1], [4,1]]
```

### 2. 排序

当前已经有序：

```text
[[1,0], [2,1], [4,1]]
```

### 3. 从左向右收紧

初始：

```text
points = [[1,0], [2,1], [4,1]]
```

#### `i = 1`

代码变量：

```text
points[i - 1] = [1,0]
points[i] = [2,1]
distance = 2 - 1 = 1
limitFromLeft = 0 + 1 = 1
```

更新：

```text
points[1][1] = min(1,1) = 1
```

结果不变：

```text
[[1,0], [2,1], [4,1]]
```

#### `i = 2`

```text
points[i - 1] = [2,1]
points[i] = [4,1]
distance = 4 - 2 = 2
limitFromLeft = 1 + 2 = 3
points[2][1] = min(1,3) = 1
```

仍然不变：

```text
[[1,0], [2,1], [4,1]]
```

### 4. 从右向左收紧

#### `i = 1`

```text
points[i] = [2,1]
points[i + 1] = [4,1]
distance = 4 - 2 = 2
limitFromRight = 1 + 2 = 3
points[1][1] = min(1,3) = 1
```

#### `i = 0`

```text
points[i] = [1,0]
points[i + 1] = [2,1]
distance = 2 - 1 = 1
limitFromRight = 1 + 1 = 2
points[0][1] = min(0,2) = 0
```

最终有效限制：

```text
points = [[1,0], [2,1], [4,1]]
```

### 5. 计算相邻区间峰值

初始：

```text
answer = 0
```

#### 区间 `[1,2]`

```text
leftId = 1
leftHeight = 0
rightId = 2
rightHeight = 1
distance = 2 - 1 = 1
```

峰值：

```text
peak = (0 + 1 + 1) / 2 = 1
answer = max(0,1) = 1
```

#### 区间 `[2,4]`

```text
leftId = 2
leftHeight = 1
rightId = 4
rightHeight = 1
distance = 4 - 2 = 2
```

峰值：

```text
peak = (1 + 1 + 2) / 2 = 2
answer = max(1,2) = 2
```

这段可以构造：

```text
编号：2 3 4
高度：1 2 1
```

### 6. 计算尾部

最后限制点：

```text
lastId = 4
lastHeight = 1
```

到第 `5` 栋还有一步：

```text
tailPeak = 1 + (5 - 4) = 2
answer = max(2,2) = 2
```

一种最优建筑高度：

```text
编号：1 2 3 4 5
高度：0 1 2 1 2
```

图解：

```text
高度 2 | . . █ . █
高度 1 | . █ █ █ █
       +----------
编号     1 2 3 4 5
```

最终答案：

```text
2
```

## 示例 2：没有额外限制

输入：

```text
n = 6
restrictions = []
```

加入隐含限制后：

```text
points = [[1,0]]
```

没有其他限制点，所以左右传播循环都不会执行。

相邻区间循环也不会执行。

只计算尾部：

```text
lastId = 1
lastHeight = 0
tailPeak = 0 + (6 - 1) = 5
```

可以构造：

```text
编号：1 2 3 4 5 6
高度：0 1 2 3 4 5
```

图解：

```text
高度 5 | . . . . . █
高度 4 | . . . . █ █
高度 3 | . . . █ █ █
高度 2 | . . █ █ █ █
高度 1 | . █ █ █ █ █
       +------------
编号     1 2 3 4 5 6
```

答案：

```text
5
```

## 示例 3：限制需要被大幅收紧

输入：

```text
n = 10
restrictions =
[
    [5,3],
    [2,5],
    [7,4],
    [10,3]
]
```

### 1. 加入 `[1,0]` 并排序

```text
points =
[
    [1,0],
    [2,5],
    [5,3],
    [7,4],
    [10,3]
]
```

### 2. 从左向右收紧

| `i` | 左点 | 当前点原上限 | `distance` | `limitFromLeft` | 当前点新上限 |
|---:|---|---:|---:|---:|---:|
| 1 | `[1,0]` | 5 | 1 | `0+1=1` | `min(5,1)=1` |
| 2 | `[2,1]` | 3 | 3 | `1+3=4` | `min(3,4)=3` |
| 3 | `[5,3]` | 4 | 2 | `3+2=5` | `min(4,5)=4` |
| 4 | `[7,4]` | 3 | 3 | `4+3=7` | `min(3,7)=3` |

左向右后：

```text
points =
[
    [1,0],
    [2,1],
    [5,3],
    [7,4],
    [10,3]
]
```

最明显的变化：

```text
[2,5] -> [2,1]
```

因为第 2 栋紧邻高度为 `0` 的第 1 栋，不可能达到 `5`。

### 3. 从右向左收紧

| `i` | 当前点 | 右点 | `distance` | `limitFromRight` | 当前点新上限 |
|---:|---|---|---:|---:|---:|
| 3 | `[7,4]` | `[10,3]` | 3 | `3+3=6` | `min(4,6)=4` |
| 2 | `[5,3]` | `[7,4]` | 2 | `4+2=6` | `min(3,6)=3` |
| 1 | `[2,1]` | `[5,3]` | 3 | `3+3=6` | `min(1,6)=1` |
| 0 | `[1,0]` | `[2,1]` | 1 | `1+1=2` | `min(0,2)=0` |

最终限制不再变化：

```text
[
    [1,0],
    [2,1],
    [5,3],
    [7,4],
    [10,3]
]
```

### 4. 计算区间峰值

| 区间 | `leftHeight` | `rightHeight` | `distance` | `peak` | 更新后 `answer` |
|---|---:|---:|---:|---:|---:|
| `[1,2]` | 0 | 1 | 1 | `(0+1+1)/2=1` | 1 |
| `[2,5]` | 1 | 3 | 3 | `(1+3+3)/2=3` | 3 |
| `[5,7]` | 3 | 4 | 2 | `(3+4+2)/2=4` | 4 |
| `[7,10]` | 4 | 3 | 3 | `(4+3+3)/2=5` | 5 |

最后限制点就是第 `10` 栋：

```text
tailPeak = 3 + (10 - 10) = 3
```

最终：

```text
answer = 5
```

区间 `[7,10]` 可以构造：

```text
编号：7 8 9 10
高度：4 5 4 3
```

最高值为 `5`。

## 示例 4：限制顺序混乱

输入：

```text
n = 9
restrictions = [[8,2], [3,2], [6,3]]
```

加入 `[1,0]` 后排序：

```text
points = [[1,0], [3,2], [6,3], [8,2]]
```

左右收紧后仍然是：

```text
[[1,0], [3,2], [6,3], [8,2]]
```

计算 `[3,6]`：

```text
leftId = 3
leftHeight = 2
rightId = 6
rightHeight = 3
distance = 3
```

```text
peak = (2 + 3 + 3) / 2 = 4
```

可以构造：

```text
编号：3 4 5 6
高度：2 3 4 3
```

所以答案至少是 `4`。

其他区间和尾部都不超过 `4`，最终答案是：

```text
4
```

这个例子说明：

```text
最高值可能出现在两个限制点之间，
不能只取 restrictions 中 maxHeight 的最大值。
```

## 示例 5：重复限制点

本地测试额外考虑：

```text
n = 6
restrictions = [[4,4], [4,1]]
```

同一栋建筑有两个上限：

```text
第 4 栋 <= 4
第 4 栋 <= 1
```

必须同时满足，因此真正上限是：

```text
min(4,1) = 1
```

合并后：

```text
points = [[1,0], [4,1]]
```

区间 `[1,4]`：

```text
distance = 3
peak = (0 + 1 + 3) / 2 = 2
```

尾部：

```text
tailPeak = 1 + (6 - 4) = 3
```

最终答案：

```text
3
```

## 正确性说明

### 1. 左右传播后，每个限制点上限有效

从左往右传播保证：

```text
任意限制点不会高到无法从左边限制点以每步最多 +1 到达。
```

从右往左传播保证：

```text
任意限制点不会高到无法下降到右边限制点。
```

因此处理后的每个限制点上限都同时满足左右相邻限制点的影响。

### 2. 相邻限制点间的峰值公式给出最大可行高度

任意中间位置的高度同时受到：

```text
左端点高度 + 距离
右端点高度 + 距离
```

两个上界限制。

取两者较小值，并在所有中间位置取最大，就会在两条边界线相遇处得到：

```text
floor((leftHeight + rightHeight + distance) / 2)
```

这个高度既不能再增大，又能通过每步高度差不超过 `1` 的方式构造出来，所以它就是该区间的最大可行高度。

### 3. 所有可能位置都被覆盖

建筑范围被分成：

```text
相邻限制点之间的所有区间
最后一个限制点到第 n 栋的尾部区间
```

算法计算了每个区间的最高值，也计算了尾部最高值。

所以所有建筑位置都被覆盖，最终 `answer` 是全局最大高度。

## 复杂度分析

设原始限制数量为 `m`。

排序：

```text
O(m log m)
```

合并重复限制：

```text
O(m)
```

从左往右传播：

```text
O(m)
```

从右往左传播：

```text
O(m)
```

计算所有区间峰值：

```text
O(m)
```

总时间复杂度：

```text
O(m log m)
```

空间复杂度：

```text
O(m)
```

用于保存排序和合并后的限制点。

## 为什么使用 `long long`

代码使用：

```cpp
long long distance;
long long limitFromLeft;
long long peak;
```

因为会计算：

```text
高度 + 距离
左高度 + 右高度 + 距离
```

即使最终答案能放进 `int`，中间加法也可能接近或超过 `int` 范围。

使用 `long long` 可以避免中间计算溢出。

## 常见错误

### 错误 1：忘记加入 `[1,0]`

第 1 栋高度为 `0` 是题目的强制限制。

忘记加入后，左侧限制无法正确传播。

### 错误 2：只从左往右传播一次

右侧较低的限制也可能收紧左侧限制点。

必须：

```text
左 -> 右
右 -> 左
```

各做一次。

### 错误 3：直接取限制点高度最大值

最高建筑可能在两个限制点之间。

例如：

```text
[3,1] 和 [7,1]
```

中间可以达到 `3`。

### 错误 4：峰值公式忘记距离

错误：

```text
(leftHeight + rightHeight) / 2
```

正确：

```text
(leftHeight + rightHeight + distance) / 2
```

距离决定中间有多少空间用于上升和下降。

### 错误 5：忘记最后一段

最后限制点到第 `n` 栋之间没有右侧限制，可以继续每步加 `1`。

必须计算：

```text
lastHeight + n - lastId
```

### 错误 6：把上限当成固定高度

`[id, maxHeight]` 表示：

```text
height[id] <= maxHeight
```

不是：

```text
height[id] == maxHeight
```

传播后它可能被进一步降低。

### 错误 7：没有排序

如果限制点乱序，直接计算距离可能得到负数或错误区间。

必须先按 `id` 排序。

### 错误 8：使用 `int` 计算所有中间值

峰值公式包含三项相加，使用 `long long` 更安全。

## 通过这道题可以学习到什么

### 1. 学会把稀疏限制传播到整个区间

题目只给出少量限制点，但这些限制会通过“相邻高度差不超过 1”影响附近所有建筑。

这是一种约束传播：

```text
一个点的限制会沿着编号轴向左右扩散。
```

### 2. 学会双向扫描

只从一个方向扫描只能得到一侧约束。

这道题需要：

```text
正向扫描处理左侧影响
反向扫描处理右侧影响
```

类似技巧常用于：

- 前缀最大值和后缀最大值
- 左右最近有效信息
- 区间约束传播
- 字符串左右匹配

### 3. 学会区分原始上限和有效上限

输入给出的：

```text
[2,5]
```

不代表第 2 栋真的能达到 `5`。

它还会受到第 1 栋高度为 `0` 的影响，最终有效上限可能只有 `1`。

这训练我们理解：

```text
局部条件必须和全局条件一起考虑。
```

### 4. 学会把连续问题压缩到关键点

`n` 可能很大，但限制点数量可能远小于 `n`。

没有必要逐栋建筑模拟。

只需要保存：

```text
第 1 栋
所有受限制建筑
```

然后用数学公式计算两个关键点之间的最高值。

这叫做：

```text
关键点压缩
```

### 5. 学会从几何图形推导公式

两个限制点之间的高度上界像两条斜率分别为 `+1` 和 `-1` 的直线。

最高峰就是两条线的交点附近：

```text
左边上升线
右边上升线
```

从图形推导出：

```text
(leftHeight + rightHeight + distance) / 2
```

这是“离散数组 + 几何直觉”的结合。

### 6. 学会贪心地把限制点取到尽可能高

为了最大化全局最高建筑，我们可以把每个有效限制点视为尽可能达到其收紧后的上限。

因为提高边界不会降低区间中能够达到的最高峰。

这背后包含贪心思想。

### 7. 学会处理边界区间

中间区间有左右两个限制点。

尾部区间只有左侧限制点。

不同边界条件需要单独分析：

```text
中间：两侧共同限制
尾部：只受左侧限制，可以持续上升
```

### 8. 学会安全处理整数

即使变量最终看起来不会太大，中间表达式仍可能溢出。

这道题能帮助你养成习惯：

```text
看到多个大整数相加时，优先考虑 long long。
```

## 还可以延伸学习什么知识

### 1. 差分约束

相邻建筑条件可以写成：

```text
height[i] - height[i - 1] <= 1
height[i - 1] - height[i] <= 1
```

这是差分约束的形式。

本题因为约束只发生在一条线性链上，所以可以用两次扫描解决，不需要一般图上的最短路算法。

### 2. Lipschitz 条件

相邻高度差不超过 `1`，也可以理解为离散版本的：

```text
函数变化速度不超过 1。
```

数学上类似 `1-Lipschitz` 条件。

这解释了为什么高度上界会形成斜率为 `+1` 或 `-1` 的直线。

### 3. 上包络与下包络

每个限制点 `(id, limit)` 都会产生一个 V 形上界：

```text
height[x] <= limit + |x - id|
```

所有限制共同作用时，真实上界是这些函数的最小值。

左右传播实际上是在高效计算这些上界的下包络。

### 4. 排序后处理相邻关系

很多题目都是：

```text
先按坐标排序
再只处理相邻点
```

例如：

- 合并区间
- 安排会议
- 扫描线
- 最近点关系
- 稀疏事件模拟

### 5. 前向传播与反向传播

本题的两次扫描可以抽象为：

```text
forward pass
backward pass
```

类似结构还出现在：

- 动态规划状态优化
- 糖果问题
- 接雨水的前后缀方法
- 字符串约束修正

### 6. 离散峰值计算

峰值公式中需要向下取整，是因为建筑位置和高度都是整数。

这涉及：

- 奇偶性
- 整数除法
- 连续交点到离散位置的转换

### 7. 可行解构造

题目只要求最大高度，但还可以进一步思考：

```text
如何构造一组真正达到这个最大值的所有建筑高度？
```

做法可以是：

- 先确定最高峰所在区间和位置
- 从峰值向左右每步最多下降 1
- 同时不超过各限制点上限

这能加深对“上界可达到”的理解。

## 学完后的自查问题

1. 为什么要主动加入限制点 `[1,0]`？
2. 为什么限制点必须先排序？
3. 为什么只从左往右传播不够？
4. `distance` 在代码中表示什么？
5. `limitFromLeft` 为什么是 `左点高度 + distance`？
6. 两个限制点之间的峰值公式是什么？
7. 为什么最后一个限制点右侧要单独计算？
8. 为什么中间计算使用 `long long`？
9. 限制 `[id,maxHeight]` 是等于关系还是小于等于关系？
10. 如果没有任何额外限制，答案为什么是 `n - 1`？

参考答案：

```text
1. 因为第 1 栋高度固定为 0，它会约束后续建筑。
2. 因为传播和峰值计算都依赖建筑编号的相邻顺序。
3. 因为右侧较低限制也会向左收紧上限。
4. 两个相邻限制点的建筑编号差。
5. 相邻建筑每一步最多升高 1，走 distance 步最多升高 distance。
6. floor((leftHeight + rightHeight + distance) / 2)。
7. 尾部没有右侧限制，可以从最后限制点持续每步升高 1。
8. 防止高度和距离相加时发生 int 溢出。
9. 小于等于关系。
10. 从第 1 栋高度 0 开始，每栋最多增加 1，第 n 栋最高为 n - 1。
```

## 总结

这道题的完整思路可以记成：

```text
加入起点
排序限制
左右收紧
区间求峰
尾部上升
```

三个核心公式：

```text
左侧传播：
limit[i] = min(limit[i], limit[i-1] + distance)

右侧传播：
limit[i] = min(limit[i], limit[i+1] + distance)

区间峰值：
peak = floor((leftHeight + rightHeight + distance) / 2)
```

复杂度：

```text
时间复杂度：O(m log m)
空间复杂度：O(m)
```

这道题真正值得掌握的知识包括：

```text
排序
双向约束传播
关键点压缩
贪心
离散几何
峰值公式推导
边界区间处理
整数溢出保护
```
