#include "Math.h"
#include "LeetCodeUtil.h"

#include <string>
#include <algorithm>

namespace Math
{
    using namespace std;

    //-----------------------------------------------------------------------------
    // 1492. The kth Factor of n
    // Follow up: Could you solve this problem in less than O(n) complexity ?
    //-----------------------------------------------------------------------------
    class Solution1492
    {
    public:
        int kthFactor(int n, int k)
        {
            #if( 0 )
            // O(n)
            for (int div = 1; div <= ( n / 2 ); div++)
            {
                if (n % div == 0)
                {
                    k--;
                    if (k == 0)
                    {
                        return div;
                    }
                }
            }
            return k == 1 ? n : -1;
            #endif

            // O(sqrt(n))
            const int sqrtInt = static_cast<int>( std::sqrt(n) );
            // Factors list is[1, 2, 3, 4, 6, 12]. sqrtInt = 3.
            int div = 1;
            for (; div <= sqrtInt; ++div)
            {
                if (n % div == 0)
                {
                    k--;
                    if (k == 0)
                    {
                        return div;
                    }
                }
            }
            // Assume n = 12, k = 5. sqrtInt = 3.
            // div will be 4 and k became 2. We need to find the next 2.
            // Special case n = 100. sqrtInt = 10.
            div--;
            for (; div >= 1; div--)
            {
                if (div * div == n)
                {
                    // 10 * 10 = 100
                    // 10 can be counted a 1, which has been counted in the 1st loop.
                    continue;
                }

                if (n % div == 0)
                {
                    k--;
                    if (k == 0)
                    {
                        return n / div;
                    }
                }
            }

            return -1;
        }
    };

    //-----------------------------------------------------------------------------
    // 1360. Number of Days Between Two Dates (Easy)
    // The given dates are valid dates between the years 1971 and 2100.
    //-----------------------------------------------------------------------------

    class Solution1360
    {
    public:
        int daysBetweenDates(const string& date1, const string& date2)
        {
            int days1 = daysFrom1970(date1);
            int days2 = daysFrom1970(date2);

            return abs(days1 - days2);
        }

    private:

        int daysFrom1970(const string& date)
        {
            vector<int> daysInMonth = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

            // Calculate days from 1970 to the date.
            int year1;
            int month1;
            int day1;
            parseDateString(date, year1, month1, day1);

            int daysSum = 0;

            for (int i = 1970; i < year1; ++i)
            {
                daysSum += isLeapYear(i) ? 366 : 365;
            }

            for (int i = 0; i <= month1 - 1; ++i)
            {
                int daysInThisMonth = daysInMonth[i];
                if (i == 1 && isLeapYear(year1))
                {
                    daysInThisMonth = 29;
                }

                if (i == month1 - 1)
                {
                    daysSum += day1;
                }
                else
                {
                    daysSum += daysInThisMonth;
                }
            }

            return daysSum;
        }

        inline bool isLeapYear(int year)
        {
            return ( year % 4 == 0 && ( year % 100 != 0 || year % 400 == 0 ) );
        }

        inline void parseDateString(const string& date, int& year, int& month, int& day)
        {
            // YYYY-MM-DD
            year = stoi(date.substr(0, 4));
            month = stoi(date.substr(5, 2));
            day = stoi(date.substr(8, 2));
        }
    };

    //-----------------------------------------------------------------------------
    // 634. Find the Derangement of An Array (Medium)
    // Topic: Math, Dynamic programming
    //-----------------------------------------------------------------------------
    int findDerangement(int n)
    {
        // n = 1, answer = 0.
        // n = 2, answer = 1.
        // n = 3, answer = 2.
        // When n <= 3, the answer is easy. After 4, we need to find the recursion
        // equation.
        // Let dp[n] denotes the number of derangements for n.
        // n = 4. [1, 2, 3, 4]. Consider move '4' to another location. We have 3 choices.
        // Assume we move 4 to 3's location: [x, x, 4, x].
        // Now we can divide the problem into two subproblems:
        // 1. 3 is in 4's location, i.e. [x, x, 4, 3]. In this case, we have dp[2].
        // 2. 3 is not in 4's location, i.e. [x, x, 4, x(not 3)]. In this case, the number of derangements is equal to dp[3].
        // So, we have dp[4] = 3 * (dp[3] + dp[2]).
        // In general, we have dp[i] = (i - 1) * (dp[i - 1] + dp[i - 2]).

        long long prev = 1;
        long long prevOfPrev = 0;
        long long result = prev;

        for (int i = 3; i <= n; ++i)
        {
            result = (( i - 1 ) * ( prev + prevOfPrev )) % 1000000007;
            // Update prev and prevOfPrev.
            prevOfPrev = prev;
            prev = result;
        }

        return n == 1 ? 0 : static_cast<int>(result);
    }

    //-----------------------------------------------------------------------------
    // 556. Next Greater Element III (Medium)
    //-----------------------------------------------------------------------------
    class Solution556
    {
    public:
        int nextGreaterElement(int n)
        {
            string strNum = to_string(n);

            // std::next_permutation() rearranges the elements in the range
            // [first,last) into the next lexicographically greater permutation
            // However, the interviewer might not want me to use it.
            // The interviewer may want to me to implement it.
            //bool ret = std::next_permutation(strNum.begin(), strNum.end());

            if (!nextGreaterPermutation(strNum))
            {
                return -1;
            }

            long long result = stoll(strNum);

            return result > INT_MAX ? -1 : static_cast<int>(result);
        }

    private:

        bool nextGreaterPermutation(string& strNum)
        {
            if (strNum.empty()) return false;

            size_t i = strNum.size() - 1;
            // Scan from right to right, find the first i where
            // stop increasing.
            // 0 1 2 3 4 5 6 < - index
            // 1 0 2 3 6 5 4
            //         ^     Stop at index 4.
            while (i >= 1 && strNum[i] <= strNum[i - 1])
            {
                i--;
            }
            if (i == 0)
            {
                // Like: 9 8 7 6 5 4. It is monotonic increasing from left to right.
                return false; // no next permutation, i.e. already largest
            }

            size_t j = strNum.size() - 1;

            // 0 1 2 3 4 5 6 < - index
            // 1 0 2 3 6 5 4
            //         i   j
            while (strNum[j] <= strNum[i - 1])
            {
                j--;
            }
            // 1 0 2 3 6 5 4. Swap 4 and 3 => 1 0 2 4 6 5 3
            swap(strNum[i - 1], strNum[j]);

            // 1 0 2 4 6 5 3. Reverse i ~ n - 1. => 1 0 2 4 3 5 6
            reverse(strNum.begin() + i, strNum.end());

            return true;
        }
    };

    //-----------------------------------------------------------------------------
    // Test function.
    //-----------------------------------------------------------------------------
    void TestMath()
    {
        // 1492. The kth Factor of n
        // Input: n = 12, k = 3
        // Output: 3
        // Input: n = 2, k = 2
        // Output: 2
        Solution1492 sol1492;
        cout << "\n1492. The kth Factor of n: " << sol1492.kthFactor(100, 7) << endl;

        // 1360. Number of Days Between Two Dates (Easy)
        // Input: date1 = "2019-06-29", date2 = "2019-06-30"
        // Output: 1
        // Input: date1 = "2020-01-15", date2 = "2019-12-31"
        // Output: 15
        // "2100-09-22" "1991-03-12"
        // Expect: 40006
        Solution1360 sol1360;
        cout << "\n1360. Number of Days Between Two Dates: " << sol1360.daysBetweenDates("2100-09-22", "1991-03-12") << endl;

        // 634. Find the Derangement of An Array (Medium)
        // Input: 10
        // Output: 1334961
        cout << "\n634. Find the Derangement of An Array: " << findDerangement(10) << endl;

        // 556. Next Greater Element III (Medium)
        Solution556 sol556;
        cout << "\n556. Next Greater Element III: " << sol556.nextGreaterElement(1023654) << endl;
    }
}