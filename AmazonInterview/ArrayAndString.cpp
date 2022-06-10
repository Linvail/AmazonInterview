#include "LeetCodeUtil.h"

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

namespace ArrayAndString
{
    // 12. Integer to Roman
    string intToRoman(int num)
    {
        string result;
        if (num >= 1000)
        {
            int symbolCount = num / 1000;
            for (; symbolCount > 0; symbolCount--)
            {
                result.push_back('M');
            }
            num %= 1000;
        }
        // C can be placed before D(500) and M(1000) to make 400 and 900.
        if (num >= 900)
        {
            result += "CM";
            num -= 900;
        }

        if (num >= 500)
        {
            result.push_back('D');
            num -= 500;
        }
        if (num >= 400)
        {
            result += "CD";
            num -= 400;
        }

        if (num >= 100)
        {
            int symbolCount = num / 100;
            for (; symbolCount > 0; symbolCount--)
            {
                result.push_back('C');
            }
            num %= 100;
        }
        // X can be placed before L(50) and C(100) to make 40 and 90.
        if (num >= 90)
        {
            result += "XC";
            num -= 90;
        }

        if (num >= 50)
        {
            result.push_back('L');
            num %= 50;
        }
        if (num >= 40)
        {
            result += "XL";
            num -= 40;
        }

        if (num >= 10)
        {
            int symbolCount = num / 10;
            for (; symbolCount > 0; symbolCount--)
            {
                result.push_back('X');
            }
            num %= 10;
        }
        // I can be placed before V (5) and X (10) to make 4 and 9.
        if (num == 9)
        {
            result += "IX";
            num -= 9;
        }

        if (num >= 5)
        {
            result.push_back('V');
            num -= 5;
        }
        if (num == 4)
        {
            result += "IV";
            num -= 4;
        }

        for (; num > 0; num--)
        {
            result.push_back('I');
        }

        return result;
    }

    void TestArrayAndString()
    {
        // 12. Integer to Roman
        // Input: num = 58
        // Output: "LVIII"
        // Input: num = 1994
        // Output: "MCMXCIV"
        cout << "\n12. Integer to Roman " << intToRoman(1994) << endl;

    }
}