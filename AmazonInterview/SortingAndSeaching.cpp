#include "SortingAndSearching.h"
#include "LeetCodeUtil.h"

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <string_view> // C++17
#include <queue>

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
    // 1152. Analyze User Website Visit Pattern (Medium)
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
    // 1710. Maximum Units on a Truck (Easy)
    //-----------------------------------------------------------------------------
    class Solution1710
    {
    public:
        int maximumUnits(vector<vector<int>>& boxTypes, int truckSize)
        {
            // Idea: We should choose the box that can contains most units
            // In other words, choose the biggest box.
            // Sort the box data by its capacity in descending order.
            sort(boxTypes.begin(), boxTypes.end(), [](const vector<int>& left, const vector<int>& right)
                {
                    return left[1] > right[1];
                }
            );

            int result = 0;
            for (const auto& currBox : boxTypes)
            {
                const int vol = min(truckSize, currBox[0]);
                result += ( vol * currBox[1] );
                truckSize -= vol;

                if (truckSize == 0)
                {
                    break;
                }
            }

            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 2102. Sequentially Ordinal Rank Tracker (Hard)
    //-----------------------------------------------------------------------------
    // The difficult part is how to minimize the adjustment of the iterator.
    // The naive approach will cause TLE on Leetcode.
    class SORTracker
    {
    public:

        struct Place
        {
            string name;
            int score;

            // Don't forget the 'const' in the end.
            bool operator<(const Place& other) const
            {
                if (score == other.score)
                {
                    return name < other.name;
                }
                else
                {
                    return score > other.score;
                }
            }

            Place(const string& aName, int aScore)
                : name(aName), score(aScore) {}
        };

    public:
        SORTracker() : placeStore(), iter(placeStore.begin())
        {
        }

        void add(string name, int score)
        {
            Place p(name, score);
            placeStore.insert(p);
            if (iter == placeStore.end() || p < *iter)
            {
                std::advance(iter, -1);
            }
        }

        string get()
        {
            auto& name = iter->name;
            iter++;
            return name;
        }

    private:

        set<Place> placeStore;
        set<Place>::iterator iter;
    };

    //-----------------------------------------------------------------------------
    // 1438. Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit (Medium)
    //-----------------------------------------------------------------------------
    int longestSubarray(vector<int>& nums, int limit)
    {
        // Maintain a sliding window. Stretch to the right. When it becomes
        // invalid, increment the left boundary. Keep calculating the max length.
        multiset<int> numSet;
        int j = 0;
        int result = 0;
        for (int i = 0; i < nums.size(); ++i)
        {
            numSet.insert(nums[i]);
            if (*numSet.rbegin() - *numSet.begin() > limit)
            {
                // Remove the element on the left boundary.
                numSet.erase(numSet.find(nums[j]));
                // Increment the left boundary.
                j++;
            }
            result = max(result, i - j + 1);
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 1044. Longest Duplicate Substring (Hard)
    //-----------------------------------------------------------------------------
    class Solution1044
    {
    public:
        string longestDupSubstring(string s)
        {
            // To get better performance, this question requires two techniques
            // 1. binary search on the length of the duplicate substring.
            // 2. A quick algorithm to check whether a duplicate substring with
            //    length k exists. Rabin¡VKarp algorithm is one solution.
            //    But it is not trivial, so the following code won't implement it.

            // To avoid string copy, the following code will use string_view, which
            // requires C++17.

            // The min length of a duplicate substring.
            size_t left = 1;
            size_t right = s.size() - 1;

            // To remember the longest.
            std::string_view longest;
            // Set to detect if a substring occurred before.
            unordered_set<string_view> subStrSet;
            // Use <= to handle the case left = right = 1.
            while (left <= right)
            {
                auto midLen = left + ( right - left ) / 2;
                bool found = false;
                for (size_t i = 0; i < s.size() - midLen + 1; ++i)
                {
                    auto [it, inserted] = subStrSet.emplace(s.data() + i, midLen);
                    if (!inserted)
                    {
                        found = true;
                        longest = *it;
                        break;
                    }
                }

                if (found)
                {
                    left = midLen + 1;
                }
                else
                {
                    right = midLen - 1;
                }

                subStrSet.clear();
            }

            string result(longest);
            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 1648. Sell Diminishing-Valued Colored Balls (Medium-Hard)
    // Topic: Greedy, Math, Binary search, Priority queue.
    //-----------------------------------------------------------------------------
    class Solution1648
    {
    public:
        int maxProfit(vector<int>& inventory, int orders)
        {
            // Pick the balls whose the count is max.
            // O O O  <--| Step 1. Sell (5+4) * 3 = 27
            // O O O  <--|
            // O O O O    <--| Step 2. Sell (3+2) * 4 = 20
            // O O O O    <--|
            // O O O O O      <-- Step 3. Sell 1 * 5 = 5

            // Sort in descending order. Takes O(nlog(n))
            std::sort(inventory.begin(), inventory.end(), std::greater<int>());

            long long valueCurrent = inventory[0];
            size_t indexNextLevel = 0;

            const int modulo = static_cast<int>(1e9 + 7);
            long long result = 0;
            size_t len = inventory.size();

            // Until we complete all orders (in number of the balls)
            while (orders > 0)
            {
                while (indexNextLevel < len && inventory[indexNextLevel] == valueCurrent)
                {
                    indexNextLevel++;
                }

                long long valueNextLevel = indexNextLevel == len ? 0 : inventory[indexNextLevel];
                size_t height = valueCurrent - valueNextLevel;

                // We now should take all balls in the rectangle (width * height), but we must
                // consider the case that the orders might be smaller.
                long long maxCountToTake = indexNextLevel * height;
                long long remainder = 0;
                if (orders < maxCountToTake)
                {
                    // Adjust the height and set the remainder.
                    height = orders / indexNextLevel;
                    remainder = orders % indexNextLevel;
                }

                // Apply Trapezoidal rule. Assume we need to take Q.
                // Q Q -> 5
                // Q Q -> 4
                // Q O -> 3
                // O O O
                // O O O O
                long long valueBottm = valueCurrent - height + 1; // 5 - 2 + 1 = 4.
                // indexNextLevel is the width.
                long long sum = indexNextLevel * ( valueBottm + valueCurrent ) * height / 2;
                // Consider the remainder.
                sum += remainder * ( valueBottm - 1 );

                result = ( result + sum ) % modulo;

                orders -= min(static_cast<long long>(orders), maxCountToTake);
                valueCurrent = valueNextLevel;
            }

            return static_cast<int>(result);
        }
    };

    //-----------------------------------------------------------------------------
    // 2055. Plates Between Candles (Medium)
    // Topic: Binary search, prefix sum.
    //-----------------------------------------------------------------------------
    vector<int> platesBetweenCandles(string s, vector<vector<int>>& queries)
    {
        const size_t len = s.size();
        // Note that the elements will be sorted in ascending order.
        vector<size_t> candleSet;
        for (int i = 0; i < len; ++i)
        {
            if (s[i] == '|')
            {
                candleSet.push_back(i);
            }
        }

        vector<int> result;
        for (const auto& range : queries)
        {
            int sum = 0;
            // Find the candle whose index is not less than range[0].
            auto it = std::lower_bound(candleSet.begin(), candleSet.end(), range[0]);
            size_t startCandle = it - candleSet.begin();
            // upper_bound() finds the first candle whose index is greater than range[1].
            // We minus it by 1 to get the last candle in the range.
            size_t endCandle = std::upper_bound(candleSet.begin(), candleSet.end(), range[1]) - candleSet.begin();
            if (endCandle > 0)
            {
                // Note this might make endCandle become equal to startCandle.
                endCandle -= 1;
            }

            if (startCandle < endCandle)
            {
                // The total elements within the range is candleSet[endCandle] - candleSet[startCandle] + 1.
                // The total candles within the range is endCandle - startCandle + 1.
                // The 1st minus the 2nd will be the number of plates.
                result.push_back( static_cast<int>( candleSet[endCandle] - candleSet[startCandle] - ( endCandle - startCandle ) ));
            }
            else
            {
                result.push_back(0);
            }

        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 2268. Minimum Number of Keypresses (Medium)
    //-----------------------------------------------------------------------------
    class Solution2268
    {
    public:
        int minimumKeypresses(const string& s)
        {
            unordered_map<char, int> charCount;
            for (const auto& c : s)
            {
                charCount[c]++;
            }

            priority_queue<pair<int, char>> pQueue;
            for (const auto& p : charCount)
            {
                pQueue.emplace(p.second, p.first);
            }
            charCount.clear();

            int letterCount = 0;
            int result = 0;
            // The position of the letter on button. For example, in 'abc' b's pos is 2.
            int pos = 1;
            while (!pQueue.empty())
            {
                int countInStr = pQueue.top().first;
                pQueue.pop();

                result += pos * countInStr;

                letterCount++;

                if (letterCount >= 9)
                {
                    pos++;
                    letterCount = 0;
                }
            }

            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 540. Single Element in a Sorted Array (Medium)
    //-----------------------------------------------------------------------------
    int singleNonDuplicate(const vector<int>& nums)
    {
        // Index:  0 1 2   3 4 5 6 7 8
        //         1,1,2,  3,3,4,4,8,8  If we add the missing number back.
        //         1,1,2,2,3,3,4,4,8,8
        //         0 1 2 3 4 5 6 7 8 9
        //                 ^ The index of the larger numbers (>3) will change.

        // Each number has it paired number.
        // For number at even index, its pair number is supposed to be in index +1.
        // For number at odd index, its pair number is supposed to be in index - 1.
        // If it is not true, the missing number must be in its left side.

        size_t left = 0;
        size_t right = nums.size() - 1;

        // A easy case: 1 1 2
        while (left < right)
        {
            const size_t mid = left + ( right - left ) / 2;
            const size_t pairIdx = ( mid & 0b1 ) == 0 ? mid + 1 : mid - 1;
            if (nums[mid] == nums[pairIdx])
            {
                left = mid + 1;
            }
            else
            {
                // 1 2 2
                //   ^
                right = mid;
            }

        }

        return nums[left];
    }


    //-----------------------------------------------------------------------------
    // Test function.
    //-----------------------------------------------------------------------------
    void TestSortingAndSearching()
    {
        // 973. K Closest Points to Origin (Medium)
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

        // 1152. Analyze User Website Visit Pattern (Medium)
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

        // 1710. Maximum Units on a Truck (Easy)
        // Input: boxTypes = [[1,3],[2,2],[3,1]], truckSize = 4
        // Output: 8
        // You can take all the boxes of the first and second types, and one box of the third type.
        // The total number of units will be = ( 1 * 3 ) + ( 2 * 2 ) + ( 1 * 1 ) = 8.
        // Input: boxTypes = [[1,3],[5,5],[2,5],[4,2],[4,1],[3,1],[2,2],[1,3],[2,5],[3,2]], truckSize = 35
        // Output: 76
        Solution1710 sol1710;
        BuildIntMatrixFromString("[[1,3],[2,2],[3,1]]", &inputMatrix);
        cout << "\n1710. Maximum Units on a Truck: " << sol1710.maximumUnits(inputMatrix, 4) << endl;

        // 2102. Sequentially Ordinal Rank Tracker (Hard)
        cout << "\n2102. Sequentially Ordinal Rank Tracker" << endl;
        SORTracker tracker; // Initialize the tracker system.
        tracker.add("bradford", 2); // Add location with name="bradford" and score=2 to the system.
        tracker.add("branford", 3); // Add location with name="branford" and score=3 to the system.
        cout << tracker.get() << endl;              // The sorted locations, from best to worst, are: branford, bradford.
                                    // Note that branford precedes bradford due to its higher score (3 > 2).
                                    // This is the 1st time get() is called, so return the best location: "branford".
        tracker.add("alps", 2);     // Add location with name="alps" and score=2 to the system.
        cout <<  tracker.get() << endl;              // Sorted locations: branford, alps, bradford.
                                    // Note that alps precedes bradford even though they have the same score (2).
                                    // This is because "alps" is lexicographically smaller than "bradford".
                                    // Return the 2nd best location "alps", as it is the 2nd time get() is called.
        tracker.add("orland", 2);   // Add location with name="orland" and score=2 to the system.
        cout << tracker.get() << endl;              // Sorted locations: branford, alps, bradford, orland.
                                    // Return "bradford", as it is the 3rd time get() is called.
        tracker.add("orlando", 3);  // Add location with name="orlando" and score=3 to the system.
        cout << tracker.get() << endl;              // Sorted locations: branford, orlando, alps, bradford, orland.
                                    // Return "bradford".
        tracker.add("alpine", 2);   // Add location with name="alpine" and score=2 to the system.
        cout << tracker.get() << endl;              // Sorted locations: branford, orlando, alpine, alps, bradford, orland.
                                    // Return "bradford".
        cout << tracker.get() << endl;              // Sorted locations: branford, orlando, alpine, alps, bradford, orland.
                                    // Return "orland".


        // 1438. Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit (Medium)
        // Input: nums = [10,1,2,4,7,2], limit = 5
        // Output: 4
        inputVI = { 10,1,2,4,7,2 };
        cout << "\n1438. Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit: "
            << longestSubarray(inputVI, 5) << endl;

        // 1044. Longest Duplicate Substring (Hard)
        // Input: s = "banana"
        // Output: "ana"
        Solution1044 sol1044;
        cout << "\n1044. Longest Duplicate Substring: " << sol1044.longestDupSubstring("abca") << endl;

        // 2055. Plates Between Candles (Medium)
        // Input: s = "**|**|***|", queries = [[2,5],[5,9]]
        // Output: [2, 3]
        // "***" [[2, 2]]
        //
        BuildIntMatrixFromString("[[2, 2]]", &inputMatrix);
        auto resultVI = platesBetweenCandles("***", inputMatrix);
        cout << "\n2055. Plates Between Candles: " << endl;
        LeetCodeUtil::PrintVector(resultVI);

        // 2268. Minimum Number of Keypresses (Medium)
        // Input: s = "apple"
        // Output : 5
        // Input: s = "abcdefghijkl"
        // Output: 15
        Solution2268 sol2268;
        cout << "\n2268. Minimum Number of Keypresses: " << sol2268.minimumKeypresses("abcdefghijkl") << endl;

        // 540. Single Element in a Sorted Array
        // Input: nums = [1,1,2,3,3,4,4,8,8]
        // Output: 2
        cout << "\n540. Single Element in a Sorted Array: " << singleNonDuplicate({ 1,1,2,3,3,4,4,8,8 }) << endl;

        // 1648. Sell Diminishing-Valued Colored Balls (Medium-Hard)
        // Input: inventory = [2, 5], orders = 4
        // Output : 14
        // [1000000000], 1000000000, expect: 21
        // [1000000000,1000000000,1000000000], 1000000000, expect: 37
        Solution1648 sol1648;
        inputVI = { 1000000000,1000000000,1000000000 };
        int orders = 1000000000;
        cout << "\n1648. Sell Diminishing-Valued Colored Balls: " << sol1648.maxProfit(inputVI, orders) << endl;
    }
}