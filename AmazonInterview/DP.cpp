#include "DP.h"
#include "LeetCodeUtil.h"

#include <string>
#include <unordered_set>

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
            const int len = s.size();

            // It is important to fill with -1. For example, for AB, dp should be 3 as 3 = 1 + (i - (-1)) - (-1 - -1)
            // => 3 = 1 + (i - (-1)) - (0) => 3 = 1 + 1 + 1.
            vector<int> firstAppearence(26, -1);
            vector<int> secondAppearence(26, -1);

            int dp = 1;
            firstAppearence[s[0] - 'A'] = 0;
            int result = dp;

            for (int i = 1; i < len; ++i)
            {
                const char c = s[i];
                dp = dp + ( i - firstAppearence[c - 'A'] ) - ( firstAppearence[c - 'A'] - secondAppearence[c - 'A'] );

                result += dp;

                // Update the index tables.
                secondAppearence[c - 'A'] = firstAppearence[c - 'A'];
                firstAppearence[c - 'A'] = i;
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
                const int len = static_cast<int>( word.size() );

                // dp[i] : whether string[1~i] is a word in wordSet.
                // dp[j] = dp[i] && string[i + 1, j - i + 1] is a word in wordSet.
                vector<bool> dp(len + 1, false);
                dp[0] = true;

                for (int i = 0; i < len; ++i)
                {
                    if (!dp[i])
                    {
                        // When i is 0, we already marked the next eligible starting positions,
                        // we can skip those positions that is not eligible.
                        // The eligible position j means string[0 ~ j - 1] is word.
                        // For example, for 'catsdogcats'. word = [cat, cats]
                        //                      ^^
                        //                      34  dp[3] and dp[4] is true.
                        continue;
                    }

                    for (int j = i + 1; j <= len; j++)
                    {
                        string subStr = word.substr(i, j - i);
                        if (j - i < len && wordSet.count(subStr) > 0)
                        {
                            // We block the case 'j - i == len' because the entire word itself is not
                            // considered as 'concatenated'.
                            dp[j] = true;
                        }
                    }
                }

                if (dp[len])
                {
                    result.push_back(word);
                }
            }

            return result;
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
            const int len = s.size();
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
        auto resultVS = sol472.findAllConcatenatedWordsInADict_recursive(inputVS);
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
    }
}