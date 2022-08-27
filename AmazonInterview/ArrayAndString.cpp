#include "LeetCodeUtil.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <stack>
#include <set>
#include <numeric>

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

    #if(SMART)
    string intToRoman(int num)
    {
        string ones[] = { "","I","II","III","IV","V","VI","VII","VIII","IX" };
        string tens[] = { "","X","XX","XXX","XL","L","LX","LXX","LXXX","XC" };
        string hrns[] = { "","C","CC","CCC","CD","D","DC","DCC","DCCC","CM" };
        string ths[] = { "","M","MM","MMM" };

        return ths[num / 1000] + hrns[( num % 1000 ) / 100] + tens[( num % 100 ) / 10] + ones[num % 10];
    }
    #endif

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
    // 937. Reorder Data in Log Files (Medium)
    //-----------------------------------------------------------------------------
    vector<string> reorderLogFiles(vector<string>& logs)
    {
        // Use a pair to store the letter log.
        // The 1st is key and the 2nd is the content.
        using LetterLog = pair<string, string>;
        deque<LetterLog> letterLogs;
        vector<string> digitLogs;

        for (const auto& str : logs)
        {
            int idx = static_cast<int>( str.find_first_of(' ') );
            string identifier = str.substr(0, idx);
            string content = str.substr(idx + 1);

            // Examine the content part to determine if it is digit log.
            if (isdigit(content[0]))
            {
                digitLogs.push_back(str);
            }
            else
            {
                letterLogs.emplace_back(identifier, content);
            }
        }

        // Sort the letter logs. Firstly by content, if identical, then by key.
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
        // Insert the letter logs first.
        for (const auto& letterLog : letterLogs)
        {
            result.emplace_back(letterLog.first);
            result.back().push_back(' ');
            result.back() += letterLog.second;
        }
        // Then insert digit logs.
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
            // For each nums[i], find the boundary [left, right], in which nums[i] is the maximum number.
            // For example. In [7 5 6 4 8], for 6, left is 7 and right is 8. The boundary forms the subarray [5,6,4].
            //                  &   ^   &
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
        // For each nums[i], find the boundary [left, right], in which nums[i] is the minimum number.
        // For example. In [1 5 3 4 2], for 3, left is 1 and right is 2. The boundary forms the subarray [5,3,4].
        //                  &   ^   &
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
            //
            // Let prefix[i] be the prefix sum of first i elements in strength.
            //
            // The sum of subarrays including i are :
            /*
            the subarrays that start with left + 1 :
                sum(left + 1, ... i) = prefix[i + 1] - prefix[left + 1]
                sum(left + 1, ... i + 1) = prefix[i + 2] - prefix[left + 1]
                ...
                sum(left + 1, ... right - 1) = prefix[right] - prefix[left + 1]

            the subarrays that start with left + 2 :
                sum(left + 2, ... i) = prefix[i + 1] - prefix[left + 2]
                sum(left + 2, ... i + 1) = prefix[i + 2] - prefix[left + 2]
                ...
                sum(left + 2, ... right - 1) = prefix[right] - prefix[left + 2]
                ...

            the subarrays that start with i :
                sum(i, ... i) = prefix[i + 1] - prefix[i]
                sum(i, ... i + 1) = prefix[i + 2] - prefix[i]
                ...
                sum(i, ... right - 1) = prefix[right] - prefix[i]
                Then we combine all above terms, we have :
            */
            //
            // positive parts:
            // ( prefix[i + 1] + prefix[i + 2] + ... + prefix[right] ) * ( i - left )
            // negative parts:
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
    // 1567. Maximum Length of Subarray With Positive Product (Medium)
    //-----------------------------------------------------------------------------
    int getMaxLen(vector<int>& nums)
    {
        bool negative = false;
        int maxLen = 0;
        // The start position of the subarray that may or may not contain negative number.
        int startPos = 0;
        int firstNegative = -1;

        for (int i = 0; i < nums.size(); ++i)
        {
            if (nums[i] == 0)
            {
                firstNegative = -1;
                negative = false;
                startPos = i + 1;
            }
            else
            {
                if (nums[i] < 0)
                {
                    negative = !negative;
                    if (firstNegative == -1)
                    {
                        firstNegative = i;
                    }
                }

                if (!negative)
                {
                    maxLen = max(maxLen, i - startPos + 1);
                }
                else if(nums[i] > 0)
                {
                    maxLen = max(maxLen, i - firstNegative);
                }
            }
        }

        return maxLen;
    }

    //-----------------------------------------------------------------------------
    // 1151. Minimum Swaps to Group All 1's Together (Medium)
    //-----------------------------------------------------------------------------
    int minSwaps(vector<int>& data)
    {
        // Idea: Assume that there is total n 1. Image a sliding window with n size.
        // If there is total n 1-digit in the window, it means all 1 are grouped together.
        // No need to swap, the answer is 0.
        // If there is n-1 1-digit in the window, it means we at least need one swap, so
        // the answer is 1.
        //
        // As for implementation, we can use prefix sum to know the number of 1 in
        // a window.
        // 0 1 0 1 1 <- data
        // 0 1 1 2 3 <- prefix sum
        //   ^   ^    psum[3] - psum[1] = 2 - 1 = 1. So, data[2~3] has 1 1-digit.
        // std::partial_sum() can be used to calculate the prefix sum.
        vector<int> prefixSum(data.size());

        std::partial_sum(data.begin(), data.end(), prefixSum.begin());

        int totalCountOfOne = prefixSum.back();
        if (totalCountOfOne == 0) return 0;

        // Initialize to the value of the 1st windows(0 ~ n - 1).
        int maxCountOfOneInWindow = prefixSum[totalCountOfOne - 1];
        for (int i = totalCountOfOne; i < data.size(); ++i)
        {
            maxCountOfOneInWindow = max(maxCountOfOneInWindow, prefixSum[i] - prefixSum[i - totalCountOfOne]);
        }

        return totalCountOfOne - maxCountOfOneInWindow;
    }

    //-----------------------------------------------------------------------------
    // 370. Range Addition (Medium)
    //-----------------------------------------------------------------------------
    vector<int> getModifiedArray(int length, vector<vector<int>>& updates)
    {
        vector<int> result(length, 0);
        // Put inc at startIndex allows the inc to be carried to the next index
        // starting from startIndex when we do the sum accumulation.
        // Put - inc at endIndex + 1 simply means cancel out the previous carry
        // from the next index of the endIndex, because the previous carry should not be counted beyond endIndex.
        for (const auto& update : updates)
        {
            result[update[0]] += update[2];
            if (update[1] < length - 1)
            {
                result[update[1] + 1] -= update[2];
            }
        }

        // Calculate the prefix sum - let increment in every startIndex raise a
        // wave to the endIndex.
        for (int i = 1; i < length; ++i)
        {
            result[i] += result[i - 1];
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 2214. Minimum Health to Beat Game
    //-----------------------------------------------------------------------------
    long long minimumHealth(const vector<int>& damage, int armor)
    {
        // Use the armor on the level which makes greatest damage.
        long long totalDamage = 0;
        for (const auto& d : damage)
        {
            totalDamage += d;
        }

        long long maxDamage = *std::max_element(damage.begin(), damage.end());

        totalDamage -= maxDamage > armor ? armor : maxDamage;

        return totalDamage + 1;
    }

    //-----------------------------------------------------------------------------
    // 735. Asteroid Collision
    //-----------------------------------------------------------------------------
    vector<int> asteroidCollision(const vector<int>& asteroids)
    {
        vector<int> result;
        for (const auto& rock : asteroids)
        {
            if (result.empty() || rock > 0)
            {
                result.push_back(rock);
            }
            else
            {
                while (true)
                {
                    if (result.empty() || result.back() < 0)
                    {
                        result.push_back(rock);
                        break;
                    }
                    else if (result.back() == -rock)
                    {
                        // Both rocks explode.
                        result.pop_back();
                        break;
                    }
                    else if (result.back() < (-rock) )
                    {
                        // The previous rock is smaller, so it explodes.
                        // Let to left rock keep going.
                        result.pop_back();
                    }
                    else
                    {
                        // The last rock is bigger.
                        break;
                    }
                }
            }
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 2193. Minimum Number of Moves to Make Palindrome (Hard)
    //-----------------------------------------------------------------------------
    int minMovesToMakePalindrome(string inputStr)
    {
        int result = 0;
        while (!inputStr.empty())
        {
            // Find the first occurrence of the last char.
            const size_t idx = inputStr.find(inputStr.back());
            if (idx == inputStr.size() - 1)
            {
                // If this char only occurs in the end, it is unique.
                // So, we must move it to the middle.
                result += static_cast<int>(idx / 2);
            }
            else
            {
                // It takes 'idx' steps to move it to the beginning.
                result += static_cast<int>(idx);
                // Remove this char.
                inputStr.erase(idx, 1);
            }
            inputStr.pop_back();
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 767. Reorganize String (Medium)
    //-----------------------------------------------------------------------------
    string reorganizeString(const string& s)
    {
        const size_t len = s.size();
        unordered_map<char, size_t> charCountMap;
        for (const auto& c : s)
        {
            charCountMap[c]++;
        }

        size_t highestFred = 0;
        priority_queue<pair<size_t, char>> higherFreq;
        for (const auto& p : charCountMap)
        {
            highestFred = max(highestFred, p.second);
            if (highestFred > ( len + 1 ) / 2)
            {
                return "";
            }

            higherFreq.emplace(p.second, p.first);
        }

        string result;
        while (higherFreq.size() >= 2)
        {
            auto higher = higherFreq.top();
            higherFreq.pop();
            auto lower = higherFreq.top();
            higherFreq.pop();

            result.push_back(higher.second);
            result.push_back(lower.second);

            higher.first--;
            if (higher.first > 0)
            {
                higherFreq.push(higher);
            }
            lower.first--;
            if (lower.first > 0)
            {
                higherFreq.push(lower);
            }
        }

        if (!higherFreq.empty())
        {
            result.push_back(higherFreq.top().second);
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 1291. Sequential Digits (Medium)
    //
    // 10 <= low <= high <= 10^9
    //-----------------------------------------------------------------------------
    vector<int> sequentialDigits(int low, int high)
    {
        // One solution is just write down all possible digits, like
        // { 12,23,34,45,56,67,78,89,123,234,345,456,567,678,789,1234,2345,3456,4567,5678,6789,
        // 12345, 23456, 34567, 45678, 56789, 123456, 234567, 345678, 456789, 1234567, 2345678,
        // 3456789, 12345678, 23456789, 123456789 }
        // And put those >= low and <= high into the result vector.

        // The following code shows another solution.
        queue<int> candidates;
        for (int i = 1; i <= 9; ++i)
        {
            candidates.push(i);
        }

        vector<int> result;
        while (!candidates.empty())
        {
            auto num = candidates.front();
            candidates.pop();

            if (num >= low && num <= high)
            {
                result.push_back(num);
            }

            // candidates is a queue, if num is greater than high, then everything
            // beyond num will be greater than high. So, we can break.
            if (num > high)
            {
                break;
            }

            // If 12 is eligible, we will try 123, then 1234...
            int lastDigit = num % 10;
            if (lastDigit != 9)
            {
                // Exclude 9 since we are unable to add anything after it.
                candidates.push(num * 10 + ( lastDigit + 1 ));
            }
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 1856. Maximum Subarray Min-Product (Medium)
    //
    // Related to 907, 2104, 2281
    //-----------------------------------------------------------------------------
    class Solution1856
    {
    public:
        int maxSumMinProduct(const vector<int>& nums)
        {
            // We need to a monotonic increasing stack and a prefix sum array.
            const size_t len = nums.size();
            vector<long long> prefixSum(len + 1, 0);
            // assume nums =   [1 2 3]
            // prefixSum =   [0 1 3 6]
            // partial_sum causes stack overflow on int.
            //std::partial_sum(nums.begin(), nums.end(), prefixSum.begin() + 1);
            for (int i = 1; i <= len; ++i)
            {
                prefixSum[i] = prefixSum[i - 1] + nums[i - 1];
            }

            int modulo = static_cast<int>( 1e9 + 7 );
            long long result = 0;
            stack<size_t> monoStack;
            for (size_t i = 0; i <= len; ++i)
            {
                const int value = i != len ? nums[i] : INT_MIN;
                while (!monoStack.empty() && nums[monoStack.top()] > value)
                {
                    // [2, 5, 7, 6, 3, 4, 8, 1]
                    //           ^  We meet
                    //        $ Need to pop 7, which is the previous greater number.
                    size_t idxGreat = monoStack.top(); // 2 of 7.
                    monoStack.pop();
                    size_t idxLess = monoStack.empty() ? -1 : monoStack.top(); // 1 of 5

                    // Prefix sum = [0 2 7 14 20 ...]
                    //                   ^ ^ 14 - 7 = 7
                    long long product = nums[idxGreat] * ( prefixSum[i] - prefixSum[idxLess + 1] );
                    result = max(result, product);
                }

                if (i != len)
                {
                    monoStack.push(i);
                }
            }

            return result % modulo;
        }
    };

    //-----------------------------------------------------------------------------
    // 239. Sliding Window Maximum (Hard)
    //-----------------------------------------------------------------------------
    vector<int> maxSlidingWindow(const vector<int>& nums, int k)
    {
        #if( SLOW )
        int left = 0;
        int right = left + k - 1;

        multiset<int> numSet(nums.begin(), nums.begin() + k);
        int maxNum = *numSet.rbegin();
        vector<int> result{ maxNum };

        for (; right < nums.size() - 1; )
        {
            auto it = numSet.find(nums[left]);
            // Note that erase(val) will remove ALL elements which are that value.
            // However, we just need to remove one, so we use erase(iterator).
            numSet.erase(it);
            left++;
            right++;
            numSet.insert(nums[right]);
            maxNum = *numSet.rbegin();
            result.push_back(maxNum);
        }

        return result;
        #else

        // Monotonic decreasing queue to store the index.
        deque<int> queue;
        vector<int> ans;
        for (int left = 0; left < nums.size(); ++left)
        {
            // As the window move on, element nums[left-k] will be outdated.
            if (!queue.empty() && queue.front() == left - k) queue.pop_front();
            // Now we are ready to push our new element nums[left]'s index into the queue.
            // But before that, we should clear elements which is smaller then nums[left].
            // Why? Because if nums[left] is bigger then nums[i],
            // there will be no way for nums[i] be selected as the max number in range (left-k, left]
            while (!queue.empty() && nums[queue.back()] < nums[left]) queue.pop_back();
            // Now push the index into our queue.
            queue.push_back(left);
            // Okay, now nums[queue.front()] mush be the max number in range (left-k, left]
            if (left - k + 1 >= 0) ans.push_back(nums[queue.front()]);
        }
        return ans;

        #endif
    }

    //-----------------------------------------------------------------------------
    // 134. Gas Station (Medium)
    //-----------------------------------------------------------------------------
    int canCompleteCircuit(const vector<int>& gas, const vector<int>& cost)
    {
        const size_t len = gas.size();

        int totalGas = std::accumulate(gas.begin(), gas.end(), 0);
        int totalCost = std::accumulate(cost.begin(), cost.end(), 0);
        if (totalCost > totalGas)
        {
            return -1;
        }

        for (int i = 0; i < len; ++i)
        {
            bool reachable = true;
            int vol = 0;
            for (int j = i; j < i + len; ++j)
            {
                const int idx = j % len;
                vol += gas[idx] - cost[idx];
                if (vol < 0)
                {
                    reachable = false;
                    if (j >= len)
                    {
                        // Any position before i has been tested, no need to test again.
                        // So, when j goes beyond the end (len-1), we know there is no solution.
                        return -1;
                    }
                    else if (idx > i)
                    {
                        // Important idea : If we cannot go from i to j (j < len), then we know that we can't
                        // go from i+1 to j. So, we can choose the next starting point wisely.
                        // We could start j.
                        i = idx - 1;
                    }
                    break;
                }
            }

            if (reachable)
            {
                return i;
            }
        }

        return -1;
    }

    //-----------------------------------------------------------------------------
    // 560. Subarray Sum Equals K (Medium)
    //-----------------------------------------------------------------------------
    int subarraySum(const vector<int>& nums, int k)
    {
        // Use a hash table to store the prefix sum and its number of occurrence.
        // The same prefix sum may occur multiple times.
        // For example:
        // 1 0 1 -1 -1 1 ...
        // 0 1 2  1  0 1
        unordered_map<int, size_t> prefixSim2Count;
        int sum = 0;
        // If the first number is the same as k, count it.
        int result = nums[0] == k ? 1 : 0;
        // Consider the subarray starts from 0 index.
        prefixSim2Count[0] = 1;
        for (int i = 0; i < nums.size(); ++i)
        {
            sum += nums[i];
            const int diff = sum - k;
            if (prefixSim2Count.count(diff) > 0)
            {
                result += static_cast<int>(prefixSim2Count[diff]);
            }
            prefixSim2Count[sum]++;
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 2221. Find Triangular Sum of an Array (Medium)
    //-----------------------------------------------------------------------------
    int triangularSum(vector<int>& nums)
    {
        size_t len = nums.size();
        while (len > 1)
        {
            for (size_t i = 0; i < len - 1; ++i)
            {
                nums[i] = ( nums[i] + nums[i + 1] ) % 10;
            }

            len--;
        }

        return nums.front();
    }

    //-----------------------------------------------------------------------------
    // 135. Candy (Hard)
    // Topic: Greedy
    //-----------------------------------------------------------------------------
    int candy(const vector<int>& ratings)
    {
        // The following implementation shows the solution having O(n) space complexity.
        // There is a solution having O(1) space complexity but it is not easy.
        const size_t len = ratings.size();
        // If there is just one child, we just need 1 candy.
        if (len == 1) return 1;

        // Every child at least has 1 candy.
        vector<int> candies(len, 1);

        // Scan from left to right to make sure the current child has more candy
        // if the current one has higher rating than its left neighbor.
        for (int i = 1; i < len; ++i)
        {
            if (ratings[i] > ratings[i - 1])
            {
                candies[i] = candies[i - 1] + 1;
            }
        }

        // Scan from right to left to make sure the current child has more candy
        // if the current on has higher rating than its right neighbor.
        for (int i = static_cast<int>(len - 2); i >= 0; i--)
        {
            if (ratings[i] > ratings[i + 1])
            {
                // We want child[i] to have more than child[i] and also child[i-1].
                // We cannot simply use candies[i + 1] + 1 because we don't know the
                // relationship between candies[i] and candies[i + 1] + 1.
                // Maybe candies[i] is already larger than candies[i + 1] + 1.
                candies[i] = max(candies[i], candies[i + 1] + 1);
            }
        }

        return std::accumulate(candies.begin(), candies.end(), 0);
    }

    //-----------------------------------------------------------------------------
    // 564. Find the Closest Palindrome (Hard)
    //-----------------------------------------------------------------------------
    string nearestPalindromic(const string& n)
    {
        size_t digits = n.size();
        if (digits == 1)
        {
            // Special case for single digit numbers.
            return to_string(stoi(n) - 1);
        }

        // Assume the input string has 4 digits - 2537.
        // There are 5 possible cases.
        // Case 1. The candidate has one more digit, which must be 10001.
        // Case 2. The candidate has one less digit, which must be 999.
        // Case 3. The candidate has the same digit. The prefix remains the same (25), and the postfix is the mirror of the prefix (2552).
        // Case 4. The candidate has the same digit. The prefix increases by 1 (26), and the postfix is the mirror of the prefix (2662).
        // Case 5. The candidate has the same digit. The prefix decreases by 1 (24), and the postfix is the mirror of the prefix (2442).
        // Among these 5 candidates, the one having the least absolute difference from the original number is the answer.
        vector<unsigned long long> candidates;

        // + 1 is for the input having odd digit.
        int prefix = stoi(n.substr(0, ( digits + 1 ) / 2));
        // Deal with case 3 ~ 5.
        // prefix + 1 might make the number of digits increase but it doesn't matter because it would only generate bigger candidate.
        // However, we only care the smallest candidate.
        vector<int> prefixes{ prefix, prefix + 1, prefix - 1 };
        for (const auto& prefix : prefixes)
        {
            // Prefix of 12345 is 123.
            string prefixStr = to_string(prefix);
            string posfix(prefixStr);
            if (digits & 0b1)
            {
                // If the digits is odd, we need to remove the number in the middle.
                posfix.pop_back();
            }
            std::reverse(posfix.begin(), posfix.end());
            string full = prefixStr + posfix;
            // If n is "11", we will have one candidate as 11, which is not eligible. We shouldn't add it.
            if (full != n)
            {
                candidates.push_back(stoull(full));
            }
        }

        // Case 1:
        candidates.push_back(static_cast<unsigned long long>(pow(10, digits)) + 1);
        // Case 2:
        candidates.push_back(static_cast<unsigned long long>(pow(10, digits - 1)) - 1);

        unsigned long long original = stoull(n);
        long long minDiff = std::numeric_limits<long long>::max();
        int resultIdx = 0;
        for (int i = 0; i < candidates.size(); ++i)
        {
            long long diff = abs(static_cast<long long>(candidates[i] - original));
            if (diff < minDiff || (diff == minDiff && candidates[i] < candidates[resultIdx]))
            {
                minDiff = diff;
                resultIdx = i;
            }
        }

        return to_string(candidates[resultIdx]);
    }

    //-----------------------------------------------------------------------------
    // 1209. Remove All Adjacent Duplicates in String II (Medium)
    //-----------------------------------------------------------------------------
    string removeDuplicates(const string& s, int k)
    {
        // Using one stack to store the number of its occurrence.
        vector<pair<size_t, char>> charStack;
        for (const auto& c : s)
        {
            if (charStack.empty() || charStack.back().second != c)
            {
                charStack.emplace_back(1, c);
            }
            else
            {
                charStack.back().first++;
                if (charStack.back().first == k)
                {
                    charStack.pop_back();
                }
            }
        }

        string result;
        for (const auto& p : charStack)
        {
            result.append(p.first, p.second);
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 1864. Minimum Number of Swaps to Make the Binary String Alternating (Medium)
    //-----------------------------------------------------------------------------
    class Solution1864
    {
    public:
        int minSwaps(string s)
        {
            const size_t len = s.size();
            // Count how many 1 and 0 we have.
            vector<int> digitCount(2);

            // Take care of the 1st character.
            digitCount[s[0] - '0']++;
            char nextValidChar = s[0] == '0' ? '1' : '0';
            // If we don't swap the 1st, calculate the mismatched count.
            int mismatched1 = 0;
            // If we swap the 1st, calculate the mismatched count.
            int mismatched2 = 1;


            for (int i = 1; i < len; ++i)
            {
                if (s[i] != nextValidChar)
                {
                    mismatched1++;
                }
                else
                {
                    mismatched2++;
                }
                nextValidChar = nextValidChar == '0' ? '1' : '0';

                digitCount[s[i] - '0']++;
            }

            int mismatched = 0;

            if (( len & 0b1 ) == 0)
            {
                // Even length
                if (digitCount[0] != digitCount[1])
                {
                    // If count of 0 and 1 are not same.
                    return -1;
                }

                mismatched = min(mismatched1, mismatched2);
            }
            else
            {
                // Odd length
                if (1 != abs(digitCount[0] - digitCount[1]))
                {
                    return -1;
                }

                // If s starts with 0 and there is more 0 than 1, we must choose
                // mismatched1 because we must not swap the 1st 0 to 1.
                if (( s[0] == '0' && digitCount[0] > digitCount[1] ) ||
                    ( s[0] == '1' && digitCount[1] > digitCount[0] ))
                {
                    mismatched = mismatched1;
                }
                else
                {
                    mismatched = mismatched2;
                }

            }

            return mismatched / 2;
        }
    };

    //-----------------------------------------------------------------------------
    // 2340. Minimum Adjacent Swaps to Make a Valid Array (Medium)
    //
    // Topic: Greedy
    //-----------------------------------------------------------------------------
    int minimumSwaps(vector<int>& nums)
    {
        // This is a advanced practice of two pointers technique.
        // Find:
        // idxMin: the index of leftmost smallest element. The distance to 0 is
        // idxMin.
        // idxMax: the index of rightmost largest element. The distance to end is
        // N - idxMax -1, where N is the length of nums array.
        //
        // The minimum swaps needed is:
        // if idxMin < idxMax, idxMin + (N - idxMax - 1)
        // if idxMin > idxMax, idxMin + (N - idxMax - 1) - 1

        const size_t len = nums.size();
        // How to find idxMin / idxMax in one pass??
        size_t idxMin = 0;
        size_t idxMax = len - 1;

        for (int i = 1; i < len; i++)
        {
            // Deal with idxMin.
            if (nums[idxMin] > nums[i])
            {
                // Move right.
                idxMin = i;
            }

            // Deal with idxMax.
            if (nums[idxMax] < nums[len - i - 1])
            {
                idxMax = len - i - 1;
            }
        }

        // Beware of edge case : [9]. idxMin == idxMax.
        return ( idxMin <= idxMax ? 0 : -1 ) + static_cast<int>(idxMin + ( len - idxMax - 1 ));
    }

    //-----------------------------------------------------------------------------
    // Test function
    //-----------------------------------------------------------------------------
    void TestArrayAndString()
    {
        // 12. Integer to Roman (Medium)
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

        // 165. Compare Version Numbers (Medium)
        //Input: version1 = "1.01", version2 = "1.001"
        // Output : 0
        // Input: version1 = "0.1", version2 = "1.1"
        // Output: -1
        cout << "\n165. Compare Version Numbers: " << compareVersion("1.2.3", "1.2.1");

        // 819. Most Common Word (Easy)
        // Input: paragraph = "Bob hit a ball, the hit BALL flew far after it was hit.", banned = ["hit"]
        // Output : "ball"
        vector<string> words{ "hit" };
        cout << "\n819. Most Common Word: " << mostCommonWord("Bob hit a ball, the hit BALL flew far after it was hit.", words) << endl;

        // 937. Reorder Data in Log Files (Medium)
        // Input: logs = ["dig1 8 1 5 1","let1 art can","dig2 3 6","let2 own kit dig","let3 art zero"]
        // Output: ["let1 art can", "let3 art zero", "let2 own kit dig", "dig1 8 1 5 1", "dig2 3 6"]
        words = { "dig1 8 1 5 1","let1 art can","dig2 3 6","let2 own kit dig","let3 art zero" };
        cout << "\n937. Reorder Data in Log Files: " << endl;
        auto resultVS =reorderLogFiles(words);
        LeetCodeUtil::PrintVector(resultVS);

        // 1099. Two Sum Less Than K (Easy)
        // Input: nums = [34,23,1,24,75,33,54,8], k = 60
        // Output: 58
        // Input: nums = [1, 2, 4, 5], k = 6
        // Output:  5
        inputVI = { 34,23,1,24,75,33,54,8 };
        cout << "\n1099. Two Sum Less Than K: " << twoSumLessThanK(inputVI, 60) << endl;

        // 907. Sum of Subarray Minimums (Medium)
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

        // 2281. Sum of Total Strength of Wizards (Hard)
        // Input: strength = [1,3,1,2]
        // Output: 44
        // Input: strength = [1, 2, 3]
        // Output: 33
        Solution2281 sol2281;
        inputVI = { 1932,1660,828,290,1808,446,1107,1843,913,856,503,535,263,704,13,1188,1391,706,861,867,262,304,1599,732,1365,1244,8,1936,642,1003,844,1539,619,135,1199,730,1301,1701,1959,243,1598,1498,346,1629,1739,1612,862,1857,1137,1728,57,233,1369,1016,1709,955,1258,705,1025,1521,1529,1161,1387,1220,667,299,395,1518,1959,873,472,1870,1266,1105,887,1471,1484,614,1762,46,1054,1555,939,287,1457,1934,1181,1592,434,628,344,995,1225,1028,706,621,881,881,100,1064,1410,335,70,1143,558,275,1635,1227,981,1360,31,1097,1770,648,336,523,937,283,1220,1964,115,1908,1554,831,1437,193,751,177,1562,584,1268,1810,451,685,1206,1406,1918,925,965,1221,1073,50,329,256,778,1404,1108,540,247,1485,1795,1131,1646,1532,1071,615,1764,922,1348,240,1547,1865,648,1434,1774,500,953,1840,1096,1619,1444,295,114,1392,1708,1023,1139,25,554,931,33,543,936,1907,1869,1376,705,970,1448,1085,590,499,718,1623,237,625,15,1717,765,161,1054,939,1120,264,1688,586,195,564,1737,1644,1219,693,697,222,1938,398,1033,1570,148,1764,724,1066,427,1151,120,853,605,406,1125,723,195,472,1832,804,274,895,221,528,1461,1598,110,1322,356,648,1768,1527,1443,349,1454,100,461,1410,1361,1318,411,493,781,548,1663,109,156,532,1642,1886,606,1161,743,1935,1525,1561,1892,1726,1418,1328,420,1699,83,891,1926,1225,1531,1060,315,632,1927,193,317,405,90,659,801,321,1315,113,298,408,1471,1308,1687,1192,170,1440,1558,1658,726,1405,343,1251,1324,122,1482,1790,1479,1649,850,1579,688,1297,1047,283,1860,554,941,1322,1242,1076,188,1824,324,1673,551,716,1611,1068,1964,1454,504,1030,45,1526,386,1797,831,525,822,688,872,1214,516,459,500,357,1412,98,1773,1043,15,416,1065,1154,45,1499,1927,1182,7,1270,1996,184,826,1586,678,1176,1697,1461,387,1942,1213,791,1414,1684,1514,660,835,1375,371,1232,1195,546,716,1832,1248,1375,513,1681,1422,1527,352,884,1595,1348,1196,693,1990,165,1830,1537,711,212,159,813,212,557,1843,215,1619,95,853,424,375,1040,1195,820,443,481,1310,1167,623,528,535,782,287,1972,1748,504,709,675,1005,136,285,106,143,659,1882,666,1929,532,1858,1532,1828,1078,1846,1886,1531,1945,1036,154,1022,390,1388,247,26,1414,1012,1656,538,449,1352,1106,680,1706,1995,1882,1220,1269,1684,1098,1184,1553,1464,136,1489,667,65,23,1711,701,1540,1173,1922,744,968,499,843,971,40,157,670,735,566,717,552,55,39,1597,450,1276,603,1430,831,1644,1277,1250,1710,905,1521,688,248,830,1720,734,420,475,29,1736,194,1470,1610,1264,3,1155,8,589,1361,1374,1462,483,189,1955,1241,1130,1759,681,1412,556,466,100,611,329,1492,1598,823,789,973,1095,1175,1089,1609,875,1142,490,1800,1516,1261,132,1378,586,533,229,153,1112,1755,1042,799,69,1504,1991,379,1066,1064,778,481,388,1936,1590,541,1318,937,832,11,1929,280,1888,1579,1974,1844,381,421,1478,1307,1744,1036,324,1468,196,1608,287,168,1726,1774,427,1730,1597,1727,747,1851,1006,1099,519,1676,1477,1159,1739,1185,890,511,94,1724,1690,1470,1175,583,1754,811,1888,321,1373,347,1389,1547,776,799,1579,1620,1387,668,937,547,871,83,1238,1908,1035,1005,1542,163,101,760,426,834,353,1178,1461,691,1256,643,1679,1366,463,1383,1655,82,896,332,134,1756,1598,1878,1977,439,94,1125,1323,1948,945,264,1250,726,1866,1764,384,1358,184,751,1287,192,220,150,1404,1649,1401,1794,1469,1703,688,1581,1112,798,1139,307,660,832,10,968,658 };
        cout << "\n2281. Sum of Total Strength of Wizards: " << sol2281.totalStrength(inputVI) << endl;

        // 1567. Maximum Length of Subarray With Positive Product (Medium)
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

        // 1151. Minimum Swaps to Group All 1's Together (Medium)
        // Input: data = [1,0,1,0,1]
        // Output: 1
        inputVI = { 1,0,1,0,1,0,0,1,1,0,1 };
        cout << "\n1151. Minimum Swaps to Group All 1's Together: " << minSwaps(inputVI) << endl;

        // 370. Range Addition (Medium)
        // Input: length = 5, updates = [[1, 3, 2], [2, 4, 3], [0, 2, -2]]
        // Output : [-2, 0, 3, 5, 3]
        vector<vector<int>> inputVVI;
        LeetCodeUtil::BuildIntMatrixFromString("[[1, 3, 2], [2, 4, 3], [0, 2, -2]]", &inputVVI);
        auto resultVI = getModifiedArray(5, inputVVI);
        cout << "\n370. Range Addition: " << endl;
        LeetCodeUtil::PrintVector(resultVI);

        // 2214. Minimum Health to Beat Game (Medium)
        // Input: damage = [2,7,4,3], armor = 4
        // Output: 13
        cout << "\n2214. Minimum Health to Beat Game: " << minimumHealth({ 2,7,4,3 }, 4) << endl;

        // 735. Asteroid Collision (Medium)
        // Input: asteroids = [10,2,-5]
        // Output: [10]
        // Input: asteroids = [-2, -1, 1, 2]
        // Output:  [-2,-1,1,2]
        cout << "\n735. Asteroid Collision: " << endl;
        resultVI = asteroidCollision({ -2,-2,1,-2 });
        LeetCodeUtil::PrintVector(resultVI);

        // 2193. Minimum Number of Moves to Make Palindrome (Hard)
        // Input: s = "aabb"
        // Output: 2
        cout << "\n2193. Minimum Number of Moves to Make Palindrome: " <<
            minMovesToMakePalindrome("aabb") << endl;

        // 767. Reorganize String (Medium)
        // Input: s = "aaab"
        // Output : ""
        cout << "\n767. Reorganize String: " << reorganizeString("aab") << endl;

        // 1291. Sequential Digits (Medium)
        // Input: low = 1000, high = 13000
        // Output: [1234, 2345, 3456, 4567, 5678, 6789, 12345]
        cout << "\n1291. Sequential Digits: " << endl;
        resultVI = sequentialDigits(1000, 13000);
        LeetCodeUtil::PrintVector(resultVI);

        // 1856. Maximum Subarray Min-Product (Medium)
        // Input: nums = [1,2,3,2]
        // Output: 14
        Solution1856 sol1856;
        cout << "\n1856. Maximum Subarray Min-Product: " << sol1856.maxSumMinProduct({ 3,1,5,6,4,2 }) << endl;

        // 239. Sliding Window Maximum (Hard)
        // Input: nums = [1,3,-1,-3,5,3,6,7], k = 3
        // Output: [3, 3, 5, 5, 6, 7]
        // Input: nums = [-7, -8, 7, 5, 7, 1, 6, 0], k = 4
        // Output: [7,7,7,7,7]
        cout << "\n239. Sliding Window Maximum: " << endl;
        resultVI = maxSlidingWindow({ -7, -8, 7, 5, 7, 1, 6, 0 }, 4);
        LeetCodeUtil::PrintVector(resultVI);

        // 134. Gas Station (Medium)
        // Input: gas = [1,2,3,4,5], cost = [3,4,5,1,2]
        // Output: 3
        // Input: gas = [2,3,4], cost = [3,4,3]
        // Output: -1
        cout << "\n134. Gas Station: " << canCompleteCircuit({ 2,3,4 }, { 3,4,3 }) << endl;

        // 560. Subarray Sum Equals K (Medium)
        // Input: nums = [1, 2, 3], k = 3
        // Output : 2
        cout << "\n560. Subarray Sum Equals K: " << subarraySum({ 1, 2, 3 }, 3) << endl;

        // 2221. Find Triangular Sum of an Array (Medium)
        inputVI = { 1,2,3,4,5 };
        cout << "\n2221. Find Triangular Sum of an Array: " << triangularSum(inputVI) << endl;

        // 135. Candy (Hard)
        // Input: ratings = [1,2,2]
        // Output: 4
        cout << "\n135. Candy: " << candy({ 1,2,2 }) << endl;

        // 564. Find the Closest Palindrome (Hard)
        // Input: n = "123"
        // Output: "121"
        cout << "\n564. Find the Closest Palindrome: " << nearestPalindromic("2147483647") << endl;

        // 1209. Remove All Adjacent Duplicates in String II (Medium)
        // Input: s = "deeedbbcccbdaa", k = 3
        // Output: "aa
        cout << "\n1209. Remove All Adjacent Duplicates in String II: " << removeDuplicates("deeedbbcccbdaa", 3) << endl;

        // 2340. Minimum Adjacent Swaps to Make a Valid Array (Medium)
        // Input: nums = [3,4,5,5,3,1]
        // Output: 6
        inputVI = { 3,4,5,5,3,1 };
        cout << "\n2340. Minimum Adjacent Swaps to Make a Valid Array: " << minimumSwaps(inputVI) << endl;
    }
}