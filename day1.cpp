#include "pch.h"
#include "harness.h"


int day1(const stringlist& input)
{
    vector<int> team1, team2;
    team1.reserve(input.size());
    team2.reserve(input.size());

    for (auto& line : input)
    {
        istringstream is(line);
        int a, b;
        is >> a >> skipws >> b;

        team1.push_back(a);
        team2.push_back(b);
    }

    ranges::sort(team1);
    ranges::sort(team2);

    int totalDist = 0;
    for (const auto& ab : views::zip(team1, team2))
    {
        int dist = abs(get<0>(ab) - get<1>(ab));
        totalDist += dist;
    }

    return totalDist;
}

i64 day1_2(const stringlist& input)
{
    vector<int> team1, team2;
    team1.reserve(input.size());
    team2.reserve(input.size());

    for (auto& line : input)
    {
        istringstream is(line);
        int a, b;
        is >> a >> skipws >> b;

        team1.push_back(a);
        team2.push_back(b);
    }

    ranges::sort(team2);

    i64 similarity = 0;
    for (int a : team1)
    {
        auto itLower = ranges::lower_bound(team2, a);
        auto itUpper = ranges::upper_bound(team2, a);
        size_t num = distance(itLower, itUpper);
        similarity += a * num;
    }

    return similarity;
}


void run_day1()
{
    string sample =
R"(3   4
4   3
2   5
1   3
3   9
3   3)";

    test(11, day1(READ(sample)));
    gogogo(day1(LOAD(1)));

    test(31, day1_2(READ(sample)));
    gogogo(day1_2(LOAD(1)));
}