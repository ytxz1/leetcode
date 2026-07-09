from typing import List


class Solution:
    def sumAndMultiply(self, s: str, queries: List[List[int]]) -> List[int]:
        MOD = 10 ** 9 + 7
        n = len(s)

        # non_zero_count_prefix[i] 表示 s[0..i-1] 中非零数字的个数。
        # 有了它，就能把原串 s 的区间 [l, r] 映射到压缩串 t 的区间。
        non_zero_count_prefix = [0] * (n + 1)

        # digit_sum_prefix[i] 表示 s[0..i-1] 的数字和。
        # 0 对数字和没有贡献，所以它也等于这些位置中非零数字的数字和。
        digit_sum_prefix = [0] * (n + 1)

        # t_digits 存放 s 中所有非零数字，保持原来的出现顺序。
        # 例如 s = "10203004"，则 t_digits = [1, 2, 3, 4]。
        t_digits = []

        for i, ch in enumerate(s):
            digit = ord(ch) - ord("0")
            non_zero_count_prefix[i + 1] = non_zero_count_prefix[i]
            digit_sum_prefix[i + 1] = digit_sum_prefix[i] + digit

            if digit != 0:
                non_zero_count_prefix[i + 1] += 1
                t_digits.append(digit)

        k = len(t_digits)

        # pow10[len] = 10^len % MOD。
        # 从一个十进制前缀中扣掉左边部分时，需要乘 10^len 来对齐位数。
        pow10 = [1] * (k + 1)
        for length in range(1, k + 1):
            pow10[length] = pow10[length - 1] * 10 % MOD

        # value_prefix[i] 表示 t_digits[0..i-1] 连接成的整数，对 MOD 取余。
        # 例如 t_digits = [1, 2, 3, 4]：
        # value_prefix = [0, 1, 12, 123, 1234]。
        value_prefix = [0] * (k + 1)
        for i, digit in enumerate(t_digits):
            value_prefix[i + 1] = (value_prefix[i] * 10 + digit) % MOD

        answer = []

        for l, r in queries:
            # s[l..r] 之前已经出现了 left 个非零数字。
            # 所以 s[l..r] 中第一个非零数字，在 t_digits 中的下标就是 left。
            left = non_zero_count_prefix[l]

            # s[0..r] 一共出现了 right 个非零数字。
            # 因此 s[l..r] 对应 t_digits 的半开区间 [left, right)。
            right = non_zero_count_prefix[r + 1]

            # non_zero_len 是本次查询子串中非零数字的个数，
            # 也就是连接出的整数 x 的十进制位数；如果为 0，则 x = 0。
            non_zero_len = right - left

            # digit_sum 是 x 的数字和。
            # 因为 x 由 s[l..r] 的非零数字组成，所以可用原串数字和前缀直接相减。
            digit_sum = digit_sum_prefix[r + 1] - digit_sum_prefix[l]

            # 计算 x：
            # value_prefix[right] 是 t_digits[0..right-1] 形成的数；
            # value_prefix[left] 是 t_digits[0..left-1] 形成的数。
            # 把左边前缀乘上 10^non_zero_len 后扣掉，就剩 t_digits[left..right-1]。
            x = (value_prefix[right] - value_prefix[left] * pow10[non_zero_len]) % MOD

            # 题目要求返回 x * digit_sum，并对 MOD 取余。
            answer.append(x * digit_sum % MOD)

        return answer
