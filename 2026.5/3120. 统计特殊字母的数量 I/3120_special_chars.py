class Solution:
    def numberOfSpecialChars(self, word: str) -> int:
        """
        统计特殊字母的数量

        特殊字母定义：如果 word 中同时存在某个字母的小写形式和大写形式，
        则称这个字母为特殊字母。

        Args:
            word: 输入字符串

        Returns:
            特殊字母的数量
        """
        # 使用集合记录出现的小写字母和大写字母
        lower_set = set()
        upper_set = set()

        for char in word:
            if char.islower():
                lower_set.add(char)
            elif char.isupper():
                upper_set.add(char)

        # 统计同时存在大小写的字母
        count = 0
        for char in lower_set:
            if char.upper() in upper_set:
                count += 1

        return count

    def numberOfSpecialCharsConcise(self, word: str) -> int:
        """
        更简洁的实现方式

        Args:
            word: 输入字符串

        Returns:
            特殊字母的数量
        """
        lower = {c for c in word if c.islower()}
        upper = {c for c in word if c.isupper()}
        return len(lower & {c.lower() for c in upper})


# 测试代码
if __name__ == "__main__":
    solution = Solution()

    # 测试示例
    test_cases = [
        ("aaAbcBC", 3),      # 特殊字母: a, b, c
        ("abc", 0),           # 无特殊字母
        ("ABC", 0),           # 无特殊字母
        ("aA", 1),            # 特殊字母: a
        ("", 0),              # 空字符串
        ("aAbBcCdD", 4),      # 特殊字母: a, b, c, d
    ]

    for word, expected in test_cases:
        result = solution.numberOfSpecialChars(word)
        status = "PASS" if result == expected else "FAIL"
        print(f"输入: '{word}' -> 输出: {result} (期望: {expected}) {status}")
