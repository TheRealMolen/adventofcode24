#include "pch.h"
#include "harness.h"
#include "pt2.h"


i64 day14(const stringlist& input, int w=101, int h=103, int ticks=100)
{
    const int midW = w / 2;
    const int midH = h / 2;

    vector<int> robotsPerCell;
    robotsPerCell.resize(w * h, 0);

    int robotsPerQuadrant[4] = { 0,0,0,0 };
    for (auto& line : input)
    {
        Pt2i pos, vel;
        istringstream is(line);
        is >> "p=" >> pos.x >> "," >> pos.y >> " v=" >> vel.x >> "," >> vel.y;

        int endX = (pos.x + (ticks * vel.x)) % w;
        int endY = (pos.y + (ticks * vel.y)) % h;
        if (endX < 0)
            endX += w;
        if (endY < 0)
            endY += h;

        ++robotsPerCell[endX + (endY * w)];

        if (endX < midW)
        {
            if (endY < midH)
                ++robotsPerQuadrant[0];
            else if (endY > midH)
                ++robotsPerQuadrant[1];
        }
        else if (endX > midW)
        {
            if (endY < midH)
                ++robotsPerQuadrant[2];
            else if (endY > midH)
                ++robotsPerQuadrant[3];
        }
    }

    //for (int y = 0; y < h; ++y)
    //{
    //    if (y != midH)
    //    {
    //        for (int x = 0; x < w; ++x)
    //        {
    //            if (x == midW)
    //            {
    //                cout << " ";
    //                continue;
    //            }

    //            int robots = robotsPerCell[x + (y * w)];
    //            if (robots == 0)
    //                cout << ".";
    //            else
    //                cout << char('0' + robots);
    //        }
    //    }

    //    cout << endl;
    //}

    i64 safety = 1;
    for (int numRobots : robotsPerQuadrant)
        safety *= numRobots;

    return safety;
}

i64 day14_2(const stringlist& input, int startTicks=0, int endTicks=1250000)
{
    constexpr int w = 101;
    constexpr int h = 103;

    vector<pair<Pt2i, Pt2i>> robots;
    for (auto& line : input)
    {
        Pt2i pos, vel;
        istringstream is(line);
        is >> "p=" >> pos.x >> "," >> pos.y >> " v=" >> vel.x >> "," >> vel.y;
        robots.emplace_back(pos, vel);
    }

    for (int ticks = startTicks; ticks <= endTicks; ++ticks)
    {
        vector<int> robotsPerCell;
        robotsPerCell.resize(w * h, 0);

        int topLeftCount = 0;
        for (auto& [pos, vel] : robots)
        {
            int endX = (pos.x + (ticks * vel.x)) % w;
            int endY = (pos.y + (ticks * vel.y)) % h;
            if (endX < 0)
                endX += w;
            if (endY < 0)
                endY += h;

            if ((endX + endY) < 45)
                ++topLeftCount;

            ++(robotsPerCell.data()[endX + (endY * w)]);
        }

        if (topLeftCount < 20)
        {
            cout << "\n\nAfter " << ticks << " ticks...\n";
            auto itCell = data(robotsPerCell);
            for (int y = 0; y < h; ++y)
            {
                for (int x = 0; x < w; ++x, ++itCell)
                {
                    int numRobots = *itCell;
                    if (numRobots == 0)
                        cout << ' ';
                    else
                        cout << char('0' + numRobots);
                }

                cout << '\n';
            }
        }
    }

    return 0;
}


void run_day14()
{
    string sample =
R"(p=0,4 v=3,-3
p=6,3 v=-1,-3
p=10,3 v=-1,2
p=2,0 v=2,-1
p=0,0 v=1,3
p=3,0 v=-2,-2
p=7,6 v=-1,-3
p=3,0 v=-1,-2
p=9,3 v=2,3
p=7,3 v=-1,2
p=2,4 v=2,-3
p=9,5 v=-3,-3)";

    test(12, day14(READ(sample), 11, 7));
    gogogo(day14(LOAD(14)));

    //gogogo(day14_2(LOAD(14)));
    skip("xmas tree");
}