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

                orders -= static_cast<int>(min(static_cast<long long>(orders), maxCountToTake));
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
    // 1094. Car Pooling (Medium)
    //-----------------------------------------------------------------------------
    bool carPooling(vector<vector<int>>& trips, int capacity)
    {
        #if( NAVIE )
        sort(trips.begin(), trips.end(), [](const vector<int> left, const vector<int>& right)
            {
                return left[1] < right[1];
            });

        // <index to drop, number of passengers to drop>
        multiset<pair<int, int>> passengersToDrop;

        int carriedPeople = 0;
        for (size_t i = 0; i < trips.size(); ++i)
        {
            const auto& currTrip = trips[i];

            // Check if we should drop someone.
            while (!passengersToDrop.empty() && passengersToDrop.begin()->first <= currTrip[1])
            {
                carriedPeople -= passengersToDrop.begin()->second;
                passengersToDrop.erase(passengersToDrop.begin());
            }

            carriedPeople += currTrip[0];
            if (carriedPeople > capacity)
            {
                return false;
            }

            passengersToDrop.emplace(currTrip[2], currTrip[0]);
        }

        return true;
        #endif

        vector<int> positions(1001);

        for (const auto& trip : trips)
        {
            positions[trip[1]] += trip[0]; // Pick up
            positions[trip[2]] -= trip[0]; // Drop off
        }

        for (const auto& passengers : positions)
        {
            capacity -= passengers;
            if (capacity < 0)
            {
                return false;
            }
        }

        return true;
    }

    //-----------------------------------------------------------------------------
    // 632. Smallest Range Covering Elements from K Lists (Hard)
    //-----------------------------------------------------------------------------
    vector<int> smallestRange(vector<vector<int>>& nums)
    {
        // We have to sort k numbers which are coming from K different lists.
        // We can use set/multiset or priority_queue to handle the sort.

        // Use priority_queue to store the left iterator and the end iterator of
        // each list.

        // Compare function for iterator.
        auto cmp = [](const auto& left, const auto& right)
        {
            return *left.first > *right.first;
        };

        using IT = vector<int>::iterator;
        priority_queue<pair<IT, IT>, vector<pair<IT, IT>>, decltype( cmp ) > sortQueue(cmp);

        int lowest = INT_MAX;
        int highest = INT_MIN;
        for (auto& list : nums)
        {
            lowest = min(lowest, list.front());
            highest = max(highest, list.front());
            sortQueue.emplace(list.begin(), list.end());
        }

        // Initial result - lowest of the first number, and the highest of the first number
        // among all lists.
        vector<int> result = { lowest, highest };

        while (!sortQueue.empty())
        {
            // 'current' has the lowest value.
            auto current = sortQueue.top();
            sortQueue.pop();

            current.first++;
            if (current.first == current.second)
            {
                // This list has come to the end.
                break;
            }
            sortQueue.push(current);

            // sortQueue has been re-sorted now, fetch the lowest.
            lowest = *sortQueue.top().first;
            // current.first is the only element that has changed.
            // Use it to update the highest number.
            highest = max(highest, *current.first);

            // range [a, b] is smaller than range [c, d] if b - a < d - c or a < c if b - a == d - c.
            if (highest - lowest < result[1] - result[0] ||
                (highest - lowest == result[1] - result[0] && lowest < result[0]))
            {
                result = { lowest, highest };
            }
        }

        return result;
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

        // 1094. Car Pooling (Medium)
        // Input: trips = [[2,1,5],[3,3,7]], capacity = 4
        // Output: false
        // [[3, 2, 7], [3, 7, 9], [8, 3, 9]], 11
        // Expect: true
        string matrixString = "[[18,435,914],[93,224,510],[32,161,343],[80,433,732],[78,643,702],[29,77,197],[93,478,948],[77,905,979],[8,491,631],[48,25,293],[80,382,512],[27,21,176],[57,82,456],[79,504,973],[30,560,587],[13,336,623],[78,406,679],[67,267,727],[56,600,899],[73,481,503],[99,302,432],[61,685,722],[46,648,788],[68,519,639],[7,274,704],[38,655,904],[99,37,826],[1,200,499],[75,601,845],[54,22,120],[75,35,594],[44,155,406],[86,113,542],[78,66,987],[88,230,433],[74,794,995],[82,420,603],[3,56,195],[63,749,939],[11,545,607],[48,434,880],[38,202,707],[20,520,595],[44,249,811],[71,151,633],[97,583,800],[18,467,744],[10,453,903],[5,872,888],[74,607,952],[20,187,893],[82,863,900],[50,406,557],[64,275,916],[84,568,674],[82,433,483],[49,718,825],[20,115,929],[96,160,840],[82,80,465],[18,178,423],[60,337,362],[74,176,694],[2,287,809],[47,582,671],[84,165,228],[40,268,536],[37,154,238],[84,213,936],[25,437,530],[11,415,958],[33,354,783],[66,565,838],[93,159,512],[34,626,834],[43,359,382],[52,90,640],[92,17,173],[12,513,861],[99,444,686],[29,529,774],[52,112,866],[65,367,888],[46,436,651],[8,4,167],[29,940,976],[56,63,66],[56,50,860],[63,480,959],[45,572,667],[16,409,540],[93,30,403],[50,462,923],[1,473,542],[20,185,195],[70,204,465],[95,101,905],[32,198,967],[70,347,434],[44,219,555],[16,48,252],[6,656,698],[25,456,771],[71,15,514],[77,502,580],[80,800,873],[35,42,954],[1,606,836],[20,10,378],[15,493,547],[100,217,326],[32,55,234],[82,168,760],[60,162,718],[44,289,505],[24,150,507],[100,277,341],[34,483,829],[8,443,811],[71,186,193],[76,122,709],[2,589,997],[61,146,180],[91,228,782],[48,86,230],[72,263,535],[59,287,377],[96,207,285],[27,386,853],[65,21,994],[48,185,453],[62,589,700],[23,558,576],[53,95,184],[12,162,212],[34,195,268],[70,391,731],[95,231,897],[11,7,789],[38,30,216],[72,643,693],[39,196,866],[1,667,812],[36,84,373],[18,133,650],[76,575,974],[10,109,737],[13,406,530],[85,491,855],[79,700,828],[67,513,850],[7,18,70],[55,459,667],[14,154,822],[40,383,775],[7,537,965],[95,83,911],[73,802,968],[77,924,998],[100,366,742],[83,407,687],[57,524,683],[97,534,956],[16,135,299],[55,315,806],[52,390,590],[58,68,505],[22,608,978],[59,582,819],[30,56,381],[96,120,731],[91,672,860],[28,366,939],[32,283,390],[84,283,287],[35,290,851],[13,205,224],[47,527,890],[11,464,998],[94,576,701],[16,176,940],[79,72,252],[32,440,929],[70,494,556],[56,414,737],[42,225,952],[27,372,430],[8,50,216],[42,86,635],[61,252,587],[56,676,731],[51,481,720],[95,898,979],[92,422,642],[41,143,299],[15,138,242],[31,257,688],[78,148,315],[93,141,951],[9,754,938],[57,748,805],[9,516,852],[68,197,839],[98,228,829],[86,157,638],[19,95,843],[57,86,953],[63,157,574],[43,215,251],[42,274,332],[70,730,954],[9,323,469],[58,152,240],[33,643,903],[61,640,711],[100,38,326],[76,212,974],[99,768,820],[1,452,914],[23,66,944],[92,151,738],[65,10,518],[49,191,229],[78,289,444],[72,134,245],[81,365,810],[57,36,74],[14,322,810],[24,304,453],[28,78,223],[78,591,678],[86,4,964],[20,649,762],[33,281,698],[2,906,914],[13,214,520],[72,171,922],[42,193,570],[64,148,153],[12,275,930],[42,661,664],[40,430,801],[9,51,726],[86,756,787],[56,777,903],[54,203,614],[45,597,764],[59,235,405],[53,566,684],[53,199,769],[24,721,876],[84,596,628],[25,223,925],[66,258,425],[93,217,571],[81,70,285],[85,356,478],[64,281,806],[39,252,978],[94,122,983],[87,377,743],[54,54,67],[7,72,721],[9,111,984],[72,91,905],[2,87,851],[97,162,786],[50,929,960],[70,20,479],[69,273,837],[13,738,904],[7,410,462],[11,530,583],[46,344,976],[65,617,699],[62,270,417],[53,248,557],[66,224,562],[10,91,527],[40,32,338],[21,306,897],[28,349,998],[96,313,393],[55,167,401],[52,362,653],[83,654,988],[93,217,290],[38,116,730],[89,475,854],[57,459,862],[56,178,498],[16,180,853],[84,72,309],[5,43,654],[30,611,629],[69,602,878],[1,166,871],[22,622,984],[22,378,706],[3,508,992],[12,403,804],[23,142,386],[43,493,923],[57,66,915],[59,408,841],[53,25,176],[90,415,503],[19,133,378],[93,46,591],[26,697,934],[64,554,839],[83,306,810],[47,46,200],[43,40,563],[33,326,798],[5,133,826],[79,698,869],[70,378,477],[63,462,551],[80,424,674],[22,372,384],[35,676,858],[33,350,805],[50,118,841],[47,634,739],[95,888,954],[80,413,862],[97,561,812],[11,559,968],[98,284,300],[82,207,854],[25,173,615],[44,628,823],[52,236,252],[45,489,795],[86,234,874],[83,589,970],[73,195,596],[47,93,740],[99,745,759],[83,239,658],[34,420,662],[51,175,852],[97,689,934],[32,355,466],[34,824,845],[59,372,544],[80,433,666],[59,171,234],[19,122,491],[38,558,655],[60,49,964],[41,55,985],[8,641,956],[16,319,863],[28,68,795],[50,546,574],[24,74,313],[31,54,678],[72,113,808],[30,84,463],[30,175,642],[7,583,672],[77,31,185],[9,611,628],[91,406,525],[69,263,929],[98,580,975],[77,70,514],[55,526,965],[79,140,361],[49,43,378],[91,622,844],[51,76,700],[24,508,559],[62,494,600],[66,174,179],[46,70,604],[66,235,400],[24,280,340],[80,160,756],[62,640,683],[29,68,292],[28,514,944],[70,11,368],[37,334,733],[26,187,803],[31,574,672],[39,727,769],[74,412,765],[81,128,233],[97,60,980],[49,179,843],[51,406,993],[98,19,59],[91,61,746],[48,89,585],[8,470,574],[79,57,927],[57,85,701],[52,42,276],[3,429,519],[65,35,878],[71,60,875],[36,22,85],[1,109,488],[94,227,649],[4,446,710],[92,475,994],[58,313,957],[26,249,801],[76,982,984],[35,176,180],[50,271,377],[99,130,874],[80,135,426],[51,64,195],[43,4,347],[87,11,646],[11,218,265],[4,302,310],[5,307,576],[98,362,926],[89,493,965],[60,154,770],[77,303,449],[72,340,547],[57,678,775],[25,40,725],[79,781,886],[57,466,867],[28,279,356],[12,491,532],[64,742,891],[55,99,240],[17,295,465],[29,200,644],[54,693,740],[46,458,672],[9,500,519],[66,716,953],[69,204,941],[68,880,958],[81,44,189],[62,294,823],[89,178,869],[100,498,628],[35,218,698],[97,111,452],[6,314,408],[63,190,636],[9,761,843],[29,395,565],[6,54,928],[61,713,813],[91,455,863],[1,806,808],[59,552,671],[32,56,218],[87,711,744],[28,589,983],[11,212,586],[81,707,755],[22,102,182],[68,204,785],[63,292,810],[61,4,972],[54,11,379],[93,743,913],[9,543,711],[99,463,696],[1,307,543],[32,426,548],[92,38,578],[93,324,909],[5,570,926],[53,5,659],[39,103,327],[35,494,627],[17,477,760],[21,360,813],[63,175,293],[55,681,707],[46,412,596],[98,8,592],[83,588,967],[24,743,996],[55,431,496],[55,98,781],[98,32,531],[66,692,854],[86,138,706],[46,48,962],[100,206,224],[19,419,661],[46,365,779],[52,344,944],[99,498,743],[8,926,970],[25,546,828],[63,249,943],[12,390,667],[35,296,390],[71,144,198],[82,191,607],[86,905,953],[34,374,796],[60,816,964],[100,612,861],[31,771,783],[76,256,439],[9,208,992],[70,613,929],[24,1,439],[35,400,825],[2,307,845],[80,334,339],[82,52,631],[64,666,851],[69,140,938],[27,508,675],[79,61,90],[73,878,957],[36,136,550],[76,391,517],[95,406,536],[39,111,389],[65,682,802],[26,420,769],[17,101,126],[48,42,714],[31,547,749],[64,62,914],[36,267,616],[7,543,770],[51,163,738],[81,667,733],[85,718,794],[96,203,369],[53,441,673],[59,552,927],[96,414,623],[96,280,673],[8,81,813],[89,377,833],[44,85,736],[17,174,429],[44,253,924],[1,172,209],[34,174,243],[47,520,632],[62,261,401],[31,75,878],[23,6,694],[98,29,448],[14,432,701],[22,765,841],[52,541,647],[5,321,985],[20,657,997],[34,277,365],[57,161,896],[59,381,775],[15,355,557],[24,683,952],[35,123,296],[61,735,934],[65,240,415],[100,834,934],[64,939,971],[67,257,778],[45,118,798],[3,920,986],[10,415,844],[83,295,455],[100,59,915],[32,217,570],[49,192,619],[62,69,450],[69,271,812],[47,239,762],[64,268,744],[57,213,228],[43,364,670],[24,512,639],[74,450,472],[8,146,562],[97,325,331],[33,476,803],[15,89,248],[24,83,370],[40,399,432],[14,730,890],[88,38,601],[13,250,690],[25,70,518],[44,761,962],[81,398,575],[54,200,312],[4,312,317],[13,104,250],[8,132,730],[72,134,405],[67,489,657],[85,788,824],[26,333,890],[4,584,618],[49,142,301],[92,493,759],[96,309,584],[71,464,856],[7,601,683],[92,25,456],[9,24,40],[83,403,567],[9,248,619],[44,190,529],[18,58,196],[82,498,883],[21,354,509],[9,106,663],[63,172,912],[34,763,828],[51,306,860],[97,806,874],[90,304,926],[70,77,810],[14,459,485],[3,344,777],[98,377,588],[61,283,430],[71,794,917],[32,250,949],[15,906,946],[40,270,330],[33,380,439],[16,706,961],[69,711,876],[46,48,317],[70,133,988],[82,747,765],[99,178,969],[55,571,830],[66,222,664],[43,91,992],[89,11,418],[28,570,775],[6,391,969],[63,178,797],[13,449,693],[67,296,678],[99,475,917],[49,218,624],[52,185,525],[84,433,926],[45,381,927],[76,459,607],[65,273,509],[17,591,733],[58,501,664],[61,191,675],[94,185,465],[65,307,695],[48,674,750],[93,802,859],[94,272,768],[30,224,976],[54,151,602],[62,767,862],[16,157,477],[34,345,404],[41,5,307],[61,61,76],[1,295,297],[85,612,660],[64,635,841],[80,175,367],[6,539,786],[88,322,469],[4,587,904],[42,98,577],[63,5,33],[79,347,790],[66,63,228],[43,531,960],[89,441,859],[98,311,515],[67,728,891],[30,23,573],[48,151,962],[84,157,858],[14,54,472],[58,106,337],[58,83,409],[77,556,906],[84,782,824],[39,3,416],[91,265,676],[64,756,969],[92,63,816],[79,372,511],[58,26,888],[10,514,722],[14,148,862],[68,236,494],[36,705,961],[20,833,929],[58,239,991],[2,748,786],[75,167,321],[96,387,820],[16,376,851],[26,682,881],[75,292,994],[31,132,919],[80,79,265],[17,301,429],[96,409,510],[34,258,442],[94,366,865],[100,315,323],[20,316,326],[20,583,848],[36,336,462],[42,208,837],[97,49,559],[94,249,701],[1,927,935],[38,325,792],[22,166,241],[65,4,396],[16,90,742],[45,91,348],[70,374,797],[89,19,851],[40,202,905],[70,227,308],[56,379,386],[70,880,952],[98,662,761],[54,63,982],[64,310,797],[39,141,411],[11,41,826],[33,855,996],[44,205,586],[73,341,772],[66,498,694],[26,468,658],[1,381,486],[12,124,900],[11,539,762],[47,295,327],[3,47,495],[75,417,823],[85,40,511],[25,2,853],[22,64,386],[69,462,516],[68,371,945],[16,57,524],[83,24,579],[59,199,312],[18,48,445],[65,90,759],[68,314,566],[30,555,906],[65,403,544],[65,195,485],[43,139,296],[81,343,445],[24,52,132],[95,10,927],[93,91,313],[46,232,429],[67,706,832],[41,588,707],[56,761,805],[64,489,552],[58,497,507],[91,52,204],[30,575,875],[85,145,363],[7,432,532],[89,16,356],[96,575,657],[81,343,543],[9,93,641],[62,211,833],[77,27,684],[32,434,973],[100,384,703],[59,235,547],[79,23,448],[87,613,683],[86,564,845],[44,441,898],[39,244,643],[81,305,651],[85,315,379],[50,418,488],[33,571,837],[80,3,461],[91,83,227],[84,147,667],[16,602,705],[34,41,801],[2,449,726],[54,73,511],[70,80,527],[7,395,875],[42,756,942],[40,348,899],[23,120,952],[19,93,335],[7,578,929],[11,893,933],[52,743,800],[92,400,929],[46,68,290],[44,817,853],[97,453,796],[31,188,728],[50,726,977],[62,446,987],[40,209,637],[29,63,788],[33,290,826],[1,450,990],[13,381,394],[44,738,901],[55,625,658],[74,701,769],[53,329,712],[34,179,960],[67,739,873],[25,326,609],[29,337,338],[58,186,442],[20,293,380],[97,425,725],[26,111,595],[23,539,921],[85,23,212],[90,133,163],[92,92,207],[19,723,879],[92,258,276],[74,123,395],[11,818,987],[56,373,679],[13,848,953],[45,471,509],[66,54,686],[54,393,596],[92,644,777],[20,11,513],[50,364,371],[7,861,899],[56,36,943],[73,585,941],[77,90,340],[71,196,208],[39,234,510],[9,435,940],[81,618,767],[39,311,977],[53,9,684],[5,234,712],[32,112,848],[55,225,406],[3,126,408],[63,466,775],[14,364,589],[60,902,982],[36,554,872],[81,118,993],[18,497,903],[36,371,749],[95,581,988],[54,537,984],[41,298,751],[66,163,580],[98,42,154],[70,195,236],[78,103,292],[56,458,795],[12,432,467],[57,3,207],[75,78,95],[72,550,758],[46,108,493],[70,387,615],[77,175,561],[7,315,599],[8,176,611],[56,211,458],[13,412,770],[34,164,692],[51,297,475],[62,360,875],[30,767,933],[40,447,965],[48,86,577],[13,445,585],[53,635,883],[23,70,619],[17,751,907],[18,376,491],[79,677,696],[11,317,406],[3,457,547],[88,621,868],[96,443,570],[96,332,347],[20,378,944],[41,120,739],[67,267,403],[96,72,833],[49,323,495],[34,315,402],[16,105,429],[61,1,923],[75,46,419],[32,100,397],[93,140,247],[9,56,240],[16,314,488],[41,108,708],[97,7,193],[31,26,101],[13,234,596],[67,571,614],[67,150,716],[51,559,817],[13,270,643],[59,138,749],[85,34,605],[90,234,349],[21,932,991],[45,310,699],[23,461,502],[76,605,694],[98,1,758],[55,5,262],[46,76,90],[57,64,673],[31,150,452],[17,392,971],[61,135,590],[26,48,739],[48,18,161],[86,80,500],[96,339,356],[25,581,599],[67,22,901],[100,198,696],[78,279,599],[89,334,386],[49,63,287],[97,30,149],[26,265,890],[89,397,437],[17,269,973],[6,285,296],[58,242,836],[21,345,782],[54,327,542],[80,813,825],[48,146,431],[49,509,614],[35,828,954],[26,418,470],[94,242,629],[10,426,686],[50,48,203],[33,804,834],[1,723,914],[80,49,868],[60,533,973],[79,834,853],[63,46,459],[96,347,391],[14,406,596],[5,113,433],[86,523,575],[97,19,132],[45,238,964],[90,238,280],[42,713,741],[84,190,429],[29,292,648],[73,583,952],[58,251,893],[23,129,472],[93,201,855],[69,259,836],[9,690,777],[13,116,974],[58,197,739]]";
        LeetCodeUtil::BuildIntMatrixFromString(matrixString, &inputMatrix);
        cout << "\n1094. Car Pooling: " << carPooling(inputMatrix, 25210);

        // 632. Smallest Range Covering Elements from K Lists (Hard)
        // Input: nums = [[4,10,15,24,26],[0,9,12,20],[5,18,22,30]]
        // Output: [20, 24]
        BuildIntMatrixFromString("[[4,10,15,24,26],[0,9,12,20],[5,18,22,30]]", &inputMatrix);
        resultVI = smallestRange(inputMatrix);
        cout << "\n632. Smallest Range Covering Elements from K Lists: " << endl;
        PrintVector(resultVI);
    }
}