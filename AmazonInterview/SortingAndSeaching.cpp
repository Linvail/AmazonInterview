#include "SortingAndSearching.h"
#include "LeetCodeUtil.h"

#include <algorithm>
#include <vector>

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
    }
}