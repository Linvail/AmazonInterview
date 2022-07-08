#include "Math.h"
#include "LeetCodeUtil.h"

#include <string>

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
    }
}