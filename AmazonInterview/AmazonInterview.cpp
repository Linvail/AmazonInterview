// AmazonInterview.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "ArrayAndString.h"
#include "DesignAndOthers.h"
#include "DP.h"
#include "LinkedList.h"
#include "TreeAndGraph.h"
#include "SortingAndSearching.h"
#include "OptimalUtilization.h"
#include "Math.h"

#include <iostream>

/*
Minimum number of swaps required to make a binary string palindrome

Give a binary string consisting of 0¡¦s and 1¡¦s only. E.g., 0100101. We are allowed to pick
any two indexes and swap them. We have to return the minimum number of swaps required to make
it a palindrome or -1 if it cannot. The string 0100101 can be made a palindrome by swapping
(3,4)-> 0101001 and swapping (0,1) -> 1001001 which is a palindrome. In this case, the
correct answer is 2.

Solution:

So we can resolve two differences with one swap. This means that if we find and even number of differences ¡X as we walk along the binary string from the two ends inwards ¡X there is a solution, and it takes half that number of swaps.

If the number of differences is odd, there is still a solution if the input string is odd, because then that middle digit can be swapped with a digit from that last difference, and so get a palindrome.

There is however no solution when the number of differences (in this algorithm) is odd and the number of digits in the input is even.

Now, you don¡¦t actually have to perform the swaps, as the challenge only asks for the number of swaps.

*/

int main()
{
    ArrayAndString::TestArrayAndString();
    LinkedList::TestLinkedList();
    TreeAndGraph::TestTreeAndGraph();
    SortingAndSearching::TestSortingAndSearching();
    DesignAndOthers::TestDesignAndOthers();

    ArrayAndString::TestOptimalUtilization();

    DP::TestDP();

    Math::TestMath();
}
