#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;

/*
 * 3093. 最长公共后缀查询
 *
 * 题目：给定 wordsContainer 和 wordsQuery 两个字符串数组，
 *       对于每个 wordsQuery[i]，找到 wordsContainer 中与它有最长公共后缀的字符串。
 *       如果多个字符串有相同的最长公共后缀，选择长度最短的；如果还相同，选择索引最小的。
 *
 * 公共后缀：两个字符串从末尾开始连续相同的字符序列。
 * 例如："abcdefgh" 和 "acbfgh" 的公共后缀是 "gh"（从末尾比较：h=h, g=g, f≠e 停止）
 */

// ============================================================================
// 方法一：暴力法
// ============================================================================
// 思路：对于每个查询，遍历所有单词，计算公共后缀长度，选择最优解
// 时间复杂度: O(Q * C * L)，Q=查询数, C=单词数, L=平均长度
// 空间复杂度: O(1)
class Solution1 {
public:
    vector<int> stringIndices(vector<string>& wordsContainer, vector<string>& wordsQuery) {
        vector<int> ans;
        for (const string& query : wordsQuery) {
            int bestIdx = 0;                        // 当前最优单词的索引
            int bestSuffixLen = -1;                  // 当前最长公共后缀长度
            int bestWordLen = wordsContainer[0].size(); // 当前最优单词的长度

            for (int i = 0; i < wordsContainer.size(); i++) {
                const string& word = wordsContainer[i];
                int suffixLen = getCommonSuffixLength(query, word);

                // 按优先级更新最优解：
                // 1. 公共后缀更长
                // 2. 公共后缀相同，单词更短
                // 3. 公共后缀和长度都相同，索引更小
                if (suffixLen > bestSuffixLen ||
                    (suffixLen == bestSuffixLen && word.size() < bestWordLen) ||
                    (suffixLen == bestSuffixLen && word.size() == bestWordLen && i < bestIdx)) {
                    bestIdx = i;
                    bestSuffixLen = suffixLen;
                    bestWordLen = word.size();
                }
            }
            ans.push_back(bestIdx);
        }
        return ans;
    }

private:
    // 计算两个字符串的公共后缀长度
    // 从两个字符串的末尾开始逐字符比较，直到字符不同或到达某个字符串的开头
    int getCommonSuffixLength(const string& s1, const string& s2) {
        int i = s1.size() - 1;  // s1 的末尾指针
        int j = s2.size() - 1;  // s2 的末尾指针
        int count = 0;           // 公共后缀长度
        while (i >= 0 && j >= 0 && s1[i] == s2[j]) {
            count++;
            i--;  // 向前移动
            j--;
        }
        return count;
    }
};

// ============================================================================
// 方法二：字典树（Trie）
// ============================================================================
// 核心思想：将字符串反转后插入字典树，后缀问题就变成了前缀问题
//
// 例如：查询 "acbfgh" 与 "abcdefgh" 的公共后缀
//   - 原字符串比较：从末尾开始 "hgf..." vs "hgf..." → 公共后缀 "gh"
//   - 反转后比较：  从开头开始 "hgefbca" vs "hgfedcba" → 公共前缀 "hg"
//   - 结果相同！反转后可以用字典树高效处理
//
// 时间复杂度: O(C * L + Q * L)，C=单词数, Q=查询数, L=平均长度
// 空间复杂度: O(C * L)，字典树存储所有反转后的单词
class Solution2 {
private:
    // 字典树节点结构体
    struct TrieNode {
        TrieNode* children[26];  // 26个小写字母的子节点指针
        int bestIdx;  // 经过该节点的所有字符串中，长度最短的字符串在 wordsContainer 中的索引
        int bestLen;  // 该最短字符串的长度

        // 构造函数：初始化为无效值
        // bestIdx = -1 表示还没有字符串经过该节点
        // bestLen = 1000001 表示初始长度设为一个很大的值
        TrieNode() : bestIdx(-1), bestLen(1000001) {
            for (int i = 0; i < 26; i++) {
                children[i] = nullptr;
            }
        }
    };

    // 插入一个单词到字典树
    // 参数：root=根节点, word=要插入的单词, idx=单词在 wordsContainer 中的索引
    //
    // 插入过程（以 "ghghgh" 为例）：
    //   1. 先更新根节点的最优解（根节点代表"空前缀"，即公共后缀长度为0的情况）
    //   2. 从后往前遍历单词的字符（相当于遍历反转后的字符串）
    //   3. 对于每个字符，创建或移动到对应的子节点
    //   4. 在路径上的每个节点，更新 bestIdx 和 bestLen
    //
    // 例如插入 "ghghgh"（索引2，长度6）：
    //   反转后为 "hghghg"
    //   路径：root → 'h' → 'g' → 'h' → 'g' → 'h' → 'g'
    //   每个节点的 bestIdx 更新为 2，bestLen 更新为 6（如果之前更大）
    void insert(TrieNode* root, const string& word, int idx) {
        TrieNode* node = root;

        // 步骤1：更新根节点（代表公共后缀长度为0的情况）
        // 根节点存储全局最短单词，用于当查询没有任何匹配时的默认返回值
        if (word.size() < root->bestLen ||
            (word.size() == root->bestLen && idx < root->bestIdx)) {
            root->bestIdx = idx;
            root->bestLen = word.size();
        }

        // 步骤2：从后往前插入字符（相当于插入反转字符串）
        // 例如 "ghghgh"，从最后一个字符 'h' 开始，依次插入 'h','g','h','g','h','g'
        for (int i = word.size() - 1; i >= 0; i--) {
            int c = word[i] - 'a';  // 将字符转换为 0-25 的索引

            // 如果对应子节点不存在，创建新节点
            if (!node->children[c]) {
                node->children[c] = new TrieNode();
            }

            // 移动到子节点
            node = node->children[c];

            // 步骤3：更新当前节点的最优解
            // 如果当前单词比该节点记录的单词更短，或者长度相同但索引更小，则更新
            if (word.size() < node->bestLen ||
                (word.size() == node->bestLen && idx < node->bestIdx)) {
                node->bestIdx = idx;
                node->bestLen = word.size();
            }
        }
    }

    // 在字典树中查询与给定单词有最长公共后缀的单词索引
    // 参数：root=根节点, word=查询单词
    // 返回：最优匹配的单词在 wordsContainer 中的索引
    //
    // 查询过程（以查询 "acbfgh" 为例，反转后为 "hgefbca"）：
    //   从根节点开始，逐字符匹配：
    //   - 匹配 'h'：depth=1，检查该节点的最优解
    //   - 匹配 'g'：depth=2，检查该节点的最优解
    //   - 匹配 'e'：depth=3，检查该节点的最优解（此时可能找到 "abcdefgh"）
    //   - 匹配 'f'：depth=4，继续...
    //   - 直到无法匹配或遍历完查询字符串
    //
    // 在每个匹配的节点，都要检查是否需要更新全局最优解：
    //   1. 深度更大（公共后缀更长）→ 更新
    //   2. 深度相同，单词更短 → 更新
    //   3. 深度和长度都相同，索引更小 → 更新
    int queryTrie(TrieNode* root, const string& word) {
        TrieNode* node = root;

        // 初始化全局最优解为根节点的值（即公共后缀长度为0时的最优解）
        int bestIdx = root->bestIdx;   // 最优单词的索引
        int bestLen = root->bestLen;   // 最优单词的长度
        int bestDepth = 0;             // 最优匹配的深度（即公共后缀长度）
        int depth = 0;                 // 当前匹配深度

        // 从后往前遍历查询单词（相当于遍历反转后的字符串）
        for (int i = word.size() - 1; i >= 0; i--) {
            int c = word[i] - 'a';

            // 如果当前字符没有对应的子节点，说明无法继续匹配，退出
            if (!node->children[c]) {
                break;
            }

            // 移动到子节点，深度+1
            node = node->children[c];
            depth++;

            // 在每个匹配的节点检查是否需要更新最优解
            // 优先级：深度（后缀长度）> 单词长度 > 索引
            if (depth > bestDepth ||
                (depth == bestDepth && node->bestLen < bestLen) ||
                (depth == bestDepth && node->bestLen == bestLen && node->bestIdx < bestIdx)) {
                bestIdx = node->bestIdx;
                bestLen = node->bestLen;
                bestDepth = depth;
            }
        }

        return bestIdx;
    }

    // 递归释放字典树的所有节点，防止内存泄漏
    void freeTrie(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; i++) {
            freeTrie(node->children[i]);
        }
        delete node;
    }

public:
    vector<int> stringIndices(vector<string>& wordsContainer, vector<string>& wordsQuery) {
        // 步骤1：创建根节点并初始化
        // 根节点的 bestIdx 和 bestLen 存储全局最短单词的信息
        // 用于当查询没有任何后缀匹配时返回默认值
        TrieNode* root = new TrieNode();
        root->bestIdx = 0;
        root->bestLen = wordsContainer[0].size();

        // 步骤2：将 wordsContainer 中所有单词插入字典树
        // 每个单词从后往前插入（相当于插入反转字符串）
        // 插入过程中，路径上的每个节点都会更新 bestIdx 和 bestLen
        for (int i = 0; i < wordsContainer.size(); i++) {
            insert(root, wordsContainer[i], i);
        }

        // 步骤3：对于每个查询，在字典树中查找最优匹配
        vector<int> ans;
        for (const string& q : wordsQuery) {
            ans.push_back(queryTrie(root, q));
        }

        // 步骤4：释放字典树内存
        freeTrie(root);
        return ans;
    }
};

// ============================================================================
// 方法三：优化暴力法（提前终止）
// ============================================================================
// 思路：在计算公共后缀时，如果已经达到理论最大值就提前返回
// 时间复杂度: O(Q * C * L)
// 空间复杂度: O(1)
class Solution3 {
public:
    vector<int> stringIndices(vector<string>& wordsContainer, vector<string>& wordsQuery) {
        vector<int> ans;
        for (const string& query : wordsQuery) {
            int bestIdx = 0;
            int bestSuffixLen = -1;
            int bestWordLen = wordsContainer[0].size();

            for (int i = 0; i < wordsContainer.size(); i++) {
                const string& word = wordsContainer[i];

                // 计算公共后缀长度（带提前终止优化）
                int suffixLen = getCommonSuffixLength(query, word);

                // 更新最优解（与方法一相同的逻辑）
                if (suffixLen > bestSuffixLen ||
                    (suffixLen == bestSuffixLen && word.size() < bestWordLen) ||
                    (suffixLen == bestSuffixLen && word.size() == bestWordLen && i < bestIdx)) {
                    bestIdx = i;
                    bestSuffixLen = suffixLen;
                    bestWordLen = word.size();
                }
            }
            ans.push_back(bestIdx);
        }
        return ans;
    }

private:
    // 计算公共后缀长度（带提前终止优化）
    // 当匹配长度达到两个字符串中较短的那个长度时，不可能更长了，直接返回
    int getCommonSuffixLength(const string& s1, const string& s2) {
        int i = s1.size() - 1;
        int j = s2.size() - 1;
        int count = 0;
        int maxPossible = min(s1.size(), s2.size());  // 理论最大公共后缀长度

        while (i >= 0 && j >= 0 && s1[i] == s2[j]) {
            count++;
            // 如果已经达到理论最大值，直接返回
            if (count == maxPossible) {
                return count;
            }
            i--;
            j--;
        }
        return count;
    }
};

// ============================================================================
// 主函数：测试三种方法
// ============================================================================
int main() {
    Solution1 sol1;
    Solution2 sol2;
    Solution3 sol3;

    // 测试用例1
    // wordsContainer = ["abcd", "bcd", "xbcd"]
    // wordsQuery = ["cd", "bcd", "xyz"]
    // 预期输出: [1, 1, 1]
    // 解释：
    //   "cd" 与三个单词的公共后缀都是 "cd"(长度2)，选最短的 "bcd"(索引1)
    //   "bcd" 与三个单词的公共后缀都是 "bcd"(长度3)，选最短的 "bcd"(索引1)
    //   "xyz" 与三个单词都没有公共后缀(长度0)，选最短的 "bcd"(索引1)
    vector<string> container1 = {"abcd", "bcd", "xbcd"};
    vector<string> query1 = {"cd", "bcd", "xyz"};
    cout << "测试用例1:" << endl;
    cout << "Container: [abcd, bcd, xbcd]" << endl;
    cout << "Query: [cd, bcd, xyz]" << endl;
    auto res1 = sol1.stringIndices(container1, query1);
    auto res2 = sol2.stringIndices(container1, query1);
    auto res3 = sol3.stringIndices(container1, query1);
    cout << "方法一: [";
    for (int i = 0; i < res1.size(); i++) cout << res1[i] << (i < res1.size()-1 ? ", " : "");
    cout << "]" << endl;
    cout << "方法二: [";
    for (int i = 0; i < res2.size(); i++) cout << res2[i] << (i < res2.size()-1 ? ", " : "");
    cout << "]" << endl;
    cout << "方法三: [";
    for (int i = 0; i < res3.size(); i++) cout << res3[i] << (i < res3.size()-1 ? ", " : "");
    cout << "]" << endl;
    cout << endl;

    // 测试用例2
    // wordsContainer = ["abcdefgh", "poiuygh", "ghghgh"]
    // wordsQuery = ["gh", "acbfgh", "acbfegh"]
    // 预期输出: [2, 0, 2]
    // 解释：
    //   "gh" 与三个单词的公共后缀都是 "gh"(长度2)，选最短的 "ghghgh"(索引2)
    //   "acbfgh" 与 "abcdefgh" 的公共后缀是 "fgh"(长度3)，最长，选索引0
    //   "acbfegh" 与三个单词的公共后缀都是 "gh"(长度2)，选最短的 "ghghgh"(索引2)
    vector<string> container2 = {"abcdefgh", "poiuygh", "ghghgh"};
    vector<string> query2 = {"gh", "acbfgh", "acbfegh"};
    cout << "测试用例2:" << endl;
    cout << "Container: [abcdefgh, poiuygh, ghghgh]" << endl;
    cout << "Query: [gh, acbfgh, acbfegh]" << endl;
    res1 = sol1.stringIndices(container2, query2);
    res2 = sol2.stringIndices(container2, query2);
    res3 = sol3.stringIndices(container2, query2);
    cout << "方法一: [";
    for (int i = 0; i < res1.size(); i++) cout << res1[i] << (i < res1.size()-1 ? ", " : "");
    cout << "]" << endl;
    cout << "方法二: [";
    for (int i = 0; i < res2.size(); i++) cout << res2[i] << (i < res2.size()-1 ? ", " : "");
    cout << "]" << endl;
    cout << "方法三: [";
    for (int i = 0; i < res3.size(); i++) cout << res3[i] << (i < res3.size()-1 ? ", " : "");
    cout << "]" << endl;

    return 0;
}
