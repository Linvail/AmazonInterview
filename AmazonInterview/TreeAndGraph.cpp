#include "TreeAndGraph.h"
#include "LeetCodeUtil.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <set>
#include <map>

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
            // This looks like a graph problem.
            // We can solve it by BFS.
            // In a matrix, we can move in 4 directions. In this question, for each character
            // in a word, we can change in 25 directions.
            // A move is to change one character to another. A move is eligible when the new
            // string exists in the wordSet.
            // Then, we add the new string into queue and remove it from wordSet. It is just like
            // marking it has been visited and cannot be visited again.
            unordered_set<string> wordSet(wordList.begin(), wordList.end());

            // If endWord doesn't exist in wordList, just return 0.
            if (wordSet.find(endWord) == wordSet.end()) return 0;

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
                    // Iterate every char in lastWord.
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
                                if (tempWord == endWord)
                                {
                                    return level;
                                }
                                else
                                {
                                    paths.push(tempWord);
                                }

                                // If we jump from "hit" to "hot" and put "hot" into the queue.
                                // We should not ever jump to "hot" again, so we must remove it.
                                wordSet.erase(tempWord);
                            }
                        }
                    }
                }
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
    // 1597. Build Binary Expression Tree From Infix Expression (Hard)
    //
    // Similar questions:
    // Basic Calculator III
    // Check If Two Expression Trees are Equivalent
    //-----------------------------------------------------------------------------
    class Solution1597
    {
    public:
        TreeNode* expTree(const string& inputString)
        {
            // "2-3/(5*2)+1"
            // Idea:
            // 1. Scan infix expression from left to right. Build tree structure.
            // For example, for "2-3", create a node(-), a node(2) and, a node(3).
            // 2. For (...), call expTree() recursively for the string within.
            // 3. Process * and / first, then process + and -. Process them by
            // building the tree structure. For example, For nodes: '2', '-', '3',
            // return a deque containing node '-' and '2' is the left child and '3'
            // is the right child.
            deque<TreeNode*> nodeQueue;
            for (int i = 0; i < inputString.size(); ++i)
            {
                if (inputString[i] != '(')
                {
                    // No need to check ')', we will skip it when we get '('.
                    nodeQueue.push_back(new TreeNode(inputString[i]));
                }
                else
                {
                    // We may encounter another '('. Need to include it.
                    int remainingLeft = 1;
                    int j = i + 1;
                    for (; j < inputString.size(); ++j)
                    {
                        if (inputString[j] == '(') remainingLeft++;
                        if (inputString[j] == ')') remainingLeft--;
                        if (remainingLeft == 0) break;
                    }
                    // ( 1 + 2 )
                    // 0 1 2 3 4
                    string exp = inputString.substr(i + 1, j - i - 1);
                    // Call recursively.
                    nodeQueue.push_back(expTree(exp));
                    i = j;
                }
            }

            auto temp = processOperator(nodeQueue, '*', '/');
            temp = processOperator(temp, '+', '-');

            return temp.front();
        }

    private:

        deque<TreeNode*> processOperator(const deque<TreeNode*>& nodes, char op1, char op2)
        {
            deque<TreeNode*> result;
            for (int i = 0; i < nodes.size(); ++i)
            {
                // deque: 1 * 2 + 3 => deque:  *  +  3
                //                            / \
                //                           1   2
                // Note that we could meet a operator node here, but if it has children, then
                // it should be regarded as a operand.
                if (nodes[i]->left == nullptr &&
                    ( nodes[i]->val == op1 || nodes[i]->val == op2 ))
                {
                    // Take out the last node from the queue and assign it to the left child node.
                    nodes[i]->left = result.back();
                    nodes[i]->right = nodes[i + 1];
                    // Overwrite the last node of the queue.
                    result.back() = nodes[i];
                    i++;
                }
                else
                {
                    result.push_back(nodes[i]);
                }
            }
            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 1730. Shortest Path to Get Food (Medium)
    //-----------------------------------------------------------------------------
    class Solution1730
    {
    public:
        int getFood(vector<vector<char>>& grid)
        {
            // Idea: BFS.
            // Any food will do, so we can start traversal from all food toward
            // the starting point.
            queue<pair<int, int>> unprocessed;
            const int m = static_cast<int>( grid.size() );
            const int n = static_cast<int>( grid[0].size() );

            for (int i = 0; i < m; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    if (grid[i][j] == '#')
                    {
                        // Put all food positions.
                        unprocessed.emplace(i, j);
                        grid[i][j] = 'X';
                    }
                }
            }

            vector<pair<int, int>> dirs{ {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

            int steps = 0;
            while (!unprocessed.empty())
            {
                steps++;
                const size_t count = unprocessed.size();
                for (size_t i = 0; i < count; ++i)
                {
                    const int x = unprocessed.front().first;
                    const int y = unprocessed.front().second;
                    unprocessed.pop();

                    for (int j = 0; j < dirs.size(); ++j)
                    {
                        const int newX = x + dirs[j].first;
                        const int newY = y + dirs[j].second;

                        if (newX < 0 || newX >= m || newY < 0 || newY >= n || grid[newX][newY] == 'X')
                        {
                            continue;
                        }

                        if (grid[newX][newY] == '*')
                        {
                            return steps;
                        }

                        // Mark this cell as obstacle since it has been visited.
                        grid[newX][newY] = 'X';
                        unprocessed.emplace(newX, newY);
                    }
                }
            }

            return -1;
        }
    };

    //-----------------------------------------------------------------------------
    // 863. All Nodes Distance K in Binary Tree (Medium)
    // This is nearly Hard
    //-----------------------------------------------------------------------------
    class Solution863
    {
    public:
        vector<int> distanceK(TreeNode* root, TreeNode* target, int k)
        {
            // Build link to the parent.
            unordered_map<TreeNode*, TreeNode*> link2Parent;
            linkParent(root, link2Parent);

            // From target node, do BFS to until reaching the k level.
            queue<TreeNode*> nodeQueue{ {target} };
            unordered_set<TreeNode*> visited{ target };

            vector<int> result;

            while (!nodeQueue.empty())
            {
                const size_t count = nodeQueue.size();
                for (size_t i = 0; i < count; ++i)
                {
                    TreeNode* curr = nodeQueue.front();
                    nodeQueue.pop();
                    if(k == 0)
                    {
                        result.push_back(curr->val);
                        continue;
                    }

                    if (curr->left && visited.count(curr->left) == 0)
                    {
                        nodeQueue.push(curr->left);
                        visited.insert(curr->left);
                    }
                    if (curr->right && visited.count(curr->right) == 0)
                    {
                        nodeQueue.push(curr->right);
                        visited.insert(curr->right);
                    }
                    if (link2Parent[curr] && visited.count(link2Parent[curr]) == 0)
                    {
                        nodeQueue.push(link2Parent[curr]);
                        visited.insert(link2Parent[curr]);
                    }
                }
                k--;
            }

            return result;
        }

    private:
        void linkParent(TreeNode* root, unordered_map<TreeNode*, TreeNode*>& link2Parent)
        {
            if (!root) return;

            if (root->left)
            {
                link2Parent[root->left] = root;
                linkParent(root->left, link2Parent);
            }
            if (root->right)
            {
                link2Parent[root->right] = root;
                linkParent(root->right, link2Parent);
            }
        }
    };

    //-----------------------------------------------------------------------------
    // 582. Kill Process (Medium)
    //-----------------------------------------------------------------------------
    vector<int> killProcess(const vector<int>& pid, const vector<int>& ppid, int kill)
    {
        // Parent pid -> some children pid.
        unordered_map<int, vector<int>> childrenMap;
        const size_t len = pid.size();
        for (int i = 0; i < len; ++i)
        {
            childrenMap[ppid[i]].push_back( pid[i] );
        }

        vector<int> result;
        queue<int> unprocessed{ {kill} };
        while (!unprocessed.empty())
        {
            const int curr = unprocessed.front();
            result.push_back(curr);
            unprocessed.pop();

            for (const auto& child : childrenMap[curr])
            {
                unprocessed.push(child);
            }
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 909. Snakes and Ladders (Medium)
    //-----------------------------------------------------------------------------
    class Solution909
    {
    public:
        int snakesAndLadders(vector<vector<int>>& board)
        {
            n = board.size();

            int steps = 0;
            queue<int> unprocessed{ {1} };
            vector<int> visited(n * n, false);

            while (!unprocessed.empty())
            {
                const size_t len = unprocessed.size();
                for (size_t i = 0; i < len; ++i)
                {
                    const int num = unprocessed.front();
                    unprocessed.pop();
                    if (num == n * n)
                    {
                        return steps;
                    }

                    for (int j = 1; j <= 6 && num + j <= n * n; ++j)
                    {
                        int nextValue = getValueForNumber(board, num + j);
                        // If nextValue is not -1, then nextValue is just the next destination.
                        // Otherwise, the destination is num + j.
                        if (nextValue == -1)
                        {
                            nextValue = num + j;
                        }

                        if (visited[nextValue - 1]) continue;

                        unprocessed.push(nextValue);
                        visited[nextValue - 1] = true;
                    }
                }

                steps++;
            }

            return -1;
        }

    private:
        // The cell of the board is numbered.
        // We need a function to convert the number to coordinate (x, y), so
        // we can fetch the number at that coordinate (x, y).
        int getValueForNumber(const vector<vector<int>>& board, int number)
        {
            const size_t zeroBasedN = n - 1;
            size_t x = zeroBasedN - ( number - 1 ) / n;
            size_t y = ( (zeroBasedN - x) % 2 == 0 ) ? ( number - 1 ) % n : zeroBasedN - ( ( number - 1 ) % n );

            return board[x][y];
        }

        size_t n = 0;
    };

    //-----------------------------------------------------------------------------
    // 694. Number of Distinct Islands (Medium)
    //-----------------------------------------------------------------------------
    class Solution694
    {
    public:
        int numDistinctIslands(vector<vector<int>>& grid)
        {
            // The crucial part of this question is how to distinguish islands.
            // Luckily, std::vector can be stored in std::set and std::set is able
            // to distinguish and sort them pretty well.
            // So, we can regard the top-left cell as the root point and save root
            // point and all relative point of an island into a vector.
            // And then push this vector into a set.
            // The number of the elements in the set is the answer.
            set<vector<pair<int, int>>> islandSet;

            height = grid.size();
            width = grid[0].size();

            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (grid[i][j] != 1)
                    {
                        continue;
                    }

                    vector<pair<int, int>> island;
                    dfsHelper(grid, i, j, i, j, island);

                    // The set will not insert if it is already in the set.
                    islandSet.insert(island);
                }
            }

            return static_cast<int>(islandSet.size());
        }

    private:
        void dfsHelper(vector<vector<int>>& grid, int x, int y, int rootX, int rootY, vector<pair<int, int>>& island)
        {
            // Mark -1 as visited.
            grid[x][y] = -1;
            island.emplace_back(x - rootX, y - rootY);

            for (const auto& dir : dirs)
            {
                int newX = x + dir.first;
                int newY = y + dir.second;
                if (newX < 0 || newX >= height || newY < 0 || newY >= width || grid[newX][newY] != 1)
                {
                    continue;
                }
                dfsHelper(grid, newX, newY, rootX, rootY, island);
            }
        }

    private:

        const vector<pair<int, int>> dirs{ {0,-1},{-1,0},{0,1},{1,0} };

        size_t height = 0;
        size_t width = 0;
    };

    //-----------------------------------------------------------------------------
    // 1557. Minimum Number of Vertices to Reach All Nodes (Medium)
    //-----------------------------------------------------------------------------
    vector<int> findSmallestSetOfVertices(int n, vector<vector<int>>& edges)
    {
        vector<int> inDegree(n, 0);

        for (const auto& edge : edges)
        {
            inDegree[edge[1]]++;
        }

        vector<int> result;
        for (int i = 0; i < n; ++i)
        {
            if (inDegree[i] == 0)
            {
                result.push_back(i);
            }
        }

        return result;
    }

    //-----------------------------------------------------------------------------
    // 1091. Shortest Path in Binary Matrix (Medium)
    //-----------------------------------------------------------------------------
    class Solution1091
    {
    public:
        int shortestPathBinaryMatrix(vector<vector<int>>& grid)
        {
            // Constraint: 1 <= n <= 100
            const size_t n = grid.size();
            if (n < 1 || grid[0][0] != 0)
            {
                return -1;
            }

            struct intPairHash
            {
                // Note this function must be const!
                unsigned long long operator()(const pair<int, int>& p) const
                {
                    return ( static_cast<unsigned long long>( p.first ) << 32 ) | static_cast<unsigned long long>( p.second );
                }
            };

            // Use BFS to find the shortest path.
            unordered_set<pair<int, int>, intPairHash> visited;
            deque<pair<int, int>> unprocessed;
            // Put starting point.
            unprocessed.emplace_back(0, 0);

            vector<pair<int, int>> dirs = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1} };

            int steps = 1;
            while (!unprocessed.empty())
            {
                const size_t count = unprocessed.size();
                for (size_t i = 0; i < count; ++i)
                {
                    pair<int, int> p = unprocessed.front();
                    unprocessed.pop_front();
                    if (p.first == n - 1 && p.second == n - 1)
                    {
                        // Reach!
                        return steps;
                    }

                    for (size_t j = 0; j < dirs.size(); ++j)
                    {
                        const int newX = p.first + dirs[j].first;
                        const int newY = p.second + dirs[j].second;

                        if (newX < 0 || newX >= n || newY < 0 || newY >= n ||
                            grid[newX][newY] != 0 || visited.count({ newX, newY }) > 0)
                        {
                            continue;
                        }

                        visited.emplace(newX, newY);
                        unprocessed.emplace_back(newX, newY);
                    }
                }
                steps++;
            }

            return -1;
        }
    };

    //-----------------------------------------------------------------------------
    // 1197. Minimum Knight Moves (Medium)
    //-----------------------------------------------------------------------------
    class Solution1197
    {
    public:
        int minKnightMoves(int x, int y)
        {
            // It requires minimum number of steps, so we should be able to use
            // BFS.
            // The regular BFS would cause TLE. To overcome, we can restrict the
            // problem in 1st quadrant.
            x = abs(x);
            y = abs(y);

            int steps = 0;
            vector<pair<int, int>> dirs = {
                {-2, 1},
                {-1, 2},
                {1, 2},
                {2, 1},
                {2, -1},
                {1, -2},
                {-1, -2},
                {-2, -1}
            };

            queue<pair<int, int>> unprocessed;
            unprocessed.emplace(0, 0);

            // Using unordered_set<string> would case TLE.

            struct intPairHash
            {
                // Note this function must be const!
                unsigned long long operator()(const pair<int, int>& p) const
                {
                    return ( static_cast<unsigned long long>( p.first ) << 32 ) | static_cast<unsigned long long>( p.second );
                }
            };

            unordered_set<pair<int, int>, intPairHash> visited;
            visited.emplace(0, 0);

            while (!unprocessed.empty())
            {
                const size_t len = unprocessed.size();
                for (size_t i = 0; i < len; ++i)
                {
                    pair<int, int> cur = unprocessed.front();
                    unprocessed.pop();

                    if (cur.first == x && cur.second == y)
                    {
                        return steps;
                    }

                    for (const auto& dir : dirs)
                    {
                        const int newX = cur.first + dir.first;
                        const int newY = cur.second + dir.second;

                        if (newX < -1 || newY < -1 || visited.count({newX, newY}) > 0)
                        {
                            continue;
                        }

                        unprocessed.emplace(newX, newY);
                        visited.emplace(newX, newY);
                    }

                }

                steps++;
            }

            return -1;
        }
    };

    //-----------------------------------------------------------------------------
    // 688. Knight Probability in Chessboard (Medium)
    //
    // Topic: dynamic programming
    //-----------------------------------------------------------------------------
    class Solution688
    {
    public:
        using MovingRec = unordered_map<int, double>;

        double knightProbability(int n, int k, int row, int column)
        {
            // We can use DFS or BFS to traverse the table, but we need to a scheme
            // to reduce the duplicated calculation.
            // For <i, j>, we must remember a pair :
            // <remaining k moves, the ways of movement to keep the knight on the table>

            return traverse(n, k, row, column);
        }

    private:

        double traverse(int n, int k, int x, int y)
        {
            if (x < 0 || y < 0 || x >= n || y >= n) return 0;
            // No need to move, so the probability should be 1.
            if (k == 0) return 1;

            // Check if we have cached the result.
            if (m_table[{x, y}].count(k) > 0)
            {
                return m_table[{x, y}][k];
            }

            double tempProbability = 0;
            for (const auto& dir : m_dirs)
            {
                int newX = x + dir.first;
                int newY = y + dir.second;

                // Each step contributes 1/8.
                tempProbability += traverse(n, k - 1, newX, newY) / m_dirs.size();
            }

            m_table[{x, y}][k] = tempProbability;
            return tempProbability;
        }

        vector<pair<int, int>> m_dirs = {
            {-2, 1},
            {-1, 2},
            {1, 2},
            {2, 1},
            {2, -1},
            {1, -2},
            {-1, -2},
            {-2, -1}
        };

        struct IntPairHash
        {
            unsigned long long operator()(const pair<int, int>& p) const
            {
                return ( static_cast<unsigned long long>( p.first ) << 32 ) | static_cast<unsigned long long>( p.second );
            }
        };

        unordered_map<pair<int, int>, MovingRec, IntPairHash> m_table;
    };

    //-----------------------------------------------------------------------------
    // 979. Distribute Coins in Binary Tree (Medium)
    //
    // This is hard. The key is focus on leaf and use recursion to convey the
    // "need to movement" from down to up.
    //-----------------------------------------------------------------------------
    class Solution979
    {
    public:
        int distributeCoins(TreeNode* root)
        {
            int movements = 0;
            helper(root, movements);

            return movements;
        }

    private:
        //! Get how much the specified node can give upwardly.
        //! The negative number means it needs its parent to give it coin.
        int helper(TreeNode* node, int& movements)
        {
            if (!node) return 0;

            int left = helper(node->left, movements);
            int right = helper(node->right, movements);

            // No matter it is positive or negative, it is regarded as 1 movement.
            movements += abs(left) + abs(right);

            // Each node should maintain exactly 1 coin.
            // Exclude left/right, we should return 0 when it just has 1 coin, meaning it
            // doesn't give or ask any coin.
            return node->val + left + right - 1;
        }
    };

    //-----------------------------------------------------------------------------
    // 289. Game of Life (Medium)
    //
    // Follow up:
    // Could you solve it in-place? Remember that the board needs to be updated
    // simultaneously:
    // You cannot update some cells first and then use their updated values to
    // update other cells.
    // In this question, we represent the board using a 2D array.In principle, the
    // board is infinite, which would cause problems when the active area
    // encroaches upon the border of the array(i.e., live cells reach the border).
    // How would you address these problems ?
    //-----------------------------------------------------------------------------
    void gameOfLife(vector<vector<int>>& board)
    {
        // The key is to define transitional state for each cell to replace the
        // original 0/1 values.
        //
        //  0(00)  dead(next) < -dead(current)
        //  1(01)  live(next) < -live(current)
        //  2(10)  dead(next) < -live(current)
        //  3(11)  live(next) < -dead(current)

        const size_t m = board.size();
        const size_t n = board[0].size();

        vector<pair<int, int>> dirs = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };

        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                // Calculate how many live cells nearby.
                int count = 0;
                for (size_t k = 0; k < dirs.size(); ++k)
                {
                    const int x = static_cast<int>(i) + dirs[k].first;
                    const int y = static_cast<int>(j) + dirs[k].second;
                    // Need to understand the question very well.
                    // The question said: "The next state is created by applying the above rules simultaneously to every cell in the current state"
                    // So, only 1 or 2 can be considered as live cell.
                    if (x >= 0 && x < m && y >= 0 && y < n && ( board[x][y] == 1 || board[x][y] == 2 ))
                    {
                        ++count;
                    }
                }

                // Change the state. "> 0" is a convenient way, we actually want to test == 1 || == 3.
                if (board[i][j] > 0 && ( count < 2 || count > 3 ))
                {
                    // Become dead.
                    board[i][j] = 2;
                }
                else if (board[i][j] == 0 && count == 3)
                {
                    // Revive
                    board[i][j] = 3;
                }
            }
        }

        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                board[i][j] %= 2;
            }
        }
    }


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

        // 1597. Build Binary Expression Tree From Infix Expression (Hard)
        // Input: s = "2-3/(5*2)+1"
        // Output: [+, -, 1, 2, / , null, null, null, null, 3, *, null, null, 5, 2]
        Solution1597 sol1597;
        TreeNode* node = sol1597.expTree("(1+(2+3))*4");
        cout << "\n1597. Build Binary Expression Tree From Infix Expression: " << endl;
        PrintTreeLevelOrder(node);

        // 1730. Shortest Path to Get Food (Medium)
        // Input: grid = [["X","X","X","X","X","X"],["X","*","O","O","O","X"],["X","O","O","#","O","X"],["X","X","X","X","X","X"]]
        // Output: 3
        vector<vector<char>> inputVVS;
        BuildCharMatrixFromString("[[X,X,X,X,X,X],[X,*,O,O,O,X],[X,O,O,#,O,X],[X,X,X,X,X,X]]", &inputVVS);
        Solution1730 sol1730;
        cout << "\n1730. Shortest Path to Get Food: " << sol1730.getFood(inputVVS) << endl;

        // 863. All Nodes Distance K in Binary Tree (Medium)
        // Input: root = [3,5,1,6,2,0,8,null,null,7,4], target = 5, k = 2
        // Output: [7, 4, 1]
        // Input: root = [0,2,1,null,null,3] target = 3, k = 3
        // Output: [2]
        root = BuildTreeFromLevelOrderString("[0,2,1,null,null,3]");
        Solution863 sol863;
        TreeNode* target = root->right->left;
        auto resultVI = sol863.distanceK(root, target, 3);
        cout << "\n863. All Nodes Distance K in Binary Tree: " << endl;
        PrintVector(resultVI);

        // 582. Kill Process (Medium)
        // Input: pid = [1,3,10,5], ppid = [3,0,5,3], kill = 5
        // Output: [5, 10]
        cout << "\n582. Kill Process: " << endl;
        resultVI = killProcess({ 1,3,10,5 }, { 3,0,5,3 }, 5);
        LeetCodeUtil::PrintVector(resultVI);

        // 909. Snakes and Ladders (Medium)
        // Input: board = [[-1,-1,-1,-1,-1,-1],[-1,-1,-1,-1,-1,-1],[-1,-1,-1,-1,-1,-1],[-1,35,-1,-1,13,-1],[-1,-1,-1,-1,-1,-1],[-1,15,-1,-1,-1,-1]]
        // Output: 4
        // [[-1,-1,-1],[-1,9,8],[-1,8,9]]
        // 1
        // [[-1,-1,19,10,-1],[2,-1,-1,6,-1],[-1,17,-1,19,-1],[25,-1,20,-1,-1],[-1,-1,-1,-1,15]]
        // 2
        string inputStr = "[[-1,-1,-1,-1,-1,-1],[-1,-1,-1,-1,-1,-1],[-1,-1,-1,-1,-1,-1],[-1,35,-1,-1,13,-1],[-1,-1,-1,-1,-1,-1],[-1,15,-1,-1,-1,-1]]";
        vector<vector<int>> inputVVI;
        LeetCodeUtil::BuildIntMatrixFromString(inputStr, &inputVVI);
        Solution909 sol909;
        cout << "\n909. Snakes and Ladders: " << sol909.snakesAndLadders(inputVVI) << endl;

        // 694. Number of Distinct Islands (Medium)
        // Input: grid = [[1,1,0,0,0],[1,1,0,0,0],[0,0,0,1,1],[0,0,0,1,1]]
        // Output: 1
        inputStr = "[[0,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,1,0,0,1,1,1,0,1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0,1,0,0,0,0,0,1,1,1,1,0],[0,0,1,0,0,1,1,1,0,0,1,0,1,0,0,1,1,0,0,1,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,1,1,0,1,0,0,0],[0,1,0,1,0,1,1,1,0,0,1,1,0,0,0,0,1,0,1,0,1,1,1,0,1,1,1,0,0,0,1,0,1,0,1,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0],[1,0,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,0,0,0,0,1,0,1,0,0,1,0,1,1,1,0,1,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1]]";
        LeetCodeUtil::BuildIntMatrixFromString(inputStr, &inputVVI);
        Solution694 sol694;
        cout << "\n694. Number of Distinct Islands: " << sol694.numDistinctIslands(inputVVI) << endl;

        // 1557. Minimum Number of Vertices to Reach All Nodes (Medium)
        // Input: n = 6, edges = [[0,1],[0,2],[2,5],[3,4],[4,2]]
        // Output: [0, 3]
        LeetCodeUtil::BuildIntMatrixFromString("[[0,1],[0,2],[2,5],[3,4],[4,2]]", &inputVVI);
        resultVI = findSmallestSetOfVertices(6, inputVVI);
        cout << "\n1557. Minimum Number of Vertices to Reach All Nodes: " << endl;
        PrintVector(resultVI);

        // 1091. Shortest Path in Binary Matrix (Medium)
        // Input: grid = [[0,1],[1,0]]
        // Output: 2
        Solution1091 sol1091;
        inputStr = "[[0,1],[1,0]]";
        LeetCodeUtil::BuildIntMatrixFromString(inputStr, &inputVVI);
        cout << "\n1091. Shortest Path in Binary Matrix: " << sol1091.shortestPathBinaryMatrix(inputVVI) << endl;

        // 1197. Minimum Knight Moves (Medium)
        // Input: x = 2, y = 1
        // Output: 1
        // Input: x = 0, y = 1
        // Output : 3
        Solution1197 sol1197;
        cout << "\n1197. Minimum Knight Moves: " << sol1197.minKnightMoves(-87, -83) << endl;

        // 688. Knight Probability in Chessboard (Medium)
        // Input: n = 3, k = 2, row = 0, column = 0
        // Output: 0.06250
        Solution688 sol688;
        cout << "\n688. Knight Probability in Chessboard: " << sol688.knightProbability(3, 2, 0, 0) << endl;

        // 979. Distribute Coins in Binary Tree (Medium)
        // Input: root = [0,3,0]
        // Output: 3
        Solution979 sol979;
        root = BuildTreeFromLevelOrderString("[0,3,0]");
        cout << "\n979. Distribute Coins in Binary Tree: " << sol979.distributeCoins(root) << endl;

        // 289. Game of Life
        // Input: board = [[0, 1, 0], [0, 0, 1], [1, 1, 1], [0, 0, 0]]
        // Output : [[0, 0, 0], [1, 0, 1], [0, 1, 1], [0, 1, 0]]
        LeetCodeUtil::BuildIntMatrixFromString("[[0, 1, 0], [0, 0, 1], [1, 1, 1], [0, 0, 0]]", &inputVVI);
        gameOfLife(inputVVI);
        cout << "\n289. Game of Life: " << endl;
        LeetCodeUtil::PrintMatrix(inputVVI);
    }
}