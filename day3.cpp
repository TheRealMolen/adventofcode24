#include "pch.h"
#include "harness.h"

#include <regex>


i64 day3(const string& input)
{
    i64 total = 0;

    regex reMul(R"(mul\(([0-9]+),([0-9]+)\))");
    auto itBegin = sregex_iterator(begin(input), end(input), reMul);
    auto itEnd = sregex_iterator();
    for (auto it = itBegin; it != itEnd; ++it)
    {
        smatch match = *it;
        total += stoi(match[1]) * stoi(match[2]);
    }

    return total;
}

i64 day3_2(const string& input)
{
    i64 total = 0;

    regex reMul(R"(mul\(([0-9]+),([0-9]+)\)|do\(\)|don't\(\))");
    auto itBegin = sregex_iterator(begin(input), end(input), reMul);
    auto itEnd = sregex_iterator();
    bool enabled = true;
    for (auto it = itBegin; it != itEnd; ++it)
    {
        smatch match = *it;
        if (match[0] == "do()")
        {
            enabled = true;
            continue;
        }
        if (match[0] == "don't()")
        {
            enabled = false;
            continue;
        }

        if (enabled)
            total += stoi(match[1]) * stoi(match[2]);
    }

    return total;
}


void run_day3()
{
    string sample1 = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
    string sample2 = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";

    test(161, day3(sample1));
    gogogo(day3(LOADSTR(3)));

    test(48, day3_2(sample2));
    gogogo(day3_2(LOADSTR(3)));
}