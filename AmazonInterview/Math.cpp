#include "Math.h"
#include "LeetCodeUtil.h"

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
    }
}