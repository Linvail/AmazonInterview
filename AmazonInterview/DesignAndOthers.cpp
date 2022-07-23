#include "DesignAndOthers.h"
#include "LeetCodeUtil.h"

#include <deque>
#include <map>
#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
#include <string>
#include <stack>

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
            const int len = static_cast<int>( cells.size() );
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

    //-----------------------------------------------------------------------------
    // 1268. Search Suggestions System (Medium)
    // Topic: Trie (prefix tree), binary search.
    //-----------------------------------------------------------------------------
    class Solution1268
    {
    public:
        vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord)
        {
            // The output must be sorted lexicographically. Since we need to sort,
            // we can sort the entire 'products' and apply binary search to find
            // the possible matched word.
            sort(products.begin(), products.end());

            vector<vector<string>> result;
            string partial;
            for (int i = 0; i < searchWord.size(); ++i)
            {
                vector<string> matches;
                partial.push_back(searchWord[i]);
                // Find the first not-less-than word.
                auto it = std::lower_bound(products.begin(), products.end(), partial);
                // Look forward and collect 3 words.
                for (int j = 0; j < 3 && it != products.end(); j++ )
                {
                    if (it->compare(0, i + 1, partial) != 0)
                    {
                        // Check if partial is prefix.
                        break;
                    }
                    matches.push_back(*it);
                    it++;
                }
                result.push_back(matches);
            }

            return result;
        }
    };

    //-----------------------------------------------------------------------------
    // 1628. Design an Expression Tree With Evaluate Function (Medium)
    //-----------------------------------------------------------------------------
    /**
     * This is the interface for the expression tree Node.
     * You should not remove it, and you can define some classes to implement it.
     */

    class Node
    {
    public:
        virtual ~Node() {};
        virtual int evaluate() const = 0;

        Node* left;
        Node* right;

    protected:
        // define your fields here
        static map<char, std::function<int(const Node*, const Node*)>> operatorMap;
    };

    map<char, std::function<int(const Node*, const Node*)>> Node::operatorMap;

    class NodeImpl : public Node
    {
    public:

        NodeImpl(const string& value) : val(value) {}

        string val;

        static void initOperator();
        int evaluate() const override;
    };

    void NodeImpl::initOperator()
    {
        operatorMap['+'] = [](const Node* left, const Node* right) { return left->evaluate() + right->evaluate(); };
        operatorMap['-'] = [](const Node* left, const Node* right) { return left->evaluate() - right->evaluate(); };
        operatorMap['*'] = [](const Node* left, const Node* right) { return left->evaluate() * right->evaluate(); };
        operatorMap['/'] = [](const Node* left, const Node* right) { return left->evaluate() / right->evaluate(); };
    }

    int NodeImpl::evaluate() const
    {
        if (isdigit(val.back()))
        {
            return stoi(val);
        }

        int result = operatorMap[val.back()](left, right);
        return result;
    }

    /**
     * This is the TreeBuilder class.
     * You can treat it as the driver code that takes the postinfix input
     * and returns the expression tree represnting it as a Node.
     */

    class TreeBuilder
    {
    public:
        Node* buildTree(const vector<string>& postfix)
        {
            NodeImpl::initOperator();

            stack<Node*> opStack;
            for (int i = 0; i < postfix.size(); ++i)
            {
                const string& curr = postfix[i];
                if (isdigit(curr.back()))
                {
                    Node* newNode = new NodeImpl(curr);
                    opStack.push(newNode);
                }
                else
                {
                    Node* op1 = opStack.top();
                    opStack.pop();
                    Node* op2 = opStack.top();
                    opStack.pop();
                    Node* newNode = new NodeImpl(curr);
                    newNode->left = op2;
                    newNode->right = op1;
                    opStack.push(newNode);
                }
            }

            return opStack.top();
        }
    };

    /**
     * Your TreeBuilder object will be instantiated and called as such:
     * TreeBuilder* obj = new TreeBuilder();
     * Node* expTree = obj->buildTree(postfix);
     * int ans = expTree->evaluate();
     */

    //-----------------------------------------------------------------------------
    // 1603. Design Parking System (Easy)
    //-----------------------------------------------------------------------------
    class ParkingSystem
    {
    public:
        ParkingSystem(int big, int medium, int small) : parkingLot({ big, medium, small })
        {
        }

        bool addCar(int carType)
        {
            int& remaining = parkingLot[carType - 1];
            if (remaining > 0)
            {
                remaining--;
                return true;
            }
            else
            {
                return false;
            }
        }

    private:
        vector<int> parkingLot;
    };

    //-----------------------------------------------------------------------------
    // 772. Basic Calculator III (Hard)
    //
    // Similar questions:
    // Build Binary Expression Tree From Infix Expression (Solved in TreeAndGraph.cpp)
    //-----------------------------------------------------------------------------
    class Solution772
    {
    public:
        int calculate(const string& s)
        {
            // Idea: Use recursion for (XXXX) first.
            // For a string without (), like "2+2x4", we should process '*' and '/'
            // first, then process '+', '-'. Return the value in the end.
            int temp = -1;
            vector<Node> elements;
            for (int i = 0; i < s.size(); ++i)
            {
                if (s[i] != '(')
                {
                    if (!isdigit(s[i]))
                    {
                        if (temp != -1)
                        {
                            // A operand may follow a pair of (xxx). In that case, temp will be
                            // -1, we must not add it.
                            elements.emplace_back(false, temp);
                        }
                        elements.emplace_back(true, s[i]);
                        temp = -1;
                    }
                    else
                    {
                        temp = temp == -1 ? s[i] - '0' : temp * 10 + ( s[i] - '0' );
                    }
                }
                else
                {
                    temp = -1;
                    // We may encounter another '('. Need to include it.
                    int remainingLeft = 1;
                    int j = i + 1;
                    for (; j < s.size(); ++j)
                    {
                        if (s[j] == '(') remainingLeft++;
                        if (s[j] == ')') remainingLeft--;

                        if (remainingLeft == 0)
                        {
                            break;
                        }
                    }
                    // 1 * ( 2 + 3 )
                    //     ^       ^
                    //     2       6
                    const string sub = s.substr(i + 1, j - i - 1);
                    int val = calculate(sub);
                    elements.emplace_back(false, val);

                    i = j;
                }
            }
            if (temp != -1)
            {
                elements.emplace_back(false, temp);
            }

            if (elements.size() > 1)
            {
                processOperator(elements, '*', '/');
                if (elements.size() > 1)
                {
                    processOperator(elements, '+', '-');
                }
            }

            return elements.back().value;
        }

    private:

        struct Node
        {
            bool isOperand = false;
            int value = 0;

            Node(bool isOp, int val)
                : isOperand(isOp), value(val)
            {}
        };

        void processOperator(vector<Node>& nodes, char op1, char op2)
        {
            vector<Node> result;
            for (int i = 0; i < nodes.size(); ++i)
            {
                const auto& node = nodes[i];
                if (node.isOperand &&
                    (static_cast<char>( node.value ) == op1 || static_cast<char>( node.value ) == op2))
                {
                    const char op = static_cast<char>( node.value );
                    const int left = result.back().value;

                    if (op == '+')
                    {
                        result.rbegin()->value = left + nodes[i + 1].value;
                    }
                    else if (op == '-')
                    {
                        result.rbegin()->value = left - nodes[i + 1].value;
                    }
                    else if (op == '*')
                    {
                        result.rbegin()->value = left * nodes[i + 1].value;
                    }
                    else
                    {
                        result.rbegin()->value = left / nodes[i + 1].value;
                    }

                    i++;
                }
                else
                {
                    result.push_back(node);
                }
            }

            result.swap(nodes);
        }
    };

    //-----------------------------------------------------------------------------
    // 227. Basic Calculator II (Medium)
    //
    // Unlike "772. Basic Calculator III", there is no parentheses in the string.
    //-----------------------------------------------------------------------------
    class Solution227
    {
    public:
        int calculate(string s)
        {
            const size_t len = s.size();

            int temp = -1;
            vector<Node> elements;
            for (int i = 0; i < len; ++i)
            {
                if (s[i] == ' ')
                {
                    continue;
                }
                if (!isdigit(s[i]))
                {
                    elements.emplace_back(false, temp);
                    elements.emplace_back(true, s[i]);
                    temp = -1;
                }
                else
                {
                    temp = temp == -1 ? s[i] - '0' : temp * 10 + ( s[i] - '0' );
                }
            }
            if (temp != -1)
            {
                elements.emplace_back(false, temp);
            }

            processOperator(elements, '*', '/');
            processOperator(elements, '+', '-');

            return elements.back().value;
        }

    private:
        struct Node
        {
            bool isOperand = false;
            int value = 0;

            Node(bool isOp, int val)
                : isOperand(isOp), value(val)
            {}
        };

        void processOperator(vector<Node>& nodes, char op1, char op2)
        {
            vector<Node> result;
            for (int i = 0; i < nodes.size(); ++i)
            {
                const auto& node = nodes[i];
                if (node.isOperand &&
                    (static_cast<char>( node.value ) == op1 || static_cast<char>( node.value ) == op2))
                {
                    const char op = static_cast<char>( node.value );
                    const int left = result.back().value;

                    if (op == '+')
                    {
                        result.rbegin()->value = left + nodes[i + 1].value;
                    }
                    else if (op == '-')
                    {
                        result.rbegin()->value = left - nodes[i + 1].value;
                    }
                    else if (op == '*')
                    {
                        result.rbegin()->value = left * nodes[i + 1].value;
                    }
                    else
                    {
                        result.rbegin()->value = left / nodes[i + 1].value;
                    }

                    i++;
                }
                else
                {
                    result.push_back(node);
                }
            }

            result.swap(nodes);
        }
    };

    //-----------------------------------------------------------------------------
    // 1166. Design File System (Medium)
    //-----------------------------------------------------------------------------
    class FileSystem
    {
    public:
        FileSystem()
        {
        }

        bool createPath(string path, int value)
        {
            // Extract parent.
            string parent = path.substr(0, path.rfind('/'));
            // Return false if parent doesn't exist.
            if (!parent.empty() && path2Value.count(parent) == 0)
            {
                return false;
            }

            auto res = path2Value.emplace(path, value);
            return res.second;
        }

        int get(string path)
        {
            auto it = path2Value.find(path);
            if (it == path2Value.end())
            {
                return -1;
            }
            return it->second;
        }

    private:
        unordered_map<string, int> path2Value;
    };

    //-----------------------------------------------------------------------------
    // 353. Design Snake Game (Medium)
    //-----------------------------------------------------------------------------
    class SnakeGame
    {
    public:
        SnakeGame(int width, int height, const vector<vector<int>>& food)
            : m_snakeBody()
            , m_Food(food.begin(), food.end())
            , m_height(height)
            , m_width(width)
        {
            m_snakeBody.emplace_back(0, 0);
        }

        int move(string direction)
        {
            pair<int, int> dir;
            if (direction == "R")
            {
                dir = { 0, 1 };
            }
            else if (direction == "L")
            {
                dir = { 0, -1 };
            }
            else if (direction == "D")
            {
                dir = { 1, 0 };
            }
            else
            {
                dir = { -1, 0 };
            }

            const int newX = m_snakeBody.back().first + dir.first;
            const int newY = m_snakeBody.back().second + dir.second;
            // Start from begin() + 1 because we want to exclude the tail.
            bool touch = std::find(m_snakeBody.begin() + 1, m_snakeBody.end(), make_pair(newX, newY)) != m_snakeBody.end();

            if (newX < 0 || newX >= m_height ||
                newY < 0 || newY >= m_width || touch)
            {
                return -1;
            }

            if (m_Food.empty() || m_Food.begin()->at(0) != newX || m_Food.begin()->at(1) != newY)
            {
                // No food here. Remove the tail.
                m_snakeBody.erase(m_snakeBody.begin());
            }
            else
            {
                // A food got eaten. Remove it.
                m_Food.erase(m_Food.begin());
            }

            // Add new head.
            m_snakeBody.emplace_back(newX, newY);

            return m_snakeBody.size() - 1;
        }

    private:
        // Record snake's body.
        deque<pair<int, int>> m_snakeBody;
        deque<vector<int>> m_Food;

        int m_height;
        int m_width;
    };

    //-----------------------------------------------------------------------------
    // Test function
    //-----------------------------------------------------------------------------
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

        // 1268. Search Suggestions System (Medium)
        // Input: products = ["mobile","mouse","moneypot","monitor","mousepad"], searchWord = "mouse"
        // Output: [
        // ["mobile", "moneypot", "monitor"],
        // ["mobile", "moneypot", "monitor"],
        // ["mouse", "mousepad"],
        // ["mouse", "mousepad"],
        // ["mouse", "mousepad"]
        // ]

        vector<string> inputVS = { "mobile","mouse","moneypot","monitor","mousepad" };
        Solution1268 sol1268;
        auto resultVVS = sol1268.suggestedProducts(inputVS, "mouse");
        cout << "\n1268. Search Suggestions System: " << endl;
        LeetCodeUtil::PrintMatrix(resultVVS);

        // 1628. Design an Expression Tree With Evaluate Function (Medium)
        // Input: s = ["3","4","+","2","*","7","/"]
        // Output: 2
        TreeBuilder builder;
        Node* root = builder.buildTree({ "3","4","+","2","*","7","/" });
        cout << "\n1628. Design an Expression Tree With Evaluate Function: " << root->evaluate() << endl;

        // 772. Basic Calculator III (Hard)
        // Input: s = "6-4/2"
        // Output: 4
        // Input: s = "2*(5+5*2)/3+(6/2+8)"
        // Output: 21
        Solution772 sol772;
        cout << "\n772. Basic Calculator III: " << sol772.calculate("(9568+(9040-(380+(2042-(7115)+(6294)-(4395-(5183+9744+(7746-(1099+2718))-(9370-(8561+(9302)-(7632+(8451-(1759+(7760))-(3377+5363+9093+(8332-(4492-(1151+(1165-8833+(775+(3749)+9399))+9112+(6273+(7285-(6112-(668-(7756-4316-(582+1835-(6644+690+1204-(7197+(7897))+(7009-(7262))-7782-(7858+(7644+(9461+(2224)-(7531-1095-(891+1022)+2197-(9855)))+(6663-(7417-(6158-(3610))+(1481))-(4182+(4761)))+(5017))+(9990)+(6218)))-(2904)+(5631)-(8888)+3401+(3569))+(1135))-(3695-(7713+(3479)-(9813+(8171+(8616-8026+(4634-(6973))-(9761-(623-4782)+(2514)+(6233)))))+(6140))-(6641)-8611+(8389)+8074-(4412))-(3703)+(9688+(9513))))-(4987)))+(9647)))))))))-(2299))-(4785))))))") << endl;

        // 227. Basic Calculator II (Medium)
        // Input: s = "3+2*2"
        // Output: 7
        Solution227 sol227;
        cout << "\n227. Basic Calculator II: " << sol227.calculate("3+2*2") << endl;

        // 353. Design Snake Game (Medium)
        // ["SnakeGame","move","move","move","move","move","move","move","move","move","move","move","move"]
        // [[3, 3, [[2, 0], [0, 0], [0, 2], [2, 2]] ], ["D"], ["D"], ["R"], ["U"], ["U"], ["L"], ["D"], ["R"], ["R"], ["U"], ["L"], ["D"]]
        // Expect: [null, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3]
        SnakeGame snakeGame(3, 3, { {2, 0},{0, 0},{0, 2}, {2, 2} });

        snakeGame.move("D");
        snakeGame.move("D");
        snakeGame.move("R");
        snakeGame.move("U");
        snakeGame.move("U");
        snakeGame.move("L");
        snakeGame.move("D");
        snakeGame.move("R");
        snakeGame.move("R");
        snakeGame.move("U");
        snakeGame.move("L");
        snakeGame.move("D");
    }
}