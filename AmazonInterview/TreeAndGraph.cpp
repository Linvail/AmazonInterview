#include "TreeAndGraph.h"
#include "LeetCodeUtil.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>

namespace TreeAndGraph
{
    using namespace std;
    using namespace LeetCodeUtil;

    //-----------------------------------------------------------------------------
    // 127. Word Ladder (Hard)
    //-----------------------------------------------------------------------------
    class Solution127
    {
    public:

        int ladderLength(string beginWord, string endWord, vector<string>& wordList)
        {
            unordered_set<string> wordSet(wordList.begin(), wordList.end());

            // If endWord doesn't exist in wordList, just return 0.
            if (wordSet.find(endWord) == wordSet.end()) return 0;

            unordered_set<string> visited;
            queue<string> paths;
            paths.push(beginWord);
            int level = 1;

            while (!paths.empty())
            {
                level++;
                const size_t queueSize = paths.size();
                for (int i = 0; i < queueSize; i++)
                {
                    string lastWord = paths.front();
                    paths.pop();
                    const size_t len = lastWord.size();
                    // Iterate every char
                    for (int j = 0; j < len; j++)
                    {
                        string tempWord(lastWord);
                        // Iterate every lower case English letter
                        for (char ch = 'a'; ch <= 'z'; ch++)
                        {
                            if (ch == lastWord[j]) continue;
                            tempWord[j] = ch;
                            if (wordSet.find(tempWord) != wordSet.end())
                            {
                                visited.insert(tempWord);

                                if (tempWord == endWord)
                                {
                                    return level;
                                }
                                else
                                {
                                    paths.push(tempWord);
                                }
                            }

                        }
                    }
                }

                // This is tricky. Note that we process the paths with the same length in the for-loop.
                // Within the for-loop, the words put into 'visited' must be still usable for the other paths
                // with the same length. Once we finished one loop, those visited words should not be usable anymore.
                // The path should go back, otherwise we will enter infinite loop.
                for (const auto& word : visited)
                {
                    wordSet.erase(word);
                }
                visited.clear();
            }

            return 0;
        }
    };

    //-----------------------------------------------------------------------------
    // 126. Word Ladder II (Hard)
    // Very Hard.
    //-----------------------------------------------------------------------------
    class Solution126
    {
    public:

        vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList)
        {
            vector<vector<string>> result;
            unordered_set<string> wordSet(wordList.begin(), wordList.end());

            // If endWord doesn't exist in wordList, just return empty result.
            if (wordSet.find(endWord) == wordSet.end()) return result;

            unordered_set<string> visited;
            queue<vector<string>> paths;
            paths.push({ beginWord });

            while (!paths.empty())
            {
                const size_t queueSize = paths.size();
                for (int i = 0; i < queueSize; i++)
                {
                    vector<string> currPath;
                    currPath.swap(paths.front());
                    paths.pop();
                    string lastWord = currPath.back();
                    const size_t len = lastWord.size();
                    // Iterate every char
                    for (int j = 0; j < len; j++)
                    {
                        string tempWord(lastWord);
                        // Iterate every lower case English letter
                        for (char ch = 'a'; ch <= 'z'; ch++)
                        {
                            if (ch == lastWord[j]) continue;
                            tempWord[j] = ch;
                            if (wordSet.find(tempWord) != wordSet.end())
                            {
                                vector<string> newPath(currPath);
                                newPath.push_back(tempWord);
                                visited.insert(tempWord);

                                if (tempWord == endWord)
                                {
                                    result.push_back(std::move(newPath));
                                }
                                else
                                {
                                    paths.push(std::move(newPath));
                                }
                            }

                        }
                    }
                }

                // This is tricky. Note that we process the paths with the same length in the for-loop.
                // Within the for-loop, the words put into 'visited' must be still usable for the other paths
                // with the same length. Once we finished one loop, those visited words should not be usable anymore.
                // The path should go back, otherwise we will enter infinite loop.
                for (const auto& word : visited)
                {
                    wordSet.erase(word);
                }
                visited.clear();
            }

            return result;
        }

    };

    //-----------------------------------------------------------------------------
    // 543. Diameter of Binary Tree (Easy)
    //-----------------------------------------------------------------------------
    class Solution543
    {
    public:
        int diameterOfBinaryTree(TreeNode* root)
        {
            if (!root) return 0;
            // The diameter is related to depth of subtrees.
            // It could be sum of the depth of left/right subtrees, or the diameter of
            // left/right subtrees. We should take the maximum among the 3 values.
            int diameter = 0;

            int sumOfDepth = maxDepth(root->left) + maxDepth(root->right);

            diameter = max(sumOfDepth, max(diameterOfBinaryTree(root->left), diameterOfBinaryTree(root->right)));

            return diameter;
        }

    private:

        int maxDepth(TreeNode* root)
        {
            if (!root) return 0;

            if (nodeDepthMap.count(root) > 0)
                return nodeDepthMap[root];

            int depth = 1 + max(maxDepth(root->left), maxDepth(root->right));
            nodeDepthMap[root] = depth;

            return depth;
        }

        unordered_map<TreeNode*, int> nodeDepthMap;
    };

    //-----------------------------------------------------------------------------
    // 675. Cut Off Trees for Golf Event (Hard)
    //
    // * You must cut off the trees in order from shortest to tallest.
    //-----------------------------------------------------------------------------
    class Solution675
    {
    public:

        struct Cell
        {
            short x;
            short y;
            int height;

            Cell(int i, int j, int k) : x(i), y(j), height(k) {}
        };

        int cutOffTree(vector<vector<int>>& forest)
        {
            m = static_cast<int>( forest.size() );
            n = static_cast<int>( forest[0].size() );

            deque<Cell> cellQueue;
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (forest[i][j] > 1)
                        cellQueue.emplace_back(i, j, forest[i][j] );
                }
            }
            // Need to sort tree by height first. Sort in ascending order.
            sort(cellQueue.begin(), cellQueue.end(), [](const Cell& left, const Cell& right)
                {
                    return left.height < right.height;
                });

            int totalSteps = 0;
            int startX = 0;
            int startY = 0;
            // Use BFS to find the shortest path to the next shortest tree.
            for (int i = 0; i < cellQueue.size(); i++)
            {
                int steps = shortestPathToNextTree(forest, startX, startY, cellQueue[i]);
                if (steps == -1)
                {
                    return -1;
                }

                totalSteps += steps;
                startX = cellQueue[i].x;
                startY = cellQueue[i].y;
            }

            return totalSteps;
        }

    private:
        int shortestPathToNextTree(vector<vector<int>>& forest, int aX, int aY, const Cell& target)
        {
            if (aX == target.x && aY == target.y)
                return 0;

            queue<int> unvisited;
            unvisited.push(aX * n + aY);
            vector<vector<int>> enqueued(m, vector<int>(n));
            int steps = 0;

            vector<pair<int, int>> dirs = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

            while (!unvisited.empty())
            {
                steps++;
                int count = static_cast<int>( unvisited.size() );
                while (count--)
                {
                    const int x = unvisited.front() / n;
                    const int y = unvisited.front() % n;
                    unvisited.pop();

                    for (int i = 0; i < dirs.size(); i++)
                    {
                        const int newX = x + dirs[i].first;
                        const int newY = y + dirs[i].second;

                        if (newX == target.x && newY == target.y)
                        {
                            // We reach the target.
                            return steps;
                        }

                        if (newX < 0 || newY < 0 || newX >= m || newY >= n)
                            continue;

                        const int code = newX * n + newY;
                        if (enqueued[newX][newY] == 1 || forest[newX][newY] == 0)
                            continue;

                        unvisited.push(code);
                        enqueued[newX][newY] = 1;
                    }
                }
            }

            return -1;
        }

    private:
        int m = 0;
        int n = 0;
    };

    //-----------------------------------------------------------------------------
    // 733. Flood Fill (Easy)
    //-----------------------------------------------------------------------------
    class Solution733
    {
    public:
        vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color)
        {
            const int m = static_cast<int>( image.size() );
            const int n = static_cast<int>( image[0].size() );
            queue<pair<int, int>> unprocessed;
            unprocessed.push({ sr, sc });
            const int originalColor = image[sr][sc];

            if (originalColor == color) return image;

            while (!unprocessed.empty())
            {
                const int x = unprocessed.front().first;
                const int y = unprocessed.front().second;
                unprocessed.pop();
                image[x][y] = color;

                if (x > 0 && image[x - 1][y] == originalColor)
                {
                    unprocessed.push({ x - 1, y });
                }
                if (y > 0 && image[x][y - 1] == originalColor)
                {
                    unprocessed.push({ x, y - 1 });
                }
                if (x < m - 1 && image[x + 1][y] == originalColor)
                {
                    unprocessed.push({ x + 1, y });
                }
                if (y < n - 1 && image[x][y + 1] == originalColor)
                {
                    unprocessed.push({ x, y + 1 });
                }
            }

            return image;
        }
    };

    //-----------------------------------------------------------------------------
    // Test function.
    //-----------------------------------------------------------------------------
    void TestTreeAndGraph()
    {
        // 127. Word Ladder (Hard)
        // Input: beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log","cog"]
        // Output: 5
        // Explanation: One shortest transformation sequence is "hit" -> "hot" -> "dot" -> "dog" -> cog", which is 5 words long.
        // Input: beginWord = "hog", endWord = "cog", wordList = ["cog"]
        // Output: 2
        vector<string> wordList = { "hot","dot","dog","lot","log","cog" };
        Solution127 sol27;
        cout << "\n127. Word Ladder: " << sol27.ladderLength("hit", "cog", wordList);
        cout << endl;

        // 126. Word Ladder II (Hard)
        // Input: beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log","cog"]
        // Output: [["hit", "hot", "dot", "dog", "cog"], ["hit", "hot", "lot", "log", "cog"]]
        wordList = { "hot","dot","dog","lot","log","cog" };
        Solution126 sol26;
        cout << "\n126. Word Ladder II: ";
        auto resultVVS = sol26.findLadders("hit", "cog", wordList);
        LeetCodeUtil::PrintMatrix(resultVVS);
        cout << endl;

        // 543. Diameter of Binary Tree (Easy)
        // Input: root = [1,2,3,4,5]
        // Output: 3
        TreeNode* root = BuildTreeFromLevelOrderString("[1,2,3,4,5]");
        Solution543 sol543;
        cout << "\n543. Diameter of Binary Tree: " << sol543.diameterOfBinaryTree(root) << endl;

        // 733. Flood Fill (Easy)
        // Input: image = [[1, 1, 1], [1, 1, 0], [1, 0, 1]], sr = 1, sc = 1, color = 2
        // Output : [[2, 2, 2], [2, 2, 0], [2, 0, 1]]
        vector<vector<int>> image;
        BuildIntMatrixFromString("[[1, 1, 1], [1, 1, 0], [1, 0, 1]]", &image);
        Solution733 sol733;
        auto resultVVI = sol733.floodFill(image, 1, 1, 2);
        cout << "\n733. Flood Fill: " << endl;
        PrintMatrix(resultVVI);

        // 675. Cut Off Trees for Golf Event (Hard)
        // Input: forest = [[1, 2, 3], [0, 0, 4], [7, 6, 5]]
        // Output : 6
        BuildIntMatrixFromString("[[1, 2, 3], [0, 0, 4], [7, 6, 5]]", &image);
        Solution675 sol675;
        cout << "\n675. Cut Off Trees for Golf Event: " << sol675.cutOffTree(image) << endl;

    }
}