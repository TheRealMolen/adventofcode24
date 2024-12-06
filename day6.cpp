#include "pch.h"
#include "harness.h"
#include "pt2.h"

#include <execution>


enum class Dir { Up, Right, Down, Left };

inline u8 dirBit(Dir d)
{
    return 1 << int(d);
}

static const Pt2i kDirVecs[] =
{
    { 0, -1 },
    { 1, 0 },
    { 0, 1 },
    { -1, 0 },
};


struct PatrolMap
{
    int w, h;
    vector<u8> grid;
    Pt2i start{ -1, -1 };

    static constexpr u8 Obstacle = u8(-1);


    PatrolMap(const stringlist& input)
        : w(int(input.front().size()))
        , h(int(input.size()))
    {
        grid.reserve(w * h);
        for (int y = 0; y < h; ++y)
        {
            auto itC = begin(input[y]);
            for (int x = 0; x < w; ++x, ++itC)
            {
                if (*itC == '.')
                    grid.push_back(0);
                else if (*itC == '#')
                    grid.push_back(Obstacle);
                else if (*itC == '^')
                {
                    grid.push_back(0);  // note: we mark this at the start of the first step
                    start = { x, y };
                }
            }
        }
    }

    u8& operator()(const Pt2i& p)
    {
        return *(grid.data() + p.x + p.y*w);
    }
    const u8& operator()(const Pt2i& p) const
    {
        return *(grid.data() + p.x + p.y*w);
    }

    bool isInside(const Pt2i& p) const
    {
        return (p.x >= 0) && (p.x < w) && (p.y >= 0) && (p.y < w);
    }
};

static const char kDirBitCharLookup[] =
{           // ldru
    '.',    // 0000
    '^',    // 0001
    '>',    // 0010
    'r',    // 0011
    'v',    // 0100
    '|',    // 0101
    '7',    // 0110
    'k',    // 0111
    '<',    // 1000
    'L',    // 1001
    '-',    // 1010
    '%',    // 1011
    'J',    // 1100
    'd',    // 1101
    'T',    // 1110
    '+',    // 1111
};
static_assert(sizeof(kDirBitCharLookup) == 16);

ostream& operator<<(ostream& os, const PatrolMap& m)
{
    for (int y = 0; y < m.h; ++y)
    {
        for (int x = 0; x < m.h; ++x)
        {
            u8 cell = m({ x, y });
            if (cell == PatrolMap::Obstacle)
                os << '#';
            else
                os << kDirBitCharLookup[cell & 0xf];
        }
        os << endl;
    }
    return os;
}


int day6(const stringlist& input)
{
    PatrolMap m(input);
    Pt2i p = m.start;
    Dir d = Dir::Up;

    for (;;)
    {
        m(p) |= dirBit(d);

        Pt2i np = p + kDirVecs[int(d)];
        if (!m.isInside(np))
            break;

        if (m(np) == PatrolMap::Obstacle)
        {
            d = Dir((int(d) + 1) & 3);
        }
        else
        {
            p = np;
        }
    }

    return int(ranges::count_if(m.grid, [](u8 cell) { return cell != PatrolMap::Obstacle && cell > 0; }));
}


set<Pt2i> gatherPotentialObstacles(const stringlist& input)
{
    set<Pt2i> potentialObstacles;

    PatrolMap m(input);
    Pt2i p = m.start;
    Dir d = Dir::Up;
    for (;;)
    {
        m(p) |= dirBit(d);

        if (p != m.start)
            potentialObstacles.insert(p);

        Pt2i np = p + kDirVecs[int(d)];
        if (!m.isInside(np))
            break;

        if (m(np) == PatrolMap::Obstacle)
        {
            d = Dir((int(d) + 1) & 3);
        }
        else
        {
            p = np;
        }
    }

    return potentialObstacles;
}

int day6_2(const stringlist& input)
{
    set<Pt2i> potentialObstacles = gatherPotentialObstacles(input);

    // test all obstacles to see whether they result in a loop or not
    const PatrolMap startMap(input);

    int numLoops = transform_reduce(execution::par_unseq,
        begin(potentialObstacles), end(potentialObstacles),
        0,
        plus<>{},
        [&startMap](Pt2i obstaclePos) -> int
        {
            PatrolMap m = startMap;
            m(obstaclePos) = PatrolMap::Obstacle;

            Pt2i p = m.start;
            Dir d = Dir::Up;
            for (;;)
            {
                u8* cell = &m(p);
                if (*cell & dirBit(d))
                    return 1;   // found a loop

                *cell |= dirBit(d);

                Pt2i np = p + kDirVecs[int(d)];
                if (!m.isInside(np))
                    break;

                if (m(np) == PatrolMap::Obstacle)
                {
                    d = Dir((int(d) + 1) & 3);
                }
                else
                {
                    p = np;
                }
            }

            return 0;   // no loop here
        });

    return numLoops;
}


void run_day6()
{
    string sample =
        R"(....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#...)";

    test(41, day6(READ(sample)));
    gogogo(day6(LOAD(6)), 4778);

    test(6, day6_2(READ(sample)));
    gogogo(day6_2(LOAD(6)), 1618);
}