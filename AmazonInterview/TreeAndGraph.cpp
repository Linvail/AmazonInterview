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

    //-----------------------------------------------------------------------------
    // 127. Word Ladder (Hard)
    //-----------------------------------------------------------------------------
    class Solution127
    {
    public:

        bool linkAble(const string& word1, const string& word2)
        {
            int misCount = 0;
            if (word1.size() != word2.size()) return false;

            for (int i = 0; i < word1.size(); i++)
            {
                if (word1[i] != word2[i])
                {
                    misCount++;
                    if (misCount > 1)
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        int ladderLength(string beginWord, string endWord, vector<string>& wordList)
        {
            unordered_map<string, unordered_set<string>> graph;

            // Build graph for wordList.
            for (int i = 0; i < wordList.size(); i++)
            {
                const string& word1 = wordList[i];
                for (int j = i + 1; j < wordList.size(); ++j)
                {
                    const string& word2 = wordList[j];
                    if (linkAble(word1, word2))
                    {
                        graph[word1].insert(word2);
                        graph[word2].insert(word1);
                    }
                }

                // If this word has not link so far, we still need to insert it because
                // the beginWord might link to it.
                if (graph.count(word1) == 0)
                {
                    graph.emplace(word1, unordered_set<string>());
                }
            }
            // If endWord doesn't exist in wordList, just return 0.
            if (graph.count(endWord) == 0) return 0;

            int result = 0;
            unordered_set<string> visited;
            queue<string> unprocessed;
            // Put the nodes that is linked to the beginWord into the unprocessed queue.
            for (auto& word : graph)
            {
                if (linkAble(beginWord, word.first))
                {
                    if (result == 0)
                    {
                        result = 1;
                    }
                    unprocessed.push(word.first);
                    visited.insert(word.first);
                }
            }

            // Do BFS.
            while (!unprocessed.empty())
            {
                const int len = unprocessed.size();

                for (int i = 0; i < len; ++i)
                {
                    const string curr = unprocessed.front();
                    unprocessed.pop();

                    if (curr == endWord)
                    {
                        return result + 1;
                    }

                    // Do BFS for all unvisited nearby nodes.
                    for (const auto& near : graph[curr])
                    {
                        if (visited.count(near) == 0)
                        {
                            unprocessed.push(near);
                            visited.insert(near);
                        }
                    }
                }

                result++;
            }

            return 0;
        }
    };

    //-----------------------------------------------------------------------------
    // 126. Word Ladder II
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
                    vector<string> currPath = paths.front();
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
                                    result.push_back(newPath);
                                }
                                else
                                {
                                    paths.push(newPath);
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
    // Test function.
    //-----------------------------------------------------------------------------
    void TestTreeAndGraph()
    {
        // 127. Word Ladder
        // Input: beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log","cog"]
        // Output: 5
        // Explanation: One shortest transformation sequence is "hit" -> "hot" -> "dot" -> "dog" -> cog", which is 5 words long.
        // Input: beginWord = "hog", endWord = "cog", wordList = ["cog"]
        // Output: 2
        vector<string> wordList = { "cog" };
        Solution127 sol27;
        cout << "\n127. Word Ladder: " << sol27.ladderLength("hog", "cog", wordList);
        cout << endl;

        // 126. Word Ladder II
        // Input: beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log","cog"]
        // Output: [["hit", "hot", "dot", "dog", "cog"], ["hit", "hot", "lot", "log", "cog"]]
        wordList = { "hot","dot","dog","lot","log","cog" };
        Solution126 sol26;
        cout << "\n126. Word Ladder II: ";
        auto resultVVS = sol26.findLadders("hit", "cog", wordList);
        LeetCodeUtil::PrintMatrix(resultVVS);
        cout << endl;
    }
}