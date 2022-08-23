#include "DP.h"
#include "LeetCodeUtil.h"

#include <string>
#include <unordered_set>
#include <queue>
#include <stack>

namespace DP
{
    using namespace std;

    //-----------------------------------------------------------------------------
    // 828. Count Unique Characters of All Substrings of a Given String (Hard)
    // This is very hard.
    //-----------------------------------------------------------------------------
    class Solution828
    {
    public:
        int uniqueLetterString(string s)
        {
            // Denote dp[i] as the number of unique characters in the substrings
            // which ends with s[i].
            // So, the answer is sum of all dp[i] for i = 0 ~ n - 1 where n is the
            // length of string.
            // s: REPEATCODE
            //
            // i = 9             E + 1
            //               D | E + 1
            //              OD | E + 1
            //             COD | E + 1
            //            TCOD | E + 1
            //           ATCOD | E + 1
            //  j = 3   EATCOD | E - 1 ( since leftmost E is now not unique anymore )
            //         PEATCOD | E - 1
            //  k = 1 EPEATCOD | E  0 ( leftmost E was already non - unique )
            //       REPEATCOD | E  0
            // dp[i] = dp[i-1] + (i-j) - (j-k)
            const size_t len = s.size();

            // It is important to fill with -1. For example, for AB, dp should be 3 as 3 = 1 + (i - (-1)) - (-1 - -1)
            // => 3 = 1 + (i - (-1)) - (0) => 3 = 1 + 1 + 1.
            vector<int> firstOccurrence(26, -1);
            vector<int> secondOccurrence(26, -1);

            int dp = 1;
            firstOccurrence[s[0] - 'A'] = 0;
            int result = dp;

            for (int i = 1; i < len; ++i)
            {
                const char& c = s[i];
                dp = dp + ( i - firstOccurrence[c - 'A'] ) - ( firstOccurrence[c - 'A'] - secondOccurrence[c - 'A'] );

                result += dp;

                // Update the index tables.
                secondOccurrence[c - 'A'] = firstOccurrence[c - 'A'];
                firstOccurrence[c - 'A'] = i;
            }

            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 2272. Substring With Largest Variance
    //-----------------------------------------------------------------------------
    class Solution2272
    {
    public:
        // This is related to Maximum subarray problem. There is a
        // 'Kadane's algorithm' for it.
        int largestVariance(string s)
        {
            int result = 0;
            unordered_set<char> availalbeChar(s.begin(), s.end());

            // iterate every two chars combinations.
            for (auto itFreqLow = availalbeChar.begin(); itFreqLow != availalbeChar.end(); itFreqLow++)
            {
                for (auto itFreqHigh = availalbeChar.begin(); itFreqHigh != availalbeChar.end(); itFreqHigh++)
                {
                    const char lowFreqChar = *itFreqLow;
                    const char highFreqChar = *itFreqHigh;
                    // Skip when they are the same.
                    if (lowFreqChar == highFreqChar)
                    {
                        continue;
                    }

                    int freqHigh = 0;
                    int freqLow = 0;
                    bool countPrevLowFreqChar = false;

                    for (const auto& c : s)
                    {
                        if (c == lowFreqChar) freqLow++;
                        if (c == highFreqChar) freqHigh++;

                        // This question demands at least one char for the low frequency.
                        // For example, for 'aaa', the answer is 0, not 3.
                        if (freqLow > 0)
                        {
                            result = max(result, freqHigh - freqLow);
                        }
                        else if(countPrevLowFreqChar)
                        {
                            // b|baaaaa
                            //   ^       start over at b.
                            //    ^^^^^  Meet a.
                            // We can consider 'baaaaa'. The diff is 5 - 1 = 3. The 1st b can be discarded.
                            result = max(result, freqHigh - 1);
                        }

                        if (freqLow > freqHigh)
                        {
                            // Start over.
                            freqLow = 0;
                            freqHigh = 0;
                            // bbaaaaa
                            //     ^  Meet a.
                            countPrevLowFreqChar = true;
                        }
                    }

                }
            }

            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 472. Concatenated Words
    // Topic: DP, Trie, DFS
    // Given an array of strings words (without duplicates), return all the
    // concatenated words in the given list of words.
    //
    // Similar questions: Word Break, Word Break II
    //-----------------------------------------------------------------------------
    class Solution472
    {
    public:
        vector<string> findAllConcatenatedWordsInADict(vector<string>& words)
        {
            unordered_set<string> wordSet(words.begin(), words.end());
            vector<string> result;

            for (const auto& word : words)
            {
                if (wordBreak(wordSet, word))
                {
                    result.push_back(word);
                }
            }

            return result;
        }

        bool wordBreak(unordered_set<string>& wordSet, const string& word)
        {
            const size_t len = static_cast<int>( word.size() );
            //  applepenapple
            // ^    ^  ^
            //      j  i
            // dp[i] : true is word[1~i] can be concatenated.
            // dp[i] = dp[j] && substr(j, i - j) is word. j < i.
            vector<bool> dp(len + 1, false);
            dp[0] = true;

            for (size_t i = 1; i <= len; ++i)
            {
                for (int j = static_cast<int>( i - 1 ); j >= 0; --j)
                {
                    // Note that the input word is also in the wordSet.
                    // This is a tricky to avoid comparing word to itself in wordSet.
                    // A word is considered as concatenated if it can be built from at least two words.
                    if (i - j == len)
                    {
                        break;
                    }

                    if (dp[j] && wordSet.count(word.substr(j, i - j)) > 0)
                    {
                        dp[i] = true;
                        break;
                    }
                }
            }

            return dp.back();
        }

        // Use recursion
        vector<string> findAllConcatenatedWordsInADict_recursive(vector<string>& words)
        {
            unordered_set<string> wordSet(words.begin(), words.end());
            vector<string> resultSet;

            for (const auto& word : words)
            {
                if (helper(wordSet, word, 0, 0))
                {
                    resultSet.push_back(word);
                }
            }

            return resultSet;
        }

        bool helper(const unordered_set<string>& wordSet, const string& word, int startPos, int matchedCount)
        {
            bool result = false;
            if (startPos == word.length() && matchedCount > 1)
            {
                return true;
            }

            const int len = static_cast<int>( word.size() );

            for (int i = 1; i <= len - startPos; ++i)
            {
                string subStr = word.substr(startPos, i);
                if (wordSet.count(subStr) > 0)
                {
                    if (helper(wordSet, word, startPos + i, matchedCount + 1))
                    {
                        result = true;
                        break;
                    }
                }
            }

            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 926. Flip String to Monotone Increasing
    // Topic: DP
    //-----------------------------------------------------------------------------
    class Solution926
    {
    public:
        int minFlipsMonoIncr(string s)
        {
            // dp[i] : mini count of flipping required for s[0~i].
            // dp[i+1] : if s[i+1] is '1'. Don't need to flip.
            //           if s[i+1] is '0'. We can flip it to 1, or we can flip all previous 1 to 0.
            //                             So, we must know how much 1 we have met.
            //                             We pick minimum for the above two cases.
            // Because dp[i+1] only relies on the previous dp[i], so we can optimize the space.
            // We just need one variable instead of an array.
            const size_t len = s.size();
            int miniFlipRequired = 0;
            int countOfOne = 0;

            // Be careful about the first 2 letters...
            for (int i = 0; i < len; ++i)
            {
                if (s[i] == '1')
                {
                    countOfOne++;
                }
                else
                {
                    miniFlipRequired++;
                }

                miniFlipRequired = min(miniFlipRequired, countOfOne);
            }

            return miniFlipRequired;
        }
    };

    //-----------------------------------------------------------------------------
    // 2262. Total Appeal of A String (Hard)
    // Topic: DP
    //-----------------------------------------------------------------------------
    class Solution2262
    {
    public:
        long long appealSum(const string& inputString)
        {
            // "a b b c a"   a b c a b
            //  0 1 2 3 4      1     4
            // 4 - 0  = 4      4 - 1 = 3
            vector<int> dp(inputString.size(), 0);
            dp[0] = 1;
            vector<int> charTable(26, -1);
            charTable[inputString[0] - 'a'] = 0;

            long long result = 1;
            for (int i = 1; i < inputString.size(); i++)
            {
                dp[i] = dp[i - 1] + i - charTable[inputString[i] - 'a'];
                charTable[inputString[i] - 'a'] = i;
                result += dp[i];
            }

            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 2222. Number of Ways to Select Buildings (Medium)
    //-----------------------------------------------------------------------------
    long long numberOfWays(const string& s)
    {
        #if(USE_DP)
        // Method 1. Use Dynamic programming.
        // Make dp[len][ch] demote the number of alternating strings of length of
        // 'len' and ending with character - 'ch'.
        // The eligible string could end with 0 or 1, so the answer is
        // dp[3][0] + dp[3][1].

        long long dp[4][2] = {};

        // dp[0] means the length of string is 0. It is meaningless. Set to 1 to
        // facilitate the further calculations.
        dp[0][0] = 1;
        dp[0][1] = 1;

        for (int i = 0; i < s.size(); ++i)
        {
            for (int len = 1; len <= 3; ++len)
            {
                if (s[i] == '0')
                {
                    dp[len][0] += dp[len - 1][1];
                }
                else
                {
                    dp[len][1] += dp[len - 1][0];
                }
            }
        }

        return dp[3][0] + dp[3][1];

        #else

        // Method 2:
        // The eligible ways are 010 or 101.
        // When we encounter a '1', the number of ways is x * y, where x is the
        // number of '0' to the left and y is the number 0 to the right.
        // Similarly, when we encounter a '0', the number of ways can be calculated.

        // Firstly, count all '1' and '0'.
        size_t numberOfOne = 0;
        size_t numberOfZero = 0;
        for (const auto& c : s)
        {
            if (c == '1')
            {
                numberOfOne++;
            }
            else
            {
                numberOfZero++;
            }
        }

        long long result = 0;
        size_t numberOfOneOnLeft = 0;
        size_t numberOfZeroOnLeft = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            if (s[i] == '1')
            {
                result += numberOfZeroOnLeft * ( numberOfZero - numberOfZeroOnLeft );
                numberOfOneOnLeft++;
            }
            else
            {
                result += numberOfOneOnLeft * ( numberOfOne - numberOfOneOnLeft );
                numberOfZeroOnLeft++;
            }
        }

        return result;
        #endif
    }

    //-----------------------------------------------------------------------------
    // 871. Minimum Number of Refueling Stops (Hard)
    //
    // Topic: Dynamic programming, Heap (max heap), greedy.
    //-----------------------------------------------------------------------------
    int minRefuelStops(int target, int startFuel, const vector<vector<int>>& stations)
    {
        // This can be solved by DP or max heap.
        #define HEAP_GREEDY 1
        #define DP_GREEDY 0

        #if( DP_GREEDY )
        // It is not easy to define the DP equation.
        // Let dp[i] be the max reachable distance after the i-th refuel.
        // We want the first dp[i] that reaches the target.
        // Then, we have iterate all stations. At station k, we may reach it if
        // dp[i] >= k's position (stations[k][0])
        // If we can reach, we iterate for all i (0 <= i <= k) backwardly to update
        // dp[i + 1] = max(dp[i + 1], dp[i] + stations[k][1]);
        const size_t n = stations.size();
        vector<size_t> dp(n + 1, startFuel);

        for (size_t k = 0; k < n; ++k)
        {
            // We are at k, so we at most have refueled k times.
            for (int i = static_cast<int>(k); i >= 0 && dp[i] >= stations[k][0]; i--)
            {
                dp[i + 1] = max(dp[i + 1], dp[i] + stations[k][1]);
            }
        }

        // Until we iterated all k, we don't know the final result of dp.
        for (int j = 0; j < dp.size(); j++)
        {
            if (dp[j] >= target)
            {
                return j;
            }
        }

        return -1;
        #elif(HEAP_GREEDY)

        // The idea of heap + greedy is that we always pick the station having most of fuel to refuel.
        // The most tricky part is that we may choose station backwardly while the car is not allowed
        // to travel backwardly.
        // For example, we may pick station 3 from stack and pick station 2. It actually means that we will
        // refuel in station 2 and 3.

        int result = 0;
        int lastStation = 0;
        const size_t n = stations.size();
        priority_queue<int> pq;
        for (; startFuel < target; ++result)
        {
            while (lastStation < n && stations[lastStation][0] <= startFuel)
            {
                pq.push(stations[lastStation][1]);
                lastStation++;
            }

            if (pq.empty())
                return -1;

            startFuel += pq.top();
            pq.pop();
        }

        return result;

        #endif
    }

    //-----------------------------------------------------------------------------
    // 2355. Maximum Number of Books You Can Take (Hard)
    //
    // Topic: Dynamic programming, monotonic stack.
    //-----------------------------------------------------------------------------
    class Solution2355
    {
    public:

        long long maximumBooks(const vector<int>& b)
        {
            #define DP 1

            #if(DP)

            // Let dp[i] be the maximum number of books we can take from bookshelves 0 ~ i
            // and we must take all books from the bookshelf i.
            // Use a monotonic increasing stack to store the valid contiguous section.
            // For example, [48, 100, 51, 52]. When we reach i = 3, the stack will be
            // [0, 2]. It means that we can take from bookshelf 2, 1, 0. We will take
            // 51, 50, 48.
            // 51 + 50 is calculated from the formula of sum of arithmetic sequence.
            // 48 is from dp[0].
            // Note that [3, 5, 4] won't form a valid section because we cannot take 3 books
            // from bookshelf 0. We can only take 2. So the index of 3 and 5 must be removed
            // from the stack.
            const size_t len = b.size();
            vector<long long> dp(len);
            stack<int> monotonicStack;

            for (int i = 0; i < len; ++i)
            {
                int topIndex = monotonicStack.empty() ? -1 : monotonicStack.top();

                while (!monotonicStack.empty() && b[i] - b[monotonicStack.top()] < i - monotonicStack.top())
                {
                    monotonicStack.pop();
                    topIndex = monotonicStack.empty() ? -1 : monotonicStack.top();
                }

                dp[i] = ssum(b[i], topIndex == -1 ? i + 1 : i - topIndex);

                if (topIndex != -1)
                {
                    dp[i] += dp[topIndex];
                }

                monotonicStack.push(i);
            }

            return *max_element(dp.begin(), dp.end());

            #else

            vector<int> ms;
            long long cur = 0, res = 0;
            for (int i = 0; i < b.size(); ++i)
            {
                while (!ms.empty() && b[i] - b[ms.back()] < i - ms.back())
                {
                    int j = ms.back(); ms.pop_back();
                    cur -= ssum(b[j], ms.empty() ? j + 1 : j - ms.back());
                }

                cur += ssum(b[i], ms.empty() ? i + 1 : i - ms.back());
                ms.push_back(i);
                res = max(cur, res);
            }
            return res;

            #endif
        }

    private:
        // Get sum of arithmetic sequence
        long long ssum(long long maxValue, int count)
        {
            if (count > maxValue)
            {
                // count = 100, maxValue = 3.
                // Accumulate 1 ~ 3:
                return ( maxValue + 1 ) * maxValue / 2;
            }
            else
            {
                // count = 3, maxValue = 100.
                // Accumulate 100 ~ 98:
                return ( maxValue + ( maxValue - count + 1 ) ) * count / 2;
            }
        }

    };


    //-----------------------------------------------------------------------------
    // Test function
    //-----------------------------------------------------------------------------
    void TestDP()
    {
        // 828. Count Unique Characters of All Substrings of a Given String (Hard)
        // Input: s = "ABA"
        // Output: 8
        // Input: s = "LEETCODE"
        // Output : 92
        Solution828 sol828;
        cout << "\n828. Count Unique Characters of All Substrings of a Given String: " <<
            sol828.uniqueLetterString("ABA") << endl;


        // 2272. Substring With Largest Variance (Hard)
        // Input: s = "aababbb"
        // Output: 3
        Solution2272 sol2272;
        cout << "\n2272. Substring With Largest Variance: " << sol2272.largestVariance("bbaaaaa") << endl;

        // 472. Concatenated Words (Hard)
        // Input: words = ["cat","cats","catsdogcats","dog","dogcatsdog","hippopotamuses","rat","ratcatdogcat"]
        // Output: ["catsdogcats", "dogcatsdog", "ratcatdogcat"]
        Solution472 sol472;
        vector<string> inputVS = { "cat","cats","catsdogcats","dog","dogcatsdog","hippopotamuses","rat","ratcatdogcat" };
        auto resultVS = sol472.findAllConcatenatedWordsInADict(inputVS);
        cout << "\n472. Concatenated Words: " << endl;
        LeetCodeUtil::PrintVector(resultVS);

        // 926. Flip String to Monotone Increasing (Medium)
        // Input: s = "00011000"
        // Output: 2
        // Input: s = "010110"
        // Output: 2
        Solution926 sol926;
        cout << "\n926. Flip String to Monotone Increasing: " << sol926.minFlipsMonoIncr("00011000") << endl;

        // 2262. Total Appeal of A String (Hard)
        Solution2262 sol2262;
        cout << "\n2262. Total Appeal of A String: " << sol2262.appealSum("code") << endl;

        // 2222. Number of Ways to Select Buildings (Medium)
        // Input: s = "001101"
        // Output: 6
        cout << "\n2222. Number of Ways to Select Buildings: " << numberOfWays("001101") << endl;

        // 871. Minimum Number of Refueling Stops (Hard)
        // Input: target = 100, startFuel = 10, stations = [[10,60],[20,30],[30,30],[60,40]]
        // Output: 2
        // Input: target = 100, startFuel = 1, stations = [[10,100]]
        // Output: -1
        // Input: target = 1, startFuel = 1, stations = []
        // Output: 0
        // 1000, 299
        // [[13, 21], [26, 115], [100, 47], [225, 99], [299, 141], [444, 198], [608, 190], [636, 157], [647, 255], [841, 123]]
        //vector<vector<int>> inputVVI = { {} };
        vector<vector<int>> inputVVI;
        LeetCodeUtil::BuildIntMatrixFromString("[[13, 21], [26, 115], [100, 47], [225, 99], [299, 141], [444, 198], [608, 190], [636, 157], [647, 255], [841, 123]]", &inputVVI);
        cout << "\n871. Minimum Number of Refueling Stops: " << minRefuelStops(1000, 299, inputVVI) << endl;

        // 2355. Maximum Number of Books You Can Take (Hard)
        Solution2355 sol2355;
        // 48, 100, 51, 10
        cout << "\n2355. Maximum Number of Books You Can Take: " << sol2355.maximumBooks({ 48, 100, 51 }) << endl;
    }
}