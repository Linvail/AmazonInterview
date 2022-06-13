#include "LinkedList.h"

#include "LeetCodeUtil.h"

namespace LinkedList
{
    using namespace LeetCodeUtil;

    //-----------------------------------------------------------------------------
    // 25. Reverse Nodes in k-Group (Hard)
    //-----------------------------------------------------------------------------
    class Solution25
    {
    public:
        ListNode* reverseKGroup(ListNode* head, int k)
        {
            ListNode* dummy = new ListNode(-1);
            dummy->next = head;
            ListNode* curr = head;
            ListNode* prev = dummy;

            for (int i = 1; curr != nullptr; ++i)
            {
                if (i % k == 0)
                {
                    curr = curr->next;
                    prev = reverseOneGroup(prev, curr);
                }
                else
                {
                    curr = curr->next;
                }
            }

            return dummy->next;
        }

    private:
        ListNode* reverseOneGroup(ListNode* head, ListNode* tail)
        {
            ListNode* last = head->next;
            ListNode* curr = last;

            while (last->next != tail)
            {
                ListNode* nextNode = last->next;
                last->next = nextNode->next;
                nextNode->next = curr;
                curr = nextNode;
            }

            head->next = curr;

            return last;
        }
    };

    //-----------------------------------------------------------------------------
    // 21. Merge Two Sorted Lists (Easy)
    //-----------------------------------------------------------------------------
    class Solution21
    {
    public:
        static ListNode* mergeTwoLists(ListNode* list1, ListNode* list2)
        {
            if (!list1 && !list2)
            {
                return nullptr;
            }

            if (!list1 && list2)
            {
                return list2;
            }

            if (list1 && !list2)
            {
                return list1;
            }

            if (list1->val <= list2->val)
            {
                list1->next = mergeTwoLists(list1->next, list2);
                return list1;
            }
            else
            {
                list2->next = mergeTwoLists(list1, list2->next);
                return list2;
            }
        }

        static ListNode* mergeTwoLists_iterative(ListNode* list1, ListNode* list2)
        {
            if (!list1 && !list2)
            {
                return nullptr;
            }
            if (!list1 && list2)
            {
                return list2;
            }

            if (list1 && !list2)
            {
                return list1;
            }

            ListNode* dummy = new ListNode(-1);
            ListNode* result = dummy;

            while (list1 && list2)
            {
                if (list1->val <= list2->val)
                {
                    result->next = list1;
                    list1 = list1->next;
                }
                else
                {
                    result->next = list2;
                    list2 = list2->next;
                }
                result = result->next;
            }

            result->next = list1 != nullptr ? list1 : list2;

            return dummy->next;
        }
    };

    //-----------------------------------------------------------------------------
    // 206. Reverse Linked List (Easy)
    //-----------------------------------------------------------------------------
    //          head
    //            1 -> 2 -> 3 -> nullptr
    //
    // nullptr <- 1 <- 2 <- 3
    //                     head
    class Solution206
    {
    public:
        static ListNode* reverseList(ListNode* head)
        {
            ListNode* prev = nullptr;

            while (head)
            {
                ListNode* next = head->next;
                head->next = prev;
                prev = head;
                head = next;
            }

            return prev;
        }

        //! Idea 1
        //! 1. Divide the list in two parts - first node and rest of the linked list.
        //! 2. Call reverse for the rest of the linked list.
        //! 3. Link rest to first.
        //! 4. Fix head pointer
        //!        a | -> b -> c -> d -> null
        //!            <- b <- c <- d
        //!  null <- a <- b <- c <- d
        static ListNode* reverseListRecursiveV1(ListNode* head)
        {
            if (!head || !head->next)
            {
                return head;
            }

            // restHead should be d.
            ListNode* restHead = reverseListRecursiveV1(head->next);

            // Link b to a.
            head->next->next = head;

            // head is the tail now, so next should be nullptr.
            head->next = nullptr;

            return restHead;
        }

        //! Idea 2.
        //! 1. Remember the next.
        //! 2. Reverse self to set next to the previous.
        //! 3. Recursively call on the next until there is no next node.
        static ListNode* reverseListRecursiveV2(ListNode* head)
        {
            return reverseListRecursiveV2Impl(head, nullptr);
        }

        static ListNode* reverseListRecursiveV2Impl(ListNode* aHead, ListNode* aPreviousNode)
        {
            if (!aHead)
            {
                return aPreviousNode;
            }

            ListNode* nextNode = aHead->next;
            aHead->next = aPreviousNode;

            return reverseListRecursiveV2Impl(nextNode, aHead);
        }
    };

    //-----------------------------------------------------------------------------
    // 234. Palindrome Linked List (Easy)
    //-----------------------------------------------------------------------------
    class Solution234
    {
    public:
        static bool isPalindrome(ListNode* head)
        {
            if (!head)
            {
                return false;
            }
            else if (!head->next)
            {
                return true;
            }

            // Find the middle and reverse the second half list.
            ListNode* middle = findMiddle(head);
            middle = reverseList(middle);

            // Compare the first half list and second half list.
            while (head && middle)
            {
                if (head->val != middle->val)
                {
                    return false;
                }
                head = head->next;
                middle = middle->next;
            }

            return true;
        }

    private:
        //! 1 2 3 3 4 5
        //! If the list has even number of nodes, this will return the 2nd of the middle, which is the 2nd 3.
        static ListNode* findMiddle(ListNode* aHead)
        {
            ListNode* fast = aHead;
            ListNode* slow = aHead;

            while (fast != nullptr && fast->next != nullptr)
            {
                slow = slow->next;
                fast = fast->next->next;
            }
            return slow;
        }

        // Loop version. In leetcode, it seems slower.
        static ListNode* reverseList(ListNode* head)
        {
            ListNode* prev = nullptr;

            while (head)
            {
                ListNode* next = head->next;
                head->next = prev;
                prev = head;
                head = next;
            }

            return prev;
        }
    };

    //-----------------------------------------------------------------------------
    // Test function.
    //-----------------------------------------------------------------------------
    void TestLinkedList()
    {
        // 25. Reverse Nodes in k-Group (Hard)
        // Input: head = [1,2,3,4,5], k = 2
        // Output: [2, 1, 4, 3, 5]
        vector<int> inputIV = { 1,2,3,4,5 };
        ListNode* head = LeetCodeUtil::BuildLinkedListFromVector(inputIV);
        Solution25 sol25;
        ListNode* output = sol25.reverseKGroup(head, 3);
        cout << "\n25. Reverse Nodes in k-Group:";
        LeetCodeUtil::PrintListNode(output);
        cout << endl;

        // 21. Merge Two Sorted Lists (Easy)
        ListNode* list1 = BuildLinkedListFromVector({ 1, 2, 3 });
        ListNode* list2 = BuildLinkedListFromVector({ 2, 4, 5, 6 });
        cout << "\n21. Merge Two Sorted Lists (Easy):";
        LeetCodeUtil::PrintListNode(Solution21::mergeTwoLists_iterative(list1, list2));
        cout << endl;

        // 206. Reverse Linked List (Easy)
        vector<int> list = { 1, 2, 3, 4, 5 };
        ListNode* node1 = BuildLinkedListFromVector(list);
        cout << "\n206. Reverse Linked List (Easy): " << endl;
        ListNode* result = Solution206::reverseList(node1);
        PrintListNode(result);
        node1 = Solution206::reverseListRecursiveV1(result);
        PrintListNode(node1);
        result = Solution206::reverseListRecursiveV2(node1);
        PrintListNode(result);
        cout << endl;

        // 234. Palindrome Linked List (Easy)
        inputIV = { 1,2,3,3,2,1 };
        head = LeetCodeUtil::BuildLinkedListFromVector(inputIV);
        cout << "\n234. Palindrome Linked List (Easy): " << Solution234::isPalindrome(head) << endl;

    }
}