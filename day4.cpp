#include "pch.h"
#include "harness.h"
#include "chargrid.h"


int day4(const stringlist& input)
{
    CharGrid m(input);
    CharGrid dbg(input);

    const size_t width = m.front().size();
    const size_t height = m.size();
    const size_t maxYForDown = height - 3;

    int xmasses = 0;
    auto foundAt = [&](int x, int y)
        {
            ++xmasses;

            if (dbg[y][x] == 'X' || dbg[y][x] == 'S')
                dbg[y][x] = '0';

            ++dbg[y][x];
        };

    for (int y = 0; y < height; ++y)
    {
        int x = 0;
        for (; x < width - 3; ++x)
        {
            char c = m[y][x];
            if (c == 'X')
            {
                // diag up
                if ((y >= 3) && (m[y-1][x+1] == 'M') && (m[y-2][x+2] == 'A') && (m[y-3][x+3] == 'S'))
                    foundAt(x, y);
                // horiz
                if ((m[y][x+1] == 'M') && (m[y][x+2] == 'A') && (m[y][x+3] == 'S'))
                    foundAt(x, y);
                // diag dn
                if ((y < maxYForDown) && (m[y+1][x+1] == 'M') && (m[y+2][x+2] == 'A') && (m[y+3][x+3] == 'S'))
                    foundAt(x, y);
                // down
                if ((y < maxYForDown) && (m[y+1][x] == 'M') && (m[y+2][x] == 'A') && (m[y+3][x] == 'S'))
                    foundAt(x, y);
            }
            else if (c == 'S')
            {
                // diag up
                if ((y >= 3) && (m[y-1][x+1] == 'A') && (m[y-2][x+2] == 'M') && (m[y-3][x+3] == 'X'))
                    foundAt(x, y);
                // horiz
                if ((m[y][x+1] == 'A') && (m[y][x+2] == 'M') && (m[y][x+3] == 'X'))
                    foundAt(x, y);
                // diag dn
                if ((y < maxYForDown) && (m[y+1][x+1] == 'A') && (m[y+2][x+2] == 'M') && (m[y+3][x+3] == 'X'))
                    foundAt(x, y);
                // down
                if ((y < maxYForDown) && (m[y+1][x] == 'A') && (m[y+2][x] == 'M') && (m[y+3][x] == 'X'))
                    foundAt(x, y);
            }
        }
        for (; x < width; ++x)
        {
            char c = m[y][x];
            if (c == 'X')
            {
                // down
                if ((y < maxYForDown) && (m[y+1][x] == 'M') && (m[y+2][x] == 'A') && (m[y+3][x] == 'S'))
                    foundAt(x, y);
            }
            else if (c == 'S')
            {
                // down
                if ((y < maxYForDown) && (m[y+1][x] == 'A') && (m[y+2][x] == 'M') && (m[y+3][x] == 'X'))
                    foundAt(x, y);
            }
        }
    }

    return xmasses;
}

int day4_2(const stringlist& input)
{
    CharGrid m(input);
    CharGrid dbg(input);

    const size_t width = m.front().size();
    const size_t height = m.size();

    int xmasses = 0;
    auto foundAt = [&](int x, int y)
        {
            ++xmasses;

            if (dbg[y][x] == 'X' || dbg[y][x] == 'S')
                dbg[y][x] = '0';

            ++dbg[y][x];
        };

    for (int y = 1; y < height-1; ++y)
    {
        for (int x=1; x < width - 1; ++x)
        {
            if (m[y][x] == 'A')
            {
                const bool posOk = ((m[y-1][x-1] == 'M') && (m[y+1][x+1] == 'S')) || ((m[y-1][x-1] == 'S') && (m[y+1][x+1] == 'M'));
                const bool negOk = ((m[y-1][x+1] == 'M') && (m[y+1][x-1] == 'S')) || ((m[y-1][x+1] == 'S') && (m[y+1][x-1] == 'M'));

                if (posOk && negOk)
                    foundAt(x, y);
            }
        }
    }

    return xmasses;
}


void run_day4()
{
    string sample =
R"(MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX)";

    test(18, day4(READ(sample)));
    gogogo(day4(LOAD(4)));

    test(9, day4_2(READ(sample)));
    gogogo(day4_2(LOAD(4)));
}