# 2161. 根据给定数字划分数组

## 📋 目录

- [题目描述](#题目描述)
- [示例详解](#示例详解)
- [约束条件](#约束条件)
- [解题思路](#解题思路)
- [代码实现](#代码实现)
- [复杂度分析](#复杂度分析)
- [关键点与注意事项](#关键点与注意事项)
- [常见错误与陷阱](#常见错误与陷阱)
- [相关题目推荐](#相关题目推荐)
- [参考资料](#参考资料)

---

## 题目描述

给你一个下标从 **0** 开始的整数数组 `nums` 和一个整数 `pivot`。请你将 `nums` 重新排列，使得满足以下条件：

1. **小于** `pivot` 的元素都出现在前面
2. **等于** `pivot` 的元素都出现在中间
3. **大于** `pivot` 的元素都出现在后面

返回重新排列后的数组。

### 题目本质

这道题本质上是一个 **三路分区问题**（Three-way Partition Problem），也被称为 **荷兰国旗问题**（Dutch National Flag Problem）的变体。与经典的荷兰国旗问题不同的是，本题要求保持元素的 **相对顺序**（稳定性）。

### 核心思想

将数组中的元素按照与 `pivot` 的大小关系分为三类，并保持各类元素内部的相对顺序不变。

---

## 示例详解

### 示例 1：基本示例

```
输入：nums = [9,12,5,10,14,3,10], pivot = 10
输出：[9,5,3,10,10,12,14]
```

**详细分析：**

| 原始位置 | 元素 | 与 pivot 关系 | 最终位置 |
|---------|------|--------------|---------|
| 0       | 9    | < 10         | 0       |
| 1       | 12   | > 10         | 5       |
| 2       | 5    | < 10         | 1       |
| 3       | 10   | = 10         | 3       |
| 4       | 14   | > 10         | 6       |
| 5       | 3    | < 10         | 2       |
| 6       | 10   | = 10         | 4       |

**结果组成：**
- 小于 10 的元素：`[9, 5, 3]`（保持原顺序）
- 等于 10 的元素：`[10, 10]`（保持原顺序）
- 大于 10 的元素：`[12, 14]`（保持原顺序）
- 合并结果：`[9, 5, 3, 10, 10, 12, 14]`

### 示例 2：负数示例

```
输入：nums = [-3,4,3,2], pivot = 2
输出：[-3,2,4,3]
```

**详细分析：**

| 原始位置 | 元素 | 与 pivot 关系 | 最终位置 |
|---------|------|--------------|---------|
| 0       | -3   | < 2          | 0       |
| 1       | 4    | > 2          | 2       |
| 2       | 3    | > 2          | 3       |
| 3       | 2    | = 2          | 1       |

**结果组成：**
- 小于 2 的元素：`[-3]`
- 等于 2 的元素：`[2]`
- 大于 2 的元素：`[4, 3]`（保持原顺序）
- 合并结果：`[-3, 2, 4, 3]`

### 示例 3：边界情况 - 所有元素相等

```
输入：nums = [5,5,5,5], pivot = 5
输出：[5,5,5,5]
```

**分析：** 所有元素都等于 pivot，因此顺序不变。

### 示例 4：边界情况 - 只有一类元素

```
输入：nums = [1,2,3,4,5], pivot = 3
输出：[1,2,3,4,5]
```

**分析：** 
- 小于 3 的元素：`[1, 2]`
- 等于 3 的元素：`[3]`
- 大于 3 的元素：`[4, 5]`
- 原数组已经满足条件，无需改变顺序

### 示例 5：边界情况 - 单个元素

```
输入：nums = [1], pivot = 1
输出：[1]
```

**分析：** 数组只有一个元素，直接返回。

### 示例 6：大数示例

```
输入：nums = [1000000, -1000000, 0, 500000], pivot = 0
输出：[-1000000, 0, 1000000, 500000]
```

**分析：** 测试大数值的处理能力。

---

## 约束条件

- `1 <= nums.length <= 10^5`
- `-10^6 <= nums[i] <= 10^6`
- `pivot` 等于 `nums` 中的一个元素

### 约束分析

1. **数组长度范围**：1 到 100,000，需要 O(n) 或 O(n log n) 的算法
2. **元素值范围**：-1,000,000 到 1,000,000，可以使用整数存储
3. **pivot 保证存在**：pivot 一定是数组中的某个元素，无需处理不存在的情况

---

## 解题思路

### 方法一：三路分割（推荐）

**核心思想：** 遍历数组三次，分别收集三类元素。

**算法步骤：**
1. 创建三个空列表：`less`、`equal`、`greater`
2. 第一次遍历：收集所有小于 pivot 的元素到 `less`
3. 第二次遍历：收集所有等于 pivot 的元素到 `equal`
4. 第三次遍历：收集所有大于 pivot 的元素到 `greater`
5. 合并三个列表：`less + equal + greater`

**图解说明：**

```
原始数组: [9, 12, 5, 10, 14, 3, 10]
pivot = 10

第一次遍历（收集 < 10）:
  9 < 10  ✓ → less = [9]
  12 < 10 ✗
  5 < 10  ✓ → less = [9, 5]
  10 < 10 ✗
  14 < 10 ✗
  3 < 10  ✓ → less = [9, 5, 3]
  10 < 10 ✗

第二次遍历（收集 = 10）:
  9 = 10  ✗
  12 = 10 ✗
  5 = 10  ✗
  10 = 10 ✓ → equal = [10]
  14 = 10 ✗
  3 = 10  ✗
  10 = 10 ✓ → equal = [10, 10]

第三次遍历（收集 > 10）:
  9 > 10  ✗
  12 > 10 ✓ → greater = [12]
  5 > 10  ✗
  10 > 10 ✗
  14 > 10 ✓ → greater = [12, 14]
  3 > 10  ✗
  10 > 10 ✗

合并: [9, 5, 3] + [10, 10] + [12, 14] = [9, 5, 3, 10, 10, 12, 14]
```

**优点：**
- 代码简洁易懂
- 逻辑清晰
- 时间复杂度 O(n)

**缺点：**
- 需要三次遍历
- 空间复杂度 O(n)

---

### 方法二：单次遍历 + 列表推导

**核心思想：** 使用 Python 的列表推导式，代码更简洁。

**算法步骤：**
1. 使用列表推导式一次性创建三个列表
2. 合并并返回

**优点：**
- 代码最简洁
- Python 风格

**缺点：**
- 仍然需要三次遍历（列表推导式内部）
- 空间复杂度 O(n)

---

### 方法三：统计计数法

**核心思想：** 先统计各类元素的数量，再按位置填充结果数组。

**算法步骤：**
1. 第一次遍历：统计小于、等于、大于 pivot 的元素数量
2. 计算各类元素的起始位置
3. 第二次遍历：将元素放到正确的位置

**图解说明：**

```
原始数组: [9, 12, 5, 10, 14, 3, 10]
pivot = 10

第一次遍历（统计数量）:
  less_count = 3 (9, 5, 3)
  equal_count = 2 (10, 10)
  greater_count = 2 (12, 14)

计算起始位置:
  less_start = 0
  equal_start = 3
  greater_start = 5

第二次遍历（填充结果）:
  result = [0, 0, 0, 0, 0, 0, 0]
  
  9 < 10  → result[0] = 9,   less_start = 1
  12 > 10 → result[5] = 12,  greater_start = 6
  5 < 10  → result[1] = 5,   less_start = 2
  10 = 10 → result[3] = 10,  equal_start = 4
  14 > 10 → result[6] = 14,  greater_start = 7
  3 < 10  → result[2] = 3,   less_start = 3
  10 = 10 → result[4] = 10,  equal_start = 5

结果: [9, 5, 3, 10, 10, 12, 14]
```

**优点：**
- 只需要两次遍历
- 结果数组预先分配，减少内存分配开销

**缺点：**
- 代码稍微复杂
- 空间复杂度 O(n)

---

### 方法四：双指针法

**核心思想：** 使用两个指针分别从前往后和从后往前遍历。

**算法步骤：**
1. 创建结果数组
2. 第一次遍历：从前往后，放置小于 pivot 的元素
3. 第二次遍历：从前往后，放置等于 pivot 的元素
4. 第三次遍历：从前往后，放置大于 pivot 的元素

**优点：**
- 逻辑清晰
- 代码结构规整

**缺点：**
- 需要三次遍历
- 空间复杂度 O(n)

---

### 方法五：原地分区（进阶）

**核心思想：** 使用类似快速排序的分区思想，但保持相对顺序（稳定性）。

**注意：** 经典的快速排序分区是不稳定的，需要特殊处理才能保持稳定性。

**算法思路：**
1. 使用三个指针：`less_end`、`equal_end`、`greater_start`
2. 遍历数组，将元素交换到正确的位置
3. 需要多次遍历才能保持稳定性

**优点：**
- 空间复杂度 O(1)（如果不考虑输出数组）
- 原地操作

**缺点：**
- 代码复杂
- 难以保持稳定性
- 实际上由于需要保持稳定性，很难做到真正的 O(1) 空间

---

## 代码实现

### Python 实现

#### 解法一：三路分割（推荐）

```python
from typing import List

class Solution:
    def pivotArray(self, nums: List[int], pivot: int) -> List[int]:
        """
        三路分割法
        
        思路：遍历数组三次，分别收集三类元素
        时间复杂度：O(n)
        空间复杂度：O(n)
        """
        less = []      # 存储小于 pivot 的元素
        equal = []     # 存储等于 pivot 的元素
        greater = []   # 存储大于 pivot 的元素
        
        for num in nums:
            if num < pivot:
                less.append(num)
            elif num == pivot:
                equal.append(num)
            else:
                greater.append(num)
        
        return less + equal + greater
```

#### 解法二：列表推导式

```python
class Solution:
    def pivotArray(self, nums: List[int], pivot: int) -> List[int]:
        """
        列表推导式法
        
        思路：使用列表推导式，代码更简洁
        时间复杂度：O(n)
        空间复杂度：O(n)
        """
        less = [x for x in nums if x < pivot]
        equal = [x for x in nums if x == pivot]
        greater = [x for x in nums if x > pivot]
        return less + equal + greater
```

#### 解法三：统计计数法

```python
class Solution:
    def pivotArray(self, nums: List[int], pivot: int) -> List[int]:
        """
        统计计数法
        
        思路：先统计各类元素数量，再按位置填充
        时间复杂度：O(n)
        空间复杂度：O(n)
        """
        n = len(nums)
        result = [0] * n
        
        # 统计各类元素的数量
        less_count = 0
        equal_count = 0
        greater_count = 0
        
        for num in nums:
            if num < pivot:
                less_count += 1
            elif num == pivot:
                equal_count += 1
            else:
                greater_count += 1
        
        # 计算各类元素的起始位置
        less_start = 0
        equal_start = less_count
        greater_start = less_count + equal_count
        
        # 填充结果数组
        for num in nums:
            if num < pivot:
                result[less_start] = num
                less_start += 1
            elif num == pivot:
                result[equal_start] = num
                equal_start += 1
            else:
                result[greater_start] = num
                greater_start += 1
        
        return result
```

#### 解法四：双指针法

```python
class Solution:
    def pivotArray(self, nums: List[int], pivot: int) -> List[int]:
        """
        双指针法
        
        思路：使用两个指针分别处理不同类别的元素
        时间复杂度：O(n)
        空间复杂度：O(n)
        """
        n = len(nums)
        result = [0] * n
        
        # 第一遍：放置小于 pivot 的元素
        left = 0
        for num in nums:
            if num < pivot:
                result[left] = num
                left += 1
        
        # 第二遍：放置等于 pivot 的元素
        mid = left
        for num in nums:
            if num == pivot:
                result[mid] = num
                mid += 1
        
        # 第三遍：放置大于 pivot 的元素
        right = mid
        for num in nums:
            if num > pivot:
                result[right] = num
                right += 1
        
        return result
```

---

### Java 实现

#### 解法一：三路分割

```java
class Solution {
    public int[] pivotArray(int[] nums, int pivot) {
        int n = nums.length;
        List<Integer> less = new ArrayList<>();
        List<Integer> equal = new ArrayList<>();
        List<Integer> greater = new ArrayList<>();
        
        for (int num : nums) {
            if (num < pivot) {
                less.add(num);
            } else if (num == pivot) {
                equal.add(num);
            } else {
                greater.add(num);
            }
        }
        
        int[] result = new int[n];
        int index = 0;
        
        for (int num : less) {
            result[index++] = num;
        }
        for (int num : equal) {
            result[index++] = num;
        }
        for (int num : greater) {
            result[index++] = num;
        }
        
        return result;
    }
}
```

#### 解法二：统计计数法

```java
class Solution {
    public int[] pivotArray(int[] nums, int pivot) {
        int n = nums.length;
        int[] result = new int[n];
        
        // 统计各类元素的数量
        int lessCount = 0;
        int equalCount = 0;
        int greaterCount = 0;
        
        for (int num : nums) {
            if (num < pivot) {
                lessCount++;
            } else if (num == pivot) {
                equalCount++;
            } else {
                greaterCount++;
            }
        }
        
        // 计算各类元素的起始位置
        int lessStart = 0;
        int equalStart = lessCount;
        int greaterStart = lessCount + equalCount;
        
        // 填充结果数组
        for (int num : nums) {
            if (num < pivot) {
                result[lessStart++] = num;
            } else if (num == pivot) {
                result[equalStart++] = num;
            } else {
                result[greaterStart++] = num;
            }
        }
        
        return result;
    }
}
```

---

### C++ 实现

#### 解法一：三路分割

```cpp
class Solution {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        vector<int> less, equal, greater;
        
        for (int num : nums) {
            if (num < pivot) {
                less.push_back(num);
            } else if (num == pivot) {
                equal.push_back(num);
            } else {
                greater.push_back(num);
            }
        }
        
        vector<int> result;
        result.insert(result.end(), less.begin(), less.end());
        result.insert(result.end(), equal.begin(), equal.end());
        result.insert(result.end(), greater.begin(), greater.end());
        
        return result;
    }
};
```

#### 解法二：统计计数法

```cpp
class Solution {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        int n = nums.size();
        vector<int> result(n);
        
        // 统计各类元素的数量
        int lessCount = 0, equalCount = 0, greaterCount = 0;
        for (int num : nums) {
            if (num < pivot) lessCount++;
            else if (num == pivot) equalCount++;
            else greaterCount++;
        }
        
        // 计算各类元素的起始位置
        int lessStart = 0;
        int equalStart = lessCount;
        int greaterStart = lessCount + equalCount;
        
        // 填充结果数组
        for (int num : nums) {
            if (num < pivot) {
                result[lessStart++] = num;
            } else if (num == pivot) {
                result[equalStart++] = num;
            } else {
                result[greaterStart++] = num;
            }
        }
        
        return result;
    }
};
```

---

### JavaScript 实现

#### 解法一：三路分割

```javascript
/**
 * @param {number[]} nums
 * @param {number} pivot
 * @return {number[]}
 */
var pivotArray = function(nums, pivot) {
    const less = [];
    const equal = [];
    const greater = [];
    
    for (const num of nums) {
        if (num < pivot) {
            less.push(num);
        } else if (num === pivot) {
            equal.push(num);
        } else {
            greater.push(num);
        }
    }
    
    return [...less, ...equal, ...greater];
};
```

#### 解法二：统计计数法

```javascript
var pivotArray = function(nums, pivot) {
    const n = nums.length;
    const result = new Array(n);
    
    // 统计各类元素的数量
    let lessCount = 0, equalCount = 0, greaterCount = 0;
    for (const num of nums) {
        if (num < pivot) lessCount++;
        else if (num === pivot) equalCount++;
        else greaterCount++;
    }
    
    // 计算各类元素的起始位置
    let lessStart = 0;
    let equalStart = lessCount;
    let greaterStart = lessCount + equalCount;
    
    // 填充结果数组
    for (const num of nums) {
        if (num < pivot) {
            result[lessStart++] = num;
        } else if (num === pivot) {
            result[equalStart++] = num;
        } else {
            result[greaterStart++] = num;
        }
    }
    
    return result;
};
```

---

### Go 实现

#### 解法一：三路分割

```go
func pivotArray(nums []int, pivot int) []int {
    less := []int{}
    equal := []int{}
    greater := []int{}
    
    for _, num := range nums {
        if num < pivot {
            less = append(less, num)
        } else if num == pivot {
            equal = append(equal, num)
        } else {
            greater = append(greater, num)
        }
    }
    
    result := make([]int, 0, len(nums))
    result = append(result, less...)
    result = append(result, equal...)
    result = append(result, greater...)
    
    return result
}
```

#### 解法二：统计计数法

```go
func pivotArray(nums []int, pivot int) []int {
    n := len(nums)
    result := make([]int, n)
    
    // 统计各类元素的数量
    lessCount, equalCount, greaterCount := 0, 0, 0
    for _, num := range nums {
        if num < pivot {
            lessCount++
        } else if num == pivot {
            equalCount++
        } else {
            greaterCount++
        }
    }
    
    // 计算各类元素的起始位置
    lessStart := 0
    equalStart := lessCount
    greaterStart := lessCount + equalCount
    
    // 填充结果数组
    for _, num := range nums {
        if num < pivot {
            result[lessStart] = num
            lessStart++
        } else if num == pivot {
            result[equalStart] = num
            equalStart++
        } else {
            result[greaterStart] = num
            greaterStart++
        }
    }
    
    return result
}
```

---

## 复杂度分析

### 时间复杂度

| 方法 | 时间复杂度 | 说明 |
|------|-----------|------|
| 三路分割 | O(n) | 遍历数组一次（或三次，每次 O(n)） |
| 列表推导式 | O(n) | 内部仍然是 O(n) |
| 统计计数法 | O(n) | 遍历数组两次 |
| 双指针法 | O(n) | 遍历数组三次 |

**结论：** 所有方法的时间复杂度都是 O(n)，其中 n 是数组长度。

### 空间复杂度

| 方法 | 空间复杂度 | 说明 |
|------|-----------|------|
| 三路分割 | O(n) | 需要额外空间存储三个列表 |
| 列表推导式 | O(n) | 需要额外空间存储三个列表 |
| 统计计数法 | O(n) | 需要结果数组 |
| 双指针法 | O(n) | 需要结果数组 |

**结论：** 所有方法的空间复杂度都是 O(n)，因为需要返回新数组。

### 复杂度总结

| 方法 | 时间复杂度 | 空间复杂度 | 优点 | 缺点 |
|------|-----------|-----------|------|------|
| 三路分割 | O(n) | O(n) | 代码简洁，逻辑清晰 | 三次遍历 |
| 列表推导式 | O(n) | O(n) | 代码最简洁 | Python 特有 |
| 统计计数法 | O(n) | O(n) | 两次遍历，效率较高 | 代码稍复杂 |
| 双指针法 | O(n) | O(n) | 逻辑清晰 | 三次遍历 |

---

## 关键点与注意事项

### 1. 保持相对顺序

本题要求保持元素的 **相对顺序**，即：
- 小于 pivot 的元素之间保持原有顺序
- 等于 pivot 的元素之间保持原有顺序
- 大于 pivot 的元素之间保持原有顺序

**错误示例：**
```python
# 错误：使用快速排序的分区方法（不稳定）
def partition(nums, pivot):
    # 这种方法会打乱元素的相对顺序
    pass
```

**正确示例：**
```python
# 正确：使用稳定的分区方法
def pivotArray(nums, pivot):
    less = [x for x in nums if x < pivot]
    equal = [x for x in nums if x == pivot]
    greater = [x for x in nums if x > pivot]
    return less + equal + greater
```

### 2. 处理边界情况

需要考虑以下边界情况：

1. **空数组**：虽然题目保证 `nums.length >= 1`，但实际代码中最好处理
2. **单个元素**：直接返回
3. **所有元素相等**：返回原数组
4. **pivot 为最小值**：所有元素都大于等于 pivot
5. **pivot 为最大值**：所有元素都小于等于 pivot
6. **负数**：正确处理负数比较
7. **大数**：正确处理大数值

### 3. 选择合适的方法

- **Python**：推荐使用列表推导式，代码最简洁
- **Java/C++/Go**：推荐使用统计计数法，效率较高
- **JavaScript**：推荐使用三路分割，代码清晰

### 4. 内存优化

如果需要优化内存使用，可以考虑：
- 使用原地分区算法（但可能牺牲稳定性）
- 使用迭代器而不是列表（在支持的语言中）

---

## 常见错误与陷阱

### 错误 1：打乱相对顺序

```python
# 错误：使用快速排序的分区方法
def pivotArray(nums, pivot):
    left, right = 0, len(nums) - 1
    i = 0
    while i <= right:
        if nums[i] < pivot:
            nums[left], nums[i] = nums[i], nums[left]
            left += 1
            i += 1
        elif nums[i] > pivot:
            nums[right], nums[i] = nums[i], nums[right]
            right -= 1
        else:
            i += 1
    return nums
```

**问题：** 这种方法会打乱元素的相对顺序。

**正确做法：** 使用稳定的分区方法，如三路分割。

### 错误 2：错误的比较逻辑

```python
# 错误：使用 >= 或 <=
def pivotArray(nums, pivot):
    less = [x for x in nums if x <= pivot]  # 错误
    equal = [x for x in nums if x == pivot]
    greater = [x for x in nums if x >= pivot]  # 错误
    return less + equal + greater
```

**问题：** 使用 `<=` 或 `>=` 会导致元素重复。

**正确做法：** 使用 `<`、`==`、`>` 进行精确比较。

### 错误 3：忘记处理负数

```python
# 错误：假设所有元素都是正数
def pivotArray(nums, pivot):
    # 这种方法在处理负数时可能出错
    pass
```

**问题：** 负数的比较逻辑与正数相同，但需要确保正确处理。

**正确做法：** 使用标准的比较操作符，Python 会自动处理负数。

### 错误 4：空间复杂度过高

```python
# 错误：创建过多的中间数组
def pivotArray(nums, pivot):
    less = []
    equal = []
    greater = []
    
    for num in nums:
        if num < pivot:
            less.append(num)
    
    for num in nums:
        if num == pivot:
            equal.append(num)
    
    for num in nums:
        if num > pivot:
            greater.append(num)
    
    return less + equal + greater
```

**问题：** 遍历数组三次，效率较低。

**正确做法：** 使用单次遍历收集所有元素。

### 错误 5：忽略 pivot 不存在的情况

```python
# 错误：假设 pivot 一定存在
def pivotArray(nums, pivot):
    # 如果 pivot 不存在，这种方法仍然会返回结果
    # 但可能不符合题目要求
    pass
```

**问题：** 题目保证 pivot 存在，但实际代码中最好处理这种情况。

**正确做法：** 题目已经保证 pivot 存在，无需额外处理。

---

## 相关题目推荐

### 基础题目

1. **75. 颜色分类**（荷兰国旗问题）
   - 难度：中等
   - 链接：https://leetcode.cn/problems/sort-colors/
   - 关联：经典的三路分区问题

2. **283. 移动零**
   - 难度：简单
   - 链接：https://leetcode.cn/problems/move-zeroes/
   - 关联：双指针分区思想

### 进阶题目

3. **215. 数组中的第K个最大元素**
   - 难度：中等
   - 链接：https://leetcode.cn/problems/kth-largest-element-in-an-array/
   - 关联：快速选择算法

4. **324. 摆动排序 II**
   - 难度：中等
   - 链接：https://leetcode.cn/problems/wiggle-sort-ii/
   - 关联：分区思想

5. **347. 前 K 个高频元素**
   - 难度：中等
   - 链接：https://leetcode.cn/problems/top-k-frequent-elements/
   - 关联：桶排序思想

### 相关算法

6. **快速排序**
   - 链接：https://en.wikipedia.org/wiki/Quicksort
   - 关联：分区思想的基础

7. **荷兰国旗问题**
   - 链接：https://en.wikipedia.org/wiki/Dutch_national_flag_problem
   - 关联：三路分区的经典问题

---

## 参考资料

### 官方资源

- [LeetCode 2161 题目页面](https://leetcode.cn/problems/partition-array-according-to-given-pivot/)
- [LeetCode 官方题解](https://leetcode.cn/problems/partition-array-according-to-given-pivot/solution/)

### 算法资料

- [快速排序算法详解](https://en.wikipedia.org/wiki/Quicksort)
- [荷兰国旗问题](https://en.wikipedia.org/wiki/Dutch_national_flag_problem)
- [稳定排序与不稳定排序](https://en.wikipedia.org/wiki/Sorting_algorithm#Stability)

### 编程语言文档

- [Python 列表推导式](https://docs.python.org/3/tutorial/datastructures.html#list-comprehensions)
- [Java ArrayList](https://docs.oracle.com/javase/8/docs/api/java/util/ArrayList.html)
- [C++ vector](https://en.cppreference.com/w/cpp/container/vector)
- [JavaScript Array](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array)

---

## 更新日志

- **2026-06-08**：初始版本，包含题目描述、多种解法、复杂度分析等

---

## 作者备注

这道题是荷兰国旗问题的变体，主要考察：
1. 对数组分区的理解
2. 保持稳定性的意识
3. 边界情况的处理能力

建议先理解基本的三路分割方法，再尝试优化和改进。

---

**最后更新：2026-06-08**
