#include "SortingAndSearching.h"
#include "LeetCodeUtil.h"

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>

namespace SortingAndSearching
{
    using namespace std;
    using namespace LeetCodeUtil;


    //-----------------------------------------------------------------------------
    // 973. K Closest Points to Origin
    //-----------------------------------------------------------------------------

    class Solution973
    {
    public:
        vector<vector<int>> kClosest(vector<vector<int>>& points, int k)
        {
            sort(points.begin(), points.end(), [](const vector<int>& a, const vector<int>& b)
                {
                    return a[0] * a[0] + a[1] * a[1] < b[0] * b[0] + b[1] * b[1];
                }
            );

            return vector<vector<int>>(points.begin(), points.begin() + k);
        }
    };

    //-----------------------------------------------------------------------------
    // 1152. Analyze User Website Visit Pattern
    //
    // A pattern is a list of three websites (not necessarily distinct).
    //
    // If there is more than one pattern with the same largest score, return the
    // lexicographically smallest such pattern.
    //-----------------------------------------------------------------------------
    class Solution1152
    {
    public:
        vector<string> mostVisitedPattern(vector<string>& username, vector<int>& timestamp, vector<string>& website)
        {
            // < username : < timestamp : website > >
            map<string, map<int, string>> userData;
            // <pattern, count of occurance of this pattern>
            map<vector<string>, int> patternCountMap;
            const int len = static_cast<int>(username.size());
            const int maxPatternLen = 3;

            // The user is not guaranteed to be sorted, we need to iterate over the entire vector.
            for (int i = 0; i < len; ++i)
            {
                userData[username[i]].insert(make_pair(timestamp[i], website[i]));
            }

            string prevUser = userData.begin()->first;
            const int userCount = static_cast<int>(userData.size());
            int maxCountOfPattern = 0;
            vector<string> mostOccurredPattern;

            // We use '<=' in order to process the last user.
            for (int i = 0; i <= userData.size(); ++i)
            {
                auto it = userData.begin();
                std::advance(it, userCount == i ? userCount - 1 : i);
                const string& currUser = it->first;
                if (i == userData.size() || prevUser != currUser)
                {
                    // For the previous user, build the patterns.
                    auto& visitHistory = userData[prevUser];
                    vector<string> pattern;
                    for (const auto& log : visitHistory)
                    {
                        pattern.push_back(log.second);
                        if (pattern.size() == 3)
                        {
                            break;
                        }
                    }
                    patternCountMap[pattern]++;

                    // Keep track the pattern which occurs the most.
                    if (patternCountMap[pattern] > maxCountOfPattern)
                    {
                        maxCountOfPattern = patternCountMap[pattern];
                        // pattern is now useless, we could use swap to avoid coping.
                        mostOccurredPattern.swap(pattern);
                    }
                    prevUser = currUser;
                }
            }

            // If there is more than one pattern with the same largest score, return the lexicographically smallest such pattern.
            bool havingTheSameLargestScore =
                std::count_if(patternCountMap.begin(), patternCountMap.end(), [&](const pair<vector<string>, int>& obj)
                {
                    return maxCountOfPattern == obj.second;
                }) > 1;

            if (havingTheSameLargestScore)
            {
                auto itt = std::find_if(patternCountMap.begin(),
                                        patternCountMap.end(),
                                        [&](const pair<vector<string>, int>& obj)
                    {
                        return maxCountOfPattern == obj.second;
                    });

                mostOccurredPattern = itt->first;
            }

            return mostOccurredPattern;
        }
    };


    //-----------------------------------------------------------------------------
    // Test function.
    //-----------------------------------------------------------------------------
    void TestSortingAndSearching()
    {
        // 973. K Closest Points to Origin
        // Input: points = [[1,3],[-2,2]], k = 1
        // Output: [[-2, 2]]
        // Input: points = [[3,3],[5,-1],[-2,4]], k = 2
        // Output: [[3, 3], [-2, 4]]
        vector<vector<int>> inputMatrix;
        BuildIntMatrixFromString("[[3,3],[5,-1],[-2,4]]", &inputMatrix);
        Solution973 sol973;
        auto resultVVI = sol973.kClosest(inputMatrix, 2);
        cout << "\n973. K Closest Points to Origin: " << endl;
        PrintMatrix(resultVVI);

        // 1152. Analyze User Website Visit Pattern
        // Input: username = ["joe", "joe", "joe", "james", "james", "james", "james", "mary", "mary", "mary"],
        // timestamp = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
        // website = ["home", "about", "career", "home", "cart", "maps", "home", "home", "about", "career"]
        // Output : ["home", "about", "career"]

        // ["ua","ua","ua","ub","ub","ub"]
        // [1, 2, 3, 4, 5, 6]
        // ["a", "b", "c", "a", "b", "a"]
        // Output: ["a", "b", "a"]. Not ["a","b","c"] because if there is more than one pattern with the same
        // largest score, return the lexicographically smallest such pattern.

        // ["zkiikgv","zkiikgv","zkiikgv","zkiikgv"]
        // [436363475, 710406388, 386655081, 797150921]
        // ["wnaaxbfhxp", "mryxsjc", "oz", "wlarkzzqht"]
        // Output: ["oz","mryxsjc","wlarkzzqht"]

        Solution1152 sol1152;
        vector<string> inputVVS = { "zkiikgv","zkiikgv","zkiikgv","zkiikgv" };
        vector<int> inputVI = { 436363475, 710406388, 386655081, 797150921 };
        vector<string> website = { "wnaaxbfhxp", "mryxsjc", "oz", "wlarkzzqht" };
        auto resultVS = sol1152.mostVisitedPattern
        (
            inputVVS,
            inputVI,
            website
        );
        cout << "\n1152. Analyze User Website Visit Pattern: " << endl;
        PrintVector(resultVS);
    }
}