#include "pch.h"
#include "harness.h"


bool isSafe(const auto& report)
{
    optional<bool> reportAsc;

    for (auto ab : views::pairwise(report))
    {
        int prev = get<0>(ab);
        int curr = get<1>(ab);

        int delta = curr - prev;
        bool asc = delta > 0;

        if (reportAsc.has_value() && asc != reportAsc.value())
            return false;
        reportAsc = asc;

        if (delta == 0)
            return false;
        if (abs(delta) > 3)
            return false;
    }

    return true;
}


int day2(const stringlist& input)
{
    int numSafe = 0;

    vector<int> report;
    for (auto& line : input)
    {
        report = string_to_intlist(line);
        if (isSafe(report))
            ++numSafe;
    }

    return numSafe;
}

int day2_2(const stringlist& input)
{
    int numSafe = 0;
    vector<int> report, healed;
    for (auto& line : input)
    {
        report = string_to_intlist(line);
        if (isSafe(report))
        {
            ++numSafe;
            continue;
        }

        // check for single error
        for (auto errorPos = 0; errorPos < size(report); ++errorPos)
        {
            healed.clear();
            for (size_t i=0; i<size(report); ++i)
            {
                if (i != errorPos)
                    healed.push_back(report[i]);
            }

            if (isSafe(healed))
            {
                ++numSafe;
                break;
            }
        }
    }

    return numSafe;
}


void run_day2()
{
    string sample =
R"(1 2 7 8 9
1 3 2 4 5
9 7 6 2 1
7 6 4 2 1
8 6 4 4 1
1 3 6 7 9)";

    test(2, day2(READ(sample)));
    gogogo(day2(LOAD(2)), 510);

    test(4, day2_2(READ(sample)));
    gogogo(day2_2(LOAD(2)));
}