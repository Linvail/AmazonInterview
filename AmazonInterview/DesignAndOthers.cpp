#include "DesignAndOthers.h"
#include "LeetCodeUtil.h"

#include <deque>
#include <map>
#include <unordered_map>
#include <vector>
#include <queue>

namespace DesignAndOthers
{
    using namespace std;
    using namespace LeetCodeUtil;

    //-----------------------------------------------------------------------------
    // 895. Maximum Frequency Stack (Hard)
    // freqStack.push(5); // The stack is [5]
    // freqStack.push(7); // The stack is [5,7]
    // freqStack.push(5); // The stack is [5,7,5]
    // freqStack.push(7); // The stack is [5,7,5,7]
    // freqStack.push(4); // The stack is [5,7,5,7,4]
    // freqStack.push(5); // The stack is [5,7,5,7,4,5]
    // freqStack.pop();   // return 5, as 5 is the most frequent. The stack becomes [5,7,5,7,4].
    // freqStack.pop();   // return 7, as 5 and 7 is the most frequent, but 7 is closest to the top. The stack becomes [5,7,5,4].
    // freqStack.pop();   // return 5, as 5 is the most frequent. The stack becomes [5,7,4].
    // freqStack.pop();   // return 4, as 4, 5 and 7 is the most frequent, but 4 is closest to the top. The stack becomes [5,7].
    //-----------------------------------------------------------------------------

    // Idea: Need to record the frequency of element.
    // Need to sort by the frequency. multimap should be good.
    // When the frequency is equal, how to maintain the order of push-in?
    // map, unordered_map, multimap do not seem to keep the order of push-in.
    // We can use deque/queue/vector..etc.
    class FreqStack
    {
    public:
        FreqStack() {}

        void push(int val)
        {
            m_elements[val]++;
            // The same element may appear in the vector of several frequency.
            m_freq[m_elements[val]].push_back(val);
            maxFreq = max(maxFreq, m_elements[val]);
        }

        int pop()
        {
            int element = m_freq[maxFreq].back();
            m_freq[maxFreq].pop_back();
            // If there is no element has this frequency, minus the max frequency.
            if (m_freq[maxFreq].empty())
            {
                maxFreq--;
            }
            m_elements[element]--;

            return element;
        }

    private:
        size_t maxFreq = 0;
        // element -> frequency
        unordered_map<int, size_t> m_elements;
        // frequency -> vector of elements
        unordered_map<size_t, vector<int>> m_freq;
    };

    // Using priority_queue and tuple.
    class FreqStack2
    {
    public:
        void push(int val)
        {
            freqStack.emplace(++counts[val], prio++, val);
        }

        int pop()
        {
            const auto [f, p, v] = freqStack.top();  // Requires C++17.
            freqStack.pop();
            --counts[v];
            return v;
        }
    private:
        // data range might be an issue...
        int prio = 0;
        unordered_map<int, int> counts;
        priority_queue<tuple<int, int, int>> freqStack;
    };

    //-----------------------------------------------------------------------------
    // 763. Partition Labels (Medium)
    //-----------------------------------------------------------------------------
    class Solution763
    {
    public:
        vector<int> partitionLabels(string s)
        {
            // "ababcbacadefegdehijhklij"
            //          ^      ^       ^
            // Idea: The crucial part is to know the last position of a letter.
            // For example, the last position of 'a' is 8.
            vector<int> result;

            vector<int> letterPosMap(26, 0);
            for (int i = 0; i < s.size(); ++i)
            {
                letterPosMap[s[i] - 'a'] = i;
            }

            int start = 0;  // Start position of the partition.
            int miniPos = 0;  // Minimum ending position of the partition.
            for (int i = 0; i < s.size(); ++i)
            {
                // If we encounter 'a', which last position is 8, the then
                // minimum ending position is 8.
                miniPos = max(miniPos, letterPosMap[s[i] - 'a']);

                if (i >= miniPos && i == letterPosMap[s[i] - 'a'])
                {
                    result.push_back(i - start + 1);
                    start = i + 1;
                    miniPos = start;
                }
            }

            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 957. Prison Cells After N Days (Medium)
    //-----------------------------------------------------------------------------
    class Solution957
    {
    public:
        vector<int> prisonAfterNDays(vector<int>& cells, int n)
        {
            const int len = cells.size();
            // Great n would cause TLE if we use brute force approach.
            // The key is to know there is actually a cycle for certain n number.
            unordered_map<string, int> cache;
            string cell = convertToString(cells);
            for (int i = 0; i < n; ++i)
            {
                string newCur(8, '0');
                for (int j = 1; j < len - 1; ++j)
                {
                    if (cell[j - 1] == cell[j + 1])
                    {
                        newCur[j] = '1';
                    }
                    else
                    {
                        newCur[j] = '0';
                    }
                }

                cell = newCur;
                if (cache.count(cell) == 0)
                {
                    cache[cell] = i;
                }
                else
                {
                    int cycle = i - cache[cell];
                    // The cycle occurs every 14 turns, not including the 1st day.
                    // For example, n = 15, i = 14, cycle = 14. n will be 0. This is the first time
                    // we find the same cell (cache[0).
                    n = ( n - 1 ) % cycle;
                    for (const auto& pair : cache)
                    {
                        if (pair.second == n)
                        {
                            cell = pair.first;
                            break;
                        }
                    }

                    break;
                }
            }

            vector<int> res;
            for (char c : cell) res.push_back(c - '0');

            return res;
        }

    private:
        string convertToString(const vector<int>& cells)
        {
            string result;
            for (int i = 0; i < cells.size(); ++i)
            {
                result.push_back( cells[i] == 0 ? '0' : '1' );
            }

            return result;
        }
    };




    void TestDesignAndOthers()
    {
        // 895. Maximum Frequency Stack (Hard)
        FreqStack2 freqStack;
        freqStack.push(5); // The stack is [5]
        freqStack.push(7); // The stack is [5,7]
        freqStack.push(5); // The stack is [5,7,5]
        freqStack.push(7); // The stack is [5,7,5,7]
        freqStack.push(4); // The stack is [5,7,5,7,4]
        freqStack.push(5); // The stack is [5,7,5,7,4,5]
        int result = freqStack.pop();   // return 5, as 5 is the most frequent. The stack becomes [5,7,5,7,4].
        result = freqStack.pop();   // return 7, as 5 and 7 is the most frequent, but 7 is closest to the top. The stack becomes [5,7,5,4].
        result = freqStack.pop();   // return 5, as 5 is the most frequent. The stack becomes [5,7,4].
        result = freqStack.pop();   // return 4, as 4, 5 and 7 is the most frequent, but 4 is closest to the top. The stack becomes [5,7].

        // 763. Partition Labels (Medium)
        cout << "\n763. Partition Labels: " << endl;
        // Input: s = "ababcbacadefegdehijhklij"
        // Output: [9, 7, 8]
        Solution763 sol763;
        auto resultVI = sol763.partitionLabels("ababcbacadefegdehijhklij");
        PrintVector(resultVI);

        // 957. Prison Cells After N Days (Medium)
        // Input: cells = [0,1,0,1,1,0,0,1], n = 7
        // Output: [0, 0, 1, 1, 0, 0, 0, 0]
        // Input: cells = [1,0,0,1,0,0,1,0], n = 1000000000
        // Output: [0, 0, 1, 1, 1, 1, 1, 0]
        // Input: cells = [1,1,0,1,1,0,0,1], n = 300663720
        // Output: [0,0,1,0,0,1,1,0]

        Solution957 sol957;
        vector<int> inputVI;
        BuildIntVectorFromString("[0,1,0,1,1,0,0,1]", &inputVI);
        resultVI = sol957.prisonAfterNDays(inputVI, 15);
        cout << "\n957. Prison Cells After N Day: " << endl;
        PrintVector(resultVI);
    }
}