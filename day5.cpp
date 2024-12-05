#include "pch.h"
#include "harness.h"


bool isUpdateCorrectlyOrdered(const multimap<int, int>& rules, const vector<int>& pages)
{
    for (auto itPage = begin(pages) + 1; itPage != end(pages); ++itPage)
    {
        auto [itRuleBegin, itRuleEnd] = rules.equal_range(*itPage);
        for (auto itRule = itRuleBegin; itRule != itRuleEnd; ++itRule)
        {
            const int mustNotBeBefore = itRule->second;

            auto itFound = find(begin(pages), itPage, mustNotBeBefore);
            if (itFound != itPage)
                return false;
        }
    }

    return true;
}


int day5(const stringlist& input)
{
    multimap<int, int> rules;

    int checksum = 0;

    auto itLine = begin(input);
    for (; !itLine->empty(); ++itLine)
    {
        int before, after;
        istringstream is(*itLine);
        is >> before >> "|" >> after;

        rules.emplace(before, after);
    }
    for (++itLine; itLine != end(input); ++itLine)
    {
        vector<int> pages = string_to_intlist(*itLine, ',');
        if (isUpdateCorrectlyOrdered(rules, pages))
        {
            checksum += pages[pages.size() / 2];
        }
    }

    return checksum;
}

int day5_2(const stringlist& input)
{
    multimap<int, int> rules;

    int checksum = 0;

    auto itLine = begin(input);
    for (; !itLine->empty(); ++itLine)
    {
        int before, after;
        istringstream is(*itLine);
        is >> before >> "|" >> after;

        rules.emplace(before, after);
    }

    auto byPageOrder = [&rules](int a, int b) -> bool
        {
            auto [itRuleBegin, itRuleEnd] = rules.equal_range(a);
            for (auto itRule = itRuleBegin; itRule != itRuleEnd; ++itRule)
            {
                if (itRule->second == b)
                    return false;
            }

            return a != b;
        };

    for (++itLine; itLine != end(input); ++itLine)
    {
        vector<int> pages = string_to_intlist(*itLine, ',');
        if (isUpdateCorrectlyOrdered(rules, pages))
            continue;

        ranges::sort(pages, byPageOrder);
        checksum += pages[pages.size() / 2];
    }

    return checksum;
}


void run_day5()
{
    string sample =
R"(47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47)";

    test(143, day5(READ(sample)));
    gogogo(day5(LOAD(5)));

    test(123, day5_2(READ(sample)));
    gogogo(day5_2(LOAD(5)));
}