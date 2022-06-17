#include "OptimalUtilization.h"
#include "LeetCodeUtil.h"

#include <algorithm>
#include <vector>

using namespace std;

namespace ArrayAndString
{
    //-----------------------------------------------------------------------------
    // Optimal Utilization (Seen in OA in 2019)
    //
    //
    // Given 2 lists a and b. Each element is a pair of integers where the first
    // integer represents the unique id and the second integer represents a value.
    // Your task is to find an element from a and an element form b such that the
    // sum of their values is less or equal to target and as close to target as
    // possible. Return a list of ids of selected elements. If no pair is possible,
    // return an empty list.
    //-----------------------------------------------------------------------------
    /*
    Example 1:

        Input:
        a = [[1, 2], [2, 4], [3, 6]]
        b = [[1, 2]]
        target = 7

        Output: [[2, 1]]

        Explanation:
        There are only three combinations [1, 1], [2, 1], and [3, 1], which have a total sum of 4, 6 and 8, respectively.
        Since 6 is the largest sum that does not exceed 7, [2, 1] is the optimal pair.

    Example 2:

        Input:
        a = [[1, 3], [2, 5], [3, 7], [4, 10]]
        b = [[1, 2], [2, 3], [3, 4], [4, 5]]
        target = 10

        Output: [[2, 4], [3, 2]]

        Explanation:
        There are two pairs possible. Element with id = 2 from the list `a` has a value 5, and element with id = 4 from the list `b` also has a value 5.
        Combined, they add up to 10. Similarly, element with id = 3 from `a` has a value 7, and element with id = 2 from `b` has a value 3.
        These also add up to 10. Therefore, the optimal pairs are [2, 4] and [3, 2].

    Example 3:

        Input:
        a = [[1, 8], [2, 7], [3, 14]]
        b = [[1, 5], [2, 10], [3, 14]]
        target = 20

        Output: [[3, 1]]

    Example 4:

        Input:
        a = [[1, 8], [2, 15], [3, 9]]
        b = [[1, 8], [2, 11], [3, 12]]
        target = 20

        Output: [[1, 3], [3, 2]]

    */

    vector<vector<int>> optimalUtilization
        (
        vector<vector<int>>& listA,
        vector<vector<int>>& listB,
        int target
        )
    {
        // Sort listB in descending order.
        std::sort(listB.begin(), listB.end(), [](const vector<int>& left, const vector<int>& right)
            {
                return left[1] > right[1];
            });

        int minDiffSeen = INT_MAX;
        // Store the perfectly matched results.
        vector<vector<int>> result;
        // Store the non-perfectly matched result;
        vector<vector<int>> unMatchedResult;

        for (const auto& objA : listA)
        {
            int diff = target - objA[1];
            vector<int> dummpy{ -1, diff };

            // Find the first record whose value is not-greater-than 'diff'.
            // This is tricky.
            // Originally, lower_bound() find the first not-less-than element with comp(<) in a
            // ascending order storage (sorted by <).
            // In this case, we sort the storage in reverse order (descending) and use reverse comp(>).
            // Therefore, lower_bound()'s behavior becomes finding the first not-greater-than.
            auto anotherPair = std::lower_bound(listB.begin(), listB.end(), dummpy,
                [](const vector<int>& left, const vector<int>& right)
                {
                    // The value returned indicates whether the first argument is
                    // considered to go before the second.
                    return left[1] > right[1];
                }
            );

            if (anotherPair == listB.end())
            {
                // Cannot find any match. Try next.
                continue;
            }

            diff = target - ( objA[1] + anotherPair->at(1) );
            if (diff == 0)
            {
                // Perfect match.
                result.push_back({ objA[0], anotherPair->at(0) });
                // If we ever found a perfect match, we don't accept non-perfectly
                // matched result.
                unMatchedResult.clear();
                minDiffSeen = 0;
            }
            else if(minDiffSeen != 0 && diff <= minDiffSeen)
            {
                if (diff < minDiffSeen)
                {
                    // Clear previous worse results.
                    unMatchedResult.clear();
                    minDiffSeen = diff;
                }
                unMatchedResult.push_back({ objA[0], anotherPair->at(0) });
            }
        }

        return result.empty() ? unMatchedResult : result;
    }

    void TestOptimalUtilization()
    {
        vector<vector<int>> listA;
        vector<vector<int>> listB;
        LeetCodeUtil::BuildIntMatrixFromString("[[1, 2], [2, 4], [3, 6]]", &listA);
        LeetCodeUtil::BuildIntMatrixFromString("[[1, 2]]", &listB);
        auto resultVVI = optimalUtilization(listA, listB, 7);
        cout << "\nOptimal Utilization: Example 1. Expected: [2, 1]:" << endl;
        LeetCodeUtil::PrintMatrix(resultVVI);

        LeetCodeUtil::BuildIntMatrixFromString("[[1, 3], [2, 5], [3, 7], [4, 10]]", &listA);
        LeetCodeUtil::BuildIntMatrixFromString("[[1, 2], [2, 3], [3, 4], [4, 5]]", &listB);
        resultVVI = optimalUtilization(listA, listB, 10);
        cout << "\nOptimal Utilization: Example 1. Expected: [[2, 4], [3, 2]]:" << endl;
        LeetCodeUtil::PrintMatrix(resultVVI);

        LeetCodeUtil::BuildIntMatrixFromString("[[1, 8], [2, 7], [3, 14]]", &listA);
        LeetCodeUtil::BuildIntMatrixFromString("[[1, 5], [2, 10], [3, 14]]", &listB);
        resultVVI = optimalUtilization(listA, listB, 20);
        cout << "\nOptimal Utilization: Example 1. Expected: [[3, 1]]:" << endl;
        LeetCodeUtil::PrintMatrix(resultVVI);

        LeetCodeUtil::BuildIntMatrixFromString("[[1, 8], [2, 15], [3, 9]]", &listA);
        LeetCodeUtil::BuildIntMatrixFromString("[[1, 8], [2, 11], [3, 12]]", &listB);
        resultVVI = optimalUtilization(listA, listB, 20);
        cout << "\nOptimal Utilization: Example 1. Expected: [[1, 3], [3, 2]]:" << endl;
        LeetCodeUtil::PrintMatrix(resultVVI);
    }
}