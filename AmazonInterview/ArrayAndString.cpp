#include "LeetCodeUtil.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <stack>

using namespace std;

namespace ArrayAndString
{
    //-----------------------------------------------------------------------------
    // 12. Integer to Roman (Medium)
    //-----------------------------------------------------------------------------
    string intToRoman(int num)
    {
        string result;
        if (num >= 1000)
        {
            int symbolCount = num / 1000;
            for (; symbolCount > 0; symbolCount--)
            {
                result.push_back('M');
            }
            num %= 1000;
        }
        // C can be placed before D(500) and M(1000) to make 400 and 900.
        if (num >= 900)
        {
            result += "CM";
            num -= 900;
        }

        if (num >= 500)
        {
            result.push_back('D');
            num -= 500;
        }
        if (num >= 400)
        {
            result += "CD";
            num -= 400;
        }

        if (num >= 100)
        {
            int symbolCount = num / 100;
            for (; symbolCount > 0; symbolCount--)
            {
                result.push_back('C');
            }
            num %= 100;
        }
        // X can be placed before L(50) and C(100) to make 40 and 90.
        if (num >= 90)
        {
            result += "XC";
            num -= 90;
        }

        if (num >= 50)
        {
            result.push_back('L');
            num %= 50;
        }
        if (num >= 40)
        {
            result += "XL";
            num -= 40;
        }

        if (num >= 10)
        {
            int symbolCount = num / 10;
            for (; symbolCount > 0; symbolCount--)
            {
                result.push_back('X');
            }
            num %= 10;
        }
        // I can be placed before V (5) and X (10) to make 4 and 9.
        if (num == 9)
        {
            result += "IX";
            num -= 9;
        }

        if (num >= 5)
        {
            result.push_back('V');
            num -= 5;
        }
        if (num == 4)
        {
            result += "IV";
            num -= 4;
        }

        for (; num > 0; num--)
        {
            result.push_back('I');
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 16. 3Sum Closest (Medium)
    //-----------------------------------------------------------------------------
    int threeSumClosest(vector<int>& nums, int target)
    {
        sort(nums.begin(), nums.end());

        int result = 0;
        int absDiff = INT_MAX;

        for (int i = 0; i < nums.size(); ++i)
        {
            int left = i + 1;
            int right = static_cast<int>( nums.size() - 1 );

            while (left < right)
            {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == target)
                {
                    return target;
                }
                else if (sum > target)
                {
                    right--;
                }
                else
                {
                    left++;
                }

                int diff = abs(sum - target);
                if (diff < absDiff)
                {
                    absDiff = diff;
                    result = sum;
                }

            }
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 165. Compare Version Numbers (Medium)
    //-----------------------------------------------------------------------------
    int compareVersion(string version1, string version2)
    {
        istringstream ver1stream(version1 + ".");
        istringstream ver2stream(version2 + ".");
        char dummy;

        while (ver1stream.good() || ver2stream.good())
        {
            int v1 = 0;
            int v2 = 0;

            if (ver1stream.good())
            {
                ver1stream >> v1 >> dummy;
            }
            if (ver2stream.good())
            {
                ver2stream >> v2 >> dummy;
            }

            if (v1 < v2)
            {
                return -1;
            }
            else if (v1 > v2)
            {
                return 1;
            }
        }

        return 0;
    }

    //-----------------------------------------------------------------------------
    // 819. Most Common Word (Easy)
    //-----------------------------------------------------------------------------
    string mostCommonWord(string paragraph, vector<string>& banned)
    {
        // The crucial part is how to split the paragraph.
        // We can use isalpha() to convert all non-alphabetic to space (' ').
        // Then, use istringsteam() to split the paragraph. We can use the
        // extraction operator(>>). The extraction operator will read until
        // whitespace is reached or until the stream fails.

        unordered_set<string> bannedSet(banned.begin(), banned.end());
        for (int i = 0; i < paragraph.size(); ++i)
        {
            if (!isalpha(paragraph[i]))
            {
                paragraph[i] = ' ';
            }
            else if (isupper(paragraph[i]))
            {
                paragraph[i] = tolower(paragraph[i]);
            }
        }

        unordered_map<string, int> wordCountMap;
        istringstream iss(paragraph);
        int maxFreq = 0;
        string result;
        while (iss.good())
        {
            string word;
            iss >> word;
            if (bannedSet.count(word) == 0)
            {
                wordCountMap[word]++;
                if (wordCountMap[word] > maxFreq)
                {
                    maxFreq = wordCountMap[word];
                    result = word;
                }
            }
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 937. Reorder Data in Log Files (Easy)
    //-----------------------------------------------------------------------------
    vector<string> reorderLogFiles(vector<string>& logs)
    {
        using LetterLog = pair<string, string>;
        deque<LetterLog> letterLogs;
        vector<string> digitLogs;

        for (const auto& str : logs)
        {
            int idx = static_cast<int>( str.find_first_of(' ') );
            string identifier = str.substr(0, idx);
            string content = str.substr(idx + 1);

            if (content[0] >= '0' && content[0] <= '9')
            {
                digitLogs.push_back(str);
            }
            else
            {
                letterLogs.emplace_back(identifier, content);
            }
        }

        sort(letterLogs.begin(), letterLogs.end(), [](const LetterLog& left, const LetterLog& right)
            {
                int res = left.second.compare(right.second);
                if (res == 0)
                {
                    return left.first.compare(right.first) < 0;
                }
                else
                {
                    return res < 0;
                }
            });

        vector<string> result;
        for (const auto& letterLog : letterLogs)
        {
            result.emplace_back(letterLog.first);
            result.back().push_back(' ');
            result.back() += letterLog.second;
        }
        result.insert(result.end(), digitLogs.begin(), digitLogs.end());

        return result;
    }

    //-----------------------------------------------------------------------------
    // 1099. Two Sum Less Than K (Easy)
    // i < j with nums[i] + nums[j] = sum and sum < k
    //
    // Might be related to 'Optimal Utilization'.
    //-----------------------------------------------------------------------------

    int twoSumLessThanK(vector<int>& nums, int k)
    {
        sort(nums.begin(), nums.end());
        int left = 0;
        int right = static_cast<int>( nums.size() - 1 );

        int result = -1;
        while (left < right)
        {
            int sum = nums[left] + nums[right];
            if (sum < k)
            {
                left++;
                result = max(result, sum);
            }
            else
            {
                right--;
            }
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 901. Online Stock Span (Medium)
    // Topic: Monotonic stack
    // Related to: 739. Daily Temperatures.
    //-----------------------------------------------------------------------------


    //-----------------------------------------------------------------------------
    // 907. Sum of Subarray Minimums (Medium-Hard)
    // Topic: Monotonic stack
    //
    // This question deserves to be marked Hard. The O(n) solution requires very
    // nontrivial techniques.
    //-----------------------------------------------------------------------------
    int sumSubarrayMins(vector<int>& arr)
    {
        // Some important ideas:
        // A. Previous Less Element and Next Less Element.
        // B. Monotonic increasing stack.
        // We use technique B to obtains A.
        // A[i] = PLE[i] * NLE[i]
        // PLE[i] is the distance from i to the previous element that is less than
        // arr[i].
        // NLE[i] is the distance from i to the next element that is less than
        // arr[i].
        // For example, in [3, 7, 8, 4], The previous less element of 4 is 3.
        // PLE[3] = 3
        // C. The value arr[i] can contribute to the final result is its value *
        //    number of subarrays in which i is minimum element.
        // Denote by left[i] the distance between element arr[i] and its PLE.
        // Denote by right[i] the distance between element arr[i] and its NLE.
        // The number of those subarrays is left[i] * right[i].
        // The final answer is,
        // sum(arr[i] * left[i] * right[i])

        const int len = static_cast<int>( arr.size() );
        // pair = <value, index>
        stack<pair<int, int>> monoStackPLE;
        stack<pair<int, int>> monoStackNLE;

        // left[i] is the distance between element arr[i] and its PLE.
        vector<int> left(len, -1);
        // right[i] is the distance between element arr[i] and its NLE.
        vector<int> right(len, -1);

        // Initialize
        for (int i = 0; i < len; ++i)
        {
            // Assume there is no less on the left.
            // The distance is from it to -1, so it's i + 1.
            // For example, when i = 0, the length should 1 (the subarray only contains itself).
            left[i] = i + 1;
        }
        for (int i = 0; i < len; ++i)
        {
            // Assume there is no less on the right.
            right[i] = len - i;
        }

        for (int i = 0; i < len; ++i)
        {
            // Process PLE.
            while (!monoStackPLE.empty() && monoStackPLE.top().first > arr[i])
            {
                monoStackPLE.pop();
            }
            if (!monoStackPLE.empty())
            {
                left[i] = i - monoStackPLE.top().second;

            }
            monoStackPLE.push({ arr[i], i });

            // Process NLE
            while (!monoStackNLE.empty() && monoStackNLE.top().first > arr[i])
            {
                right[monoStackNLE.top().second] = i - monoStackNLE.top().second;
                monoStackNLE.pop();
            }
            monoStackNLE.push({ arr[i], i });
        }

        int result = 0;
        int modulo = static_cast<int>( 1e9 + 7 );
        for (int i = 0; i < len; ++i)
        {
            long long temp = ( left[i] * right[i] ) % modulo;
            temp = ( temp * arr[i] ) % modulo;
            result = ( result + temp ) % modulo;
        }

        return result;
    }

    // The above solution uses two stacks and two vectors (left/right)
    // Let's try to use one stack and no vector.
    int sumSubarrayMins_improved(vector<int>& arr)
    {
        const int len = static_cast<int>(arr.size());
        // This stack stores the index of element.
        stack<int> monoStack;
        int result = 0;
        int modulo = static_cast<int>( 1e9 + 7 );

        // We want to take care of the last element, so we use <= len.
        // It makes us go into the loop.
        for (int i = 0; i <= len; i++)
        {
            // When i == len, we want to handle the item left in the stack, we always want
            // to enter the while-loop, so we compare with a very small integer.
            while (!monoStack.empty() && arr[monoStack.top()] > ( i == len ? INT_MIN : arr[i] ))
            {
                // We are going to calculate left[i] and right[i] at the same time.
                // Note that we are calculating for the previous greater number, not for arr[i].
                // [2, 5, 7, 6, 3, 4, 8, 1]
                //           ^  We meet
                //        $ Need to pop 7, which is the previous greater number.
                int indexOfGreat = monoStack.top(); // 2 for 7
                monoStack.pop();
                int indexOfLess = monoStack.empty() ? -1 : monoStack.top(); // 1 for 5

                // Now for 7, we know its PLE is 5 at 1 and its NLE is 6 at 3(i).
                // Now we know the left/right boundaries of the possible subarrays in which 7 is the minimum value.
                // They are 1, 3, so the only subarray is [7].
                long long tmp = ( arr[indexOfGreat] * ( i - indexOfGreat ) ) % modulo;
                tmp = ( tmp * ( indexOfGreat - indexOfLess ) ) % modulo;
                result = static_cast<int>( ( static_cast<long long>( result ) + tmp ) % modulo );
            }
            monoStack.push(i);
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 2104. Sum of Subarray Ranges (Medium-Hard-Hard)
    // Topic: Monotonic stack
    // Follow-up: Could you find a solution with O(n) time complexity?
    //-----------------------------------------------------------------------------
    long long subArrayRanges(vector<int>& nums)
    {
        // This is a very hard question. We need to use the technique we used in
        // '907. Sum of Subarray Minimums'.
        const int len = static_cast<int>( nums.size() );
        // This stack stores the index of element.
        stack<int> monoStack;
        long long result = 0;

        // Calculate the Sum of Subarray Maximums.
        for (int i = 0; i <= len; i++)
        {
            // For 'Maximums', we need to use Monotonic decreasing stack.
            while (!monoStack.empty() && nums[monoStack.top()] < ( i == len ? INT_MAX : nums[i] ))
            {
                // [2, 5, 4, 6, 3, 7, 8, 1]
                //           ^  Meet 6
                //        % Need to pop 4
                //     $     $
                long long indexOfLess = monoStack.top(); // 2 for 4
                monoStack.pop();
                long long indexOfGreat = monoStack.empty() ? -1 : monoStack.top(); // 1 for 5

                // For 4, we know its PGE is 5 at 1 and its NGE is 6 at 3(i).
                result += nums[indexOfLess] * ( i - indexOfLess ) * ( indexOfLess - indexOfGreat );
            }
            monoStack.push(i);
        }

        // stack doesn't have clear function, so make a new one.
        monoStack = stack<int>();

        // Calculate the Sum of Subarray Minimums.
        for (int i = 0; i <= len; i++)
        {
            while (!monoStack.empty() && nums[monoStack.top()] > ( i == len ? INT_MIN : nums[i] ))
            {
                // We are going to calculate left[i] and right[i] at the same time.
                // Note that we are calculating for the previous greater number, not for arr[i].
                // [2, 5, 7, 6, 3, 4, 8, 1]
                //           ^  We meet
                //        $ Need to pop 7, which is the previous greater number.
                long long indexOfGreat = monoStack.top(); // 2 for 7
                monoStack.pop();
                long long indexOfLess = monoStack.empty() ? -1 : monoStack.top(); // 1 for 5

                // Now for 7, we know its PLE is 5 at 1 and its NLE is 6 at 3(i).
                result -= nums[indexOfGreat] * ( i - indexOfGreat ) * ( indexOfGreat - indexOfLess );
            }
            monoStack.push(i);
        }

        return result;
    }


    //-----------------------------------------------------------------------------
    // 2281. Sum of Total Strength of Wizards (Hard)
    //
    // Very related to 907. Sum of Subarray Minimums
    //-----------------------------------------------------------------------------
    class Solution2281
    {
    public:
        // Time complexity: O(N): we have 3 passes of the input array length
        // Space complexity : O(N) : 1 prefix array and 1 stack is used
        int totalStrength(vector<int>& strength)
        {
            m_len = static_cast<int>( strength.size() );
            // To find the previous less and next less/equal element, we need a
            // monotonic increasing stack. Note it stores index, not strength.
            stack<int> monoStack;
            int result = 0;

            computeSumOfPrexfixSum(strength);

            // We want to take care of the last element, so we use <= len.
            for (int i = 0; i <= m_len; ++i)
            {
                int currStrength = i == m_len ? 0 : strength[i];
                //    [1, 3, 1, 2  E ]
                //                 ^  We meet
                //              $ Need to pop 2, which is the previous greater number.
                while (!monoStack.empty() && strength[monoStack.top()] > currStrength)
                {
                    int idxOfGreat = monoStack.top(); // 3 of 2.
                    monoStack.pop();
                    int idxOfLess = monoStack.empty() ? -1 : monoStack.top(); // 2 of 1.
                    // Now i is the right boundary and idxOfLess is the left boundary.

                    long long sum = getSumInRangeWithMinValue(idxOfGreat, idxOfLess, i);

                    result = ( result + ( sum * strength[idxOfGreat] ) % m_modulo ) % m_modulo;
                }

                monoStack.push(i);
            }

            return result;
        }

    private:

        long long getSumInRangeWithMinValue(int indexOfMin, int left, int right)
        {
            // How do we get the "sum of all subarrays including strength[i] in range (left, right)"?
            // Let's list the indexes:
            //  ...left - 1, left, left + 1, left + 2, ... i - 1, i, i + 1, ... right - 1, right, right + 1...
            // positive parts:
            // ( prefix[i + 1] + prefix[i + 2] + ... + prefix[right] ) * ( i - left )
            //    negative parts :
            // ( prefix[left + 1] + prefix[left + 2] + ... + prefix[i] ) * ( right - i )

            long long positive = m_sumOfPrefixSum[right + 1] - m_sumOfPrefixSum[indexOfMin + 1];
            positive = ( positive * ( indexOfMin - left ) ) % m_modulo;

            long long negative = m_sumOfPrefixSum[indexOfMin + 1] - m_sumOfPrefixSum[left + 1];
            negative = ( negative * (right - indexOfMin ) ) % m_modulo;

            while (negative > positive)
            {
                // I really hate this question...
                // I hate all questions that need to do the stupid modulo process.
                positive += m_modulo;
            }

            return positive - negative;
        }

        void computeSumOfPrexfixSum(const vector<int>& strength)
        {
            // To calculate the sum of all subarrays in certain range.
            // We can use prefix sum and sum of prefix sum.
            // For example, [1, 2, 3]. len = 3
            //              [0, 1, 3, 6]  <- Prefix sum.
            //              [0, 0, 1, 4, 10]
            // Index         0  1  2  3  4
            //              -1  0  1  2  3   <-- Index in strength array.

            long long prefixSum = 0;
            // Sum of first k prefix
            m_sumOfPrefixSum.resize(m_len + 2);
            for (int i = 0; i <= m_len; ++i)
            {
                m_sumOfPrefixSum[i + 1] = ( m_sumOfPrefixSum[i] + prefixSum ) % m_modulo;
                if (i != m_len)
                {
                    prefixSum = ( prefixSum + strength[i] ) % m_modulo;
                }
            }
        }

    private:
        // Sum of first k prefix. Note that this array is 1-indexed because we
        // use '-1' and 'length of array' to denote the left-most and right-most boundary of an array.
        vector<long long> m_sumOfPrefixSum;
        const int m_modulo = 1000000007;
        int m_len = 0;
    };

    //-----------------------------------------------------------------------------
    // 1567. Maximum Length of Subarray With Positive Product
    //-----------------------------------------------------------------------------
    int getMaxLen(vector<int>& nums)
    {
        bool negative = false;
        int maxLen = 0;
        // The start position of the subarray that may or may not contain negative number.
        int startPosNegative = 0;
        // The start position of the subarray that doesn't contain negative number.
        int startPosPositive = -1;
        int firstNegative = -1;

        for (int i = 0; i < nums.size(); ++i)
        {
            if (nums[i] == 0)
            {
                // Handle the case:
                //[-7,-10,-7,21,20,-12,-34,26,2,0,......]
                //     ^^^^^^^^^^^^^^^^^^^^^^^^
                if (negative && nums[firstNegative] < 0)
                {
                    // If the start position is negative.
                    maxLen = max(maxLen, i - firstNegative - 1);
                }

                startPosNegative = i + 1;
                startPosPositive = -1;
                firstNegative = -1;
                negative = false;
            }
            else
            {
                if (nums[i] < 0)
                {
                    negative = !negative;
                    startPosPositive = -1;
                    if (firstNegative == -1)
                    {
                        firstNegative = i;
                    }
                }
                else
                {
                    if (negative && startPosPositive == -1)
                    {
                        startPosPositive = i;
                    }
                    if (startPosPositive != -1)
                    {
                        maxLen = max(maxLen, i - startPosPositive + 1);
                    }
                }

                if (!negative)
                {
                    maxLen = max(maxLen, i - startPosNegative + 1);
                }
                else if(i == nums.size() - 1)
                {
                    // Handle the case:
                    //[-7,-10,-7,21,20,-12,-34,26,2]
                    //     ^^^^^^^^^^^^^^^^^^^^^^^^
                    if (negative && nums[firstNegative] < 0)
                    {
                        // If the start position is negative.
                        maxLen = max(maxLen, i - firstNegative);
                    }
                }
            }
        }

        return maxLen;
    }


    //-----------------------------------------------------------------------------
    // Test function
    //-----------------------------------------------------------------------------
    void TestArrayAndString()
    {
        // 12. Integer to Roman
        // Input: num = 58
        // Output: "LVIII"
        // Input: num = 1994
        // Output: "MCMXCIV"
        cout << "\n12. Integer to Roman " << intToRoman(1994) << endl;

        // 16. 3Sum Closest (Medium)
        // Input: nums = [-1,2,1,-4], target = 1
        // Output: 2
        vector<int> inputVI = { -1,2,1,-4 };
        cout << "\n16. 3Sum Closest: " << threeSumClosest(inputVI, 1);

        // 165. Compare Version Numbers
        //Input: version1 = "1.01", version2 = "1.001"
        // Output : 0
        // Input: version1 = "0.1", version2 = "1.1"
        // Output: -1
        cout << "\n165. Compare Version Numbers: " << compareVersion("1.2.3", "1.2.1");

        // 819. Most Common Word
        // Input: paragraph = "Bob hit a ball, the hit BALL flew far after it was hit.", banned = ["hit"]
        // Output : "ball"
        vector<string> words{ "hit" };
        cout << "\n819. Most Common Word: " << mostCommonWord("Bob hit a ball, the hit BALL flew far after it was hit.", words) << endl;

        // 937. Reorder Data in Log Files
        // Input: logs = ["dig1 8 1 5 1","let1 art can","dig2 3 6","let2 own kit dig","let3 art zero"]
        // Output: ["let1 art can", "let3 art zero", "let2 own kit dig", "dig1 8 1 5 1", "dig2 3 6"]
        words = { "dig1 8 1 5 1","let1 art can","dig2 3 6","let2 own kit dig","let3 art zero" };
        cout << "\n937. Reorder Data in Log Files: " << endl;
        auto resultVS =reorderLogFiles(words);
        LeetCodeUtil::PrintVector(resultVS);

        // 1099. Two Sum Less Than K
        // Input: nums = [34,23,1,24,75,33,54,8], k = 60
        // Output: 58
        // Input: nums = [1, 2, 4, 5], k = 6
        // Output:  5
        inputVI = { 34,23,1,24,75,33,54,8 };
        cout << "\n1099. Two Sum Less Than K: " << twoSumLessThanK(inputVI, 60) << endl;

        // 907. Sum of Subarray Minimums
        // Input: arr = [3,1,2,4]
        // Output: 17
        // Input: arr = [11,81,94,43,3]
        // Output: 444
        inputVI = { 11,81,94,43,3 };
        cout << "\n907. Sum of Subarray Minimums: " << sumSubarrayMins_improved(inputVI) << endl;

        // 2104. Sum of Subarray Ranges (Medium-Hard-Hard)
        // Input: nums = [4,-2,-3,4,1]
        // Output: 59
        inputVI = { 4,-2,-3,4,1 };
        cout << "\n2104. Sum of Subarray Ranges: " << subArrayRanges(inputVI) << endl;

        // 2281. Sum of Total Strength of Wizards
        // Input: strength = [1,3,1,2]
        // Output: 44
        // Input: strength = [1, 2, 3]
        // Output: 33
        Solution2281 sol2281;
        inputVI = { 1932,1660,828,290,1808,446,1107,1843,913,856,503,535,263,704,13,1188,1391,706,861,867,262,304,1599,732,1365,1244,8,1936,642,1003,844,1539,619,135,1199,730,1301,1701,1959,243,1598,1498,346,1629,1739,1612,862,1857,1137,1728,57,233,1369,1016,1709,955,1258,705,1025,1521,1529,1161,1387,1220,667,299,395,1518,1959,873,472,1870,1266,1105,887,1471,1484,614,1762,46,1054,1555,939,287,1457,1934,1181,1592,434,628,344,995,1225,1028,706,621,881,881,100,1064,1410,335,70,1143,558,275,1635,1227,981,1360,31,1097,1770,648,336,523,937,283,1220,1964,115,1908,1554,831,1437,193,751,177,1562,584,1268,1810,451,685,1206,1406,1918,925,965,1221,1073,50,329,256,778,1404,1108,540,247,1485,1795,1131,1646,1532,1071,615,1764,922,1348,240,1547,1865,648,1434,1774,500,953,1840,1096,1619,1444,295,114,1392,1708,1023,1139,25,554,931,33,543,936,1907,1869,1376,705,970,1448,1085,590,499,718,1623,237,625,15,1717,765,161,1054,939,1120,264,1688,586,195,564,1737,1644,1219,693,697,222,1938,398,1033,1570,148,1764,724,1066,427,1151,120,853,605,406,1125,723,195,472,1832,804,274,895,221,528,1461,1598,110,1322,356,648,1768,1527,1443,349,1454,100,461,1410,1361,1318,411,493,781,548,1663,109,156,532,1642,1886,606,1161,743,1935,1525,1561,1892,1726,1418,1328,420,1699,83,891,1926,1225,1531,1060,315,632,1927,193,317,405,90,659,801,321,1315,113,298,408,1471,1308,1687,1192,170,1440,1558,1658,726,1405,343,1251,1324,122,1482,1790,1479,1649,850,1579,688,1297,1047,283,1860,554,941,1322,1242,1076,188,1824,324,1673,551,716,1611,1068,1964,1454,504,1030,45,1526,386,1797,831,525,822,688,872,1214,516,459,500,357,1412,98,1773,1043,15,416,1065,1154,45,1499,1927,1182,7,1270,1996,184,826,1586,678,1176,1697,1461,387,1942,1213,791,1414,1684,1514,660,835,1375,371,1232,1195,546,716,1832,1248,1375,513,1681,1422,1527,352,884,1595,1348,1196,693,1990,165,1830,1537,711,212,159,813,212,557,1843,215,1619,95,853,424,375,1040,1195,820,443,481,1310,1167,623,528,535,782,287,1972,1748,504,709,675,1005,136,285,106,143,659,1882,666,1929,532,1858,1532,1828,1078,1846,1886,1531,1945,1036,154,1022,390,1388,247,26,1414,1012,1656,538,449,1352,1106,680,1706,1995,1882,1220,1269,1684,1098,1184,1553,1464,136,1489,667,65,23,1711,701,1540,1173,1922,744,968,499,843,971,40,157,670,735,566,717,552,55,39,1597,450,1276,603,1430,831,1644,1277,1250,1710,905,1521,688,248,830,1720,734,420,475,29,1736,194,1470,1610,1264,3,1155,8,589,1361,1374,1462,483,189,1955,1241,1130,1759,681,1412,556,466,100,611,329,1492,1598,823,789,973,1095,1175,1089,1609,875,1142,490,1800,1516,1261,132,1378,586,533,229,153,1112,1755,1042,799,69,1504,1991,379,1066,1064,778,481,388,1936,1590,541,1318,937,832,11,1929,280,1888,1579,1974,1844,381,421,1478,1307,1744,1036,324,1468,196,1608,287,168,1726,1774,427,1730,1597,1727,747,1851,1006,1099,519,1676,1477,1159,1739,1185,890,511,94,1724,1690,1470,1175,583,1754,811,1888,321,1373,347,1389,1547,776,799,1579,1620,1387,668,937,547,871,83,1238,1908,1035,1005,1542,163,101,760,426,834,353,1178,1461,691,1256,643,1679,1366,463,1383,1655,82,896,332,134,1756,1598,1878,1977,439,94,1125,1323,1948,945,264,1250,726,1866,1764,384,1358,184,751,1287,192,220,150,1404,1649,1401,1794,1469,1703,688,1581,1112,798,1139,307,660,832,10,968,658 };
        cout << "\n2281. Sum of Total Strength of Wizards: " << sol2281.totalStrength(inputVI) << endl;

        // 1567. Maximum Length of Subarray With Positive Product
        // Input: nums = [-1,-2,-3,0,1]
        // Output: 2
        // Input: nums = [-1,2]
        // Output: 1
        // Input: nums = [0,0,0,0,0]
        // Output: 0
        // Input: nums = [5,-20,-20,-39,-5,0,0,0,36,-32,0,-7,-10,-7,21,20,-12,-34,26,2]
        // Output: 8
        // Input: nums = [25,10,-28,-12,-13,-16,-13,28,5,21,28,4,0,-1]
        // Output: 9
        inputVI = { 25,10,-28,-12,-13,-16,-13,28,5,21,28,4,0,-1 };
        cout << "\n1567. Maximum Length of Subarray With Positive Product: " << getMaxLen(inputVI) << endl;
    }
}