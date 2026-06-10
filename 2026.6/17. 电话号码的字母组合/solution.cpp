#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>

using namespace std;

/**
 * 17. 电话号码的字母组合
 *
 * 题目描述：
 * 给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。
 * 答案可以按任意顺序返回。
 *
 * 数字到字母的映射如下（与电话按键相同）：
 * 2 -> a, b, c
 * 3 -> d, e, f
 * 4 -> g, h, i
 * 5 -> j, k, l
 * 6 -> m, n, o
 * 7 -> p, q, r, s
 * 8 -> t, u, v
 * 9 -> w, x, y, z
 */

// ==================== 解法一：回溯法（推荐）====================
// 思路：使用递归和回溯，逐位生成所有可能的字母组合
// 时间复杂度：O(3^m × 4^n)，m 是对应 3 个字母的数字个数，n 是对应 4 个字母的数字个数
// 空间复杂度：O(m + n)，递归栈的深度
class Solution1 {
private:
    // 数字到字母的映射
    string letterMap[10] = {
        "",     // 0
        "",     // 1
        "abc",  // 2
        "def",  // 3
        "ghi",  // 4
        "jkl",  // 5
        "mno",  // 6
        "pqrs", // 7
        "tuv",  // 8
        "wxyz"  // 9
    };

    vector<string> result;  // 存储结果
    string path;            // 当前路径

    // 回溯函数
    void backtrack(string& digits, size_t index) {
        // 终止条件：处理完所有数字
        if (index == digits.size()) {
            result.push_back(path);
            return;
        }

        // 获取当前数字
        int digit = digits[index] - '0';
        string letters = letterMap[digit];

        // 遍历当前数字对应的所有字母
        for (char c : letters) {
            path.push_back(c);           // 做选择
            backtrack(digits, index + 1); // 递归
            path.pop_back();             // 撤销选择（回溯）
        }
    }

public:
    vector<string> letterCombinations(string digits) {
        // 边界条件：空字符串
        if (digits.empty()) {
            return {};
        }

        // 清空结果
        result.clear();
        path.clear();

        // 开始回溯
        backtrack(digits, 0);

        return result;
    }
};

// ==================== 解法二：迭代法 ====================
// 思路：使用循环，逐步生成所有组合
// 时间复杂度：O(3^m × 4^n)
// 空间复杂度：O(3^m × 4^n)
class Solution2 {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) {
            return {};
        }

        // 数字到字母的映射
        string letterMap[10] = {
            "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
        };

        vector<string> result = {""};

        // 对于每一位数字
        for (char digit : digits) {
            vector<string> temp;
            string letters = letterMap[digit - '0'];

            // 遍历当前结果列表
            for (string& s : result) {
                // 追加当前数字对应的所有字母
                for (char c : letters) {
                    temp.push_back(s + c);
                }
            }

            // 更新结果列表
            result = temp;
        }

        return result;
    }
};

// ==================== 解法三：BFS（广度优先搜索）====================
// 思路：使用队列进行广度优先搜索
// 时间复杂度：O(3^m × 4^n)
// 空间复杂度：O(3^m × 4^n)
class Solution3 {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) {
            return {};
        }

        // 数字到字母的映射
        string letterMap[10] = {
            "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
        };

        queue<string> q;
        q.push("");

        // 对于每一位数字
        for (char digit : digits) {
            int size = q.size();
            string letters = letterMap[digit - '0'];

            // 处理当前队列中的所有字符串
            for (int i = 0; i < size; i++) {
                string current = q.front();
                q.pop();

                // 追加当前数字对应的所有字母
                for (char c : letters) {
                    q.push(current + c);
                }
            }
        }

        // 将队列转换为向量
        vector<string> result;
        while (!q.empty()) {
            result.push_back(q.front());
            q.pop();
        }

        return result;
    }
};

// ==================== 解法四：DFS（使用显式栈）====================
// 思路：使用栈进行深度优先搜索
// 时间复杂度：O(3^m × 4^n)
// 空间复杂度：O(3^m × 4^n)
class Solution4 {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) {
            return {};
        }

        // 数字到字母的映射
        string letterMap[10] = {
            "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
        };

        vector<string> result;

        // 使用栈存储状态：(当前索引, 当前路径)
        stack<pair<int, string>> stk;
        stk.push({0, ""});

        while (!stk.empty()) {
            pair<int, string> current = stk.top();
            stk.pop();
            size_t index = current.first;
            string path = current.second;

            // 终止条件：处理完所有数字
            if (index == digits.size()) {
                result.push_back(path);
                continue;
            }

            // 获取当前数字对应的字母
            int digit = digits[index] - '0';
            string letters = letterMap[digit];

            // 将所有可能的选择压入栈
            for (char c : letters) {
                stk.push({index + 1, path + c});
            }
        }

        return result;
    }
};

// ==================== 辅助函数：打印结果 ====================
void printVector(const vector<string>& vec, const string& label = "") {
    if (!label.empty()) {
        cout << label << ": ";
    }
    cout << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << "\"" << vec[i] << "\"";
        if (i < vec.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

// ==================== 测试函数 ====================
void testSolution(Solution1& sol, const string& name) {
    cout << "========== " << name << " ==========" << endl;

    // 测试用例 1
    string digits1 = "23";
    vector<string> result1 = sol.letterCombinations(digits1);
    cout << "输入: \"" << digits1 << "\"" << endl;
    printVector(result1, "输出");
    vector<string> expected1 = {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"};
    printVector(expected1, "期望");
    cout << "测试通过: " << (result1 == expected1 ? "✓" : "✗") << endl;
    cout << endl;

    // 测试用例 2：空字符串
    string digits2 = "";
    vector<string> result2 = sol.letterCombinations(digits2);
    cout << "输入: \"" << digits2 << "\"" << endl;
    printVector(result2, "输出");
    vector<string> expected2 = {};
    printVector(expected2, "期望");
    cout << "测试通过: " << (result2 == expected2 ? "✓" : "✗") << endl;
    cout << endl;

    // 测试用例 3：单个数字
    string digits3 = "2";
    vector<string> result3 = sol.letterCombinations(digits3);
    cout << "输入: \"" << digits3 << "\"" << endl;
    printVector(result3, "输出");
    vector<string> expected3 = {"a", "b", "c"};
    printVector(expected3, "期望");
    cout << "测试通过: " << (result3 == expected3 ? "✓" : "✗") << endl;
    cout << endl;

    // 测试用例 4：包含 '7' 和 '9'
    string digits4 = "79";
    vector<string> result4 = sol.letterCombinations(digits4);
    cout << "输入: \"" << digits4 << "\"" << endl;
    printVector(result4, "输出");
    vector<string> expected4 = {
        "pw", "px", "py", "pz", "qw", "qx", "qy", "qz",
        "rw", "rx", "ry", "rz", "sw", "sx", "sy", "sz"
    };
    printVector(expected4, "期望");
    cout << "测试通过: " << (result4 == expected4 ? "✓" : "✗") << endl;
    cout << endl;

    // 测试用例 5：多位数字
    string digits5 = "234";
    vector<string> result5 = sol.letterCombinations(digits5);
    cout << "输入: \"" << digits5 << "\"" << endl;
    cout << "输出数量: " << result5.size() << endl;
    cout << "期望数量: 27" << endl;
    cout << "测试通过: " << (result5.size() == 27 ? "✓" : "✗") << endl;
    cout << endl;
}

int main() {
    cout << "17. 电话号码的字母组合" << endl;
    cout << endl;

    // 测试解法一
    Solution1 sol1;
    testSolution(sol1, "解法一：回溯法");

    // 测试解法二
    Solution2 sol2;
    cout << "========== 解法二：迭代法 ==========" << endl;
    string digits1 = "23";
    vector<string> result1 = sol2.letterCombinations(digits1);
    cout << "输入: \"" << digits1 << "\"" << endl;
    printVector(result1, "输出");
    vector<string> expected1 = {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"};
    printVector(expected1, "期望");
    cout << "测试通过: " << (result1 == expected1 ? "✓" : "✗") << endl;
    cout << endl;

    // 测试解法三
    Solution3 sol3;
    cout << "========== 解法三：BFS ==========" << endl;
    string digits2 = "2";
    vector<string> result2 = sol3.letterCombinations(digits2);
    cout << "输入: \"" << digits2 << "\"" << endl;
    printVector(result2, "输出");
    vector<string> expected2 = {"a", "b", "c"};
    printVector(expected2, "期望");
    cout << "测试通过: " << (result2 == expected2 ? "✓" : "✗") << endl;
    cout << endl;

    // 测试解法四
    Solution4 sol4;
    cout << "========== 解法四：DFS ==========" << endl;
    string digits3 = "79";
    vector<string> result3 = sol4.letterCombinations(digits3);
    cout << "输入: \"" << digits3 << "\"" << endl;
    printVector(result3, "输出");
    // 注意：DFS 使用栈，结果顺序与回溯法不同，但内容相同
    cout << "输出数量: " << result3.size() << endl;
    cout << "期望数量: 16" << endl;
    cout << "测试通过: " << (result3.size() == 16 ? "✓" : "✗") << endl;
    cout << endl;

    cout << "所有测试完成！" << endl;
    return 0;
}
