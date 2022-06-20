// AmazonInterview.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "ArrayAndString.h"
#include "DesignAndOthers.h"
#include "DP.h"
#include "LinkedList.h"
#include "TreeAndGraph.h"
#include "SortingAndSearching.h"
#include "OptimalUtilization.h"

#include <iostream>

int main()
{
    ArrayAndString::TestArrayAndString();
    LinkedList::TestLinkedList();
    TreeAndGraph::TestTreeAndGraph();
    SortingAndSearching::TestSortingAndSearching();
    DesignAndOthers::TestDesignAndOthers();

    ArrayAndString::TestOptimalUtilization();

    DP::TestDP();
}
