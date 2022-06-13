#include "LeetCodeUtil.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

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
            int right = nums.size() - 1;

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
    // 165. Compare Version Numbers
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
    // 819. Most Common Word
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
    // 937. Reorder Data in Log Files
    //-----------------------------------------------------------------------------
    vector<string> reorderLogFiles(vector<string>& logs)
    {
        using LetterLog = pair<string, string>;
        deque<LetterLog> letterLogs;
        vector<string> digitLogs;

        for (const auto& str : logs)
        {
            int idx = str.find_first_of(' ');
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
    }
}