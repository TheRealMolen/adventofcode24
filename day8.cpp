#include "pch.h"
#include "harness.h"

#include "pt2.h"


int day8(const stringlist& input)
{
    const size_t w = input.front().size();
    const size_t h = input.size();

    map<char, vector<Pt2i>> antennasByFreq;
    for (int y = 0; y < h; ++y)
    {
        auto itCurr = begin(input[y]);
        for (int x = 0; x < w; ++x, ++itCurr)
        {
            char c = *itCurr;
            if (c == '.')
                continue;

            auto itFreq = antennasByFreq.try_emplace(c).first;
            itFreq->second.emplace_back(x, y);
        }
    }

    set<Pt2i> antinodes;
    auto add_antinode = [=, &antinodes](const Pt2i& p)
        {
            if (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h)
            {
                antinodes.insert(p);
            }
        };

    for (const auto& freq_antennas : antennasByFreq)
    {
        const vector<Pt2i>& antennas = freq_antennas.second;
        for (auto itA = begin(antennas); itA != end(antennas); ++itA)
        {
            auto itB = itA;
            for (++itB; itB != end(antennas); ++itB)
            {
                Pt2i d = *itB - *itA;
                add_antinode(*itA - d);
                add_antinode(*itB + d);
            }
        }
    }

    return int(antinodes.size());
}

int day8_2(const stringlist& input)
{
    const size_t w = input.front().size();
    const size_t h = input.size();

    map<char, vector<Pt2i>> antennasByFreq;
    for (int y = 0; y < h; ++y)
    {
        auto itCurr = begin(input[y]);
        for (int x = 0; x < w; ++x, ++itCurr)
        {
            char c = *itCurr;
            if (c == '.')
                continue;

            auto itFreq = antennasByFreq.try_emplace(c).first;
            itFreq->second.emplace_back(x, y);
        }
    }

    set<Pt2i> antinodes;
    auto add_antinode = [=, &antinodes](const Pt2i& p) -> bool
        {
            if (p.x < 0 || p.x >= w || p.y < 0 || p.y >= h)
                return false;
            
            antinodes.insert(p);
            return true;
        };

    for (const auto& freq_antennas : antennasByFreq)
    {
        const vector<Pt2i>& antennas = freq_antennas.second;
        for (auto itA = begin(antennas); itA != end(antennas); ++itA)
        {
            auto itB = itA;
            for (++itB; itB != end(antennas); ++itB)
            {
                Pt2i d = *itB - *itA;

                // lo
                for (Pt2i p = *itA; ; p -= d)
                {
                    if (!add_antinode(p))
                        break;
                }

                // hi
                for (Pt2i p = *itB; ; p += d)
                {
                    if (!add_antinode(p))
                        break;
                }
            }
        }
    }

    return int(antinodes.size());
}


void run_day8()
{
    string sample =
R"(............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............)";

    test(14, day8(READ(sample)));
    gogogo(day8(LOAD(8)));

    test(34, day8_2(READ(sample)));
    gogogo(day8_2(LOAD(8)));
}