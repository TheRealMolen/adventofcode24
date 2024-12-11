#include "pch.h"
#include "harness.h"
#include "chargrid.h"
#include "pt2.h"


void collect_paths_from(int x, int y, const CharGrid& g, unordered_set<Pt2u8>& found)
{
    char c = g(x, y);
    if (c == '9')
    {
        found.emplace(u8(x), u8(y));
        return;
    }

    char nextC = c + 1;

    auto check_path = [&g, &found, nextC](int x, int y) -> void
        {
            if (g(x, y) == nextC)
                collect_paths_from(x, y, g, found);
        };

    if (x > 0)
        check_path(x - 1, y);

    if (y > 0)
        check_path(x, y - 1);

    if (y + 1 < g.w())
        check_path(x, y + 1);
     
    if (x + 1 < g.w())
        check_path(x + 1, y);
}

int count_paths_from(int x, int y, const CharGrid& g)
{
    char c = g(x, y);
    if (c == '9')
        return 1;

    char nextC = c + 1;
    int paths = 0;

    auto check_path = [&g, &paths, nextC](int x, int y) -> void
        {
            if (g(x, y) == nextC)
                paths += count_paths_from(x, y, g);
        };

    if (x > 0)
        check_path(x - 1, y);

    if (y > 0)
        check_path(x, y - 1);

    if (y + 1 < g.w())
        check_path(x, y + 1);

    if (x + 1 < g.w())
        check_path(x + 1, y);

    return paths;
}


int day10(const stringlist& input)
{
    const CharGrid g(input);

    int paths = 0;

    auto itRow = begin(g);
    for (int y = 0; y < g.h(); ++y, ++itRow)
    {
        auto itC = begin(*itRow);
        for (int x = 0; x < g.w(); ++x, ++itC)
        {
            if (*itC == '0')
            {
                unordered_set<Pt2u8> found;
                collect_paths_from(x, y, g, found);
                paths += int(found.size());
            }
        }
    }

    return paths;
}

int day10_2(const stringlist& input)
{
    const CharGrid g(input);

    int paths = 0;

    auto itRow = begin(g);
    for (int y = 0; y < g.h(); ++y, ++itRow)
    {
        auto itC = begin(*itRow);
        for (int x = 0; x < g.w(); ++x, ++itC)
        {
            if (*itC == '0')
            {
                int rating = count_paths_from(x, y, g);
                paths += rating;
            }
        }
    }

    return paths;
}


void run_day10()
{
    string sample =
R"(89010123
78121874
87430965
96549874
45678903
32019012
01329801
10456732)";

    test(36, day10(READ(sample)));
    gogogo(day10(LOAD(10)));

    test(81, day10_2(READ(sample)));
    gogogo(day10_2(LOAD(10)));
}