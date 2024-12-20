#include "pch.h"
#include "harness.h"
#include "pt2.h"


enum ERdir : u8 { East, South, West, North };
constexpr Pt2i kRdirVec[] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

struct ReindeerCell
{
    bool isWall = false;
    u8 bestEntryDir = East;
    i32 bestScore = numeric_limits<i32>::max();

    bool visited() const { return bestScore != numeric_limits<i32>::max(); }
};

struct ReindeerMaze
{
    //using ItCell = vector<ReindeerCell>::iterator;
    using ItCell = ReindeerCell*;

    vector<ReindeerCell> cells;
    int w, h;

    Pt2i startPos, endPos;

    ItCell operator[](const Pt2i& p)
    {
        return data(cells) + (p.x + w*p.y);
    }
};

struct RdToTry
{
    Pt2i cellPos;
    ERdir facingDir;
    int cost;
};

ReindeerMaze load_maze(const stringlist& input)
{
    ReindeerMaze maze;
    maze.w = int(input.front().size());
    maze.h = int(input.size());
    maze.cells.reserve(maze.w * maze.h);

    for (int y = 0; y < maze.h; ++y)
    {
        auto itC = begin(input[y]);
        for (int x = 0; x < maze.w; ++x, ++itC)
        {
            char c = *itC;
            if (c == '#')
            {
                maze.cells.emplace_back(true);
            }
            else
            {
                if (c == 'S')
                    maze.startPos = { x, y };
                else if (c == 'E')
                    maze.endPos = { x, y };

                maze.cells.emplace_back(false);
            }
        }
    }

    return maze;
}



int day16(const stringlist& input)
{
    ReindeerMaze maze = load_maze(input);

    vector<RdToTry> open;
    auto addTry = [&open, &maze](Pt2i newPos, ERdir newDir, int newCost)
        {
            auto itNew = maze[newPos];
            if (!itNew->isWall)
            {
                auto itExisting = ranges::find_if(open, [&](const RdToTry& t) { return t.cellPos == newPos && t.facingDir == newDir; });
                if (itExisting == end(open))
                    open.emplace_back(newPos, newDir, newCost);
                else if (itExisting->cost > newCost)
                    itExisting->cost = newCost;
            }
        };
    auto addTriesFromCell = [&maze, &addTry](Pt2i currPos, ERdir currDir, int currCost)
        {
            addTry(currPos + kRdirVec[currDir], currDir, currCost + 1);

            ERdir leftDir = ERdir((int(currDir) + 3) % 4);
            if (!maze[currPos + kRdirVec[leftDir]]->isWall)
                addTry(currPos, leftDir, currCost + 1000);

            ERdir rightDir = ERdir((int(currDir) + 1) % 4);
            if (!maze[currPos + kRdirVec[rightDir]]->isWall)
                addTry(currPos, rightDir, currCost + 1000);
        };

    // add the initial tries
    addTriesFromCell(maze.startPos, East, 0);
    addTry(maze.startPos, West, 2000);

    auto itEndCell = maze[maze.endPos];

    size_t max_open = 0;
    while (!open.empty())
    {
        max_open = max(max_open, size(open));

        auto itCheapest = ranges::min_element(open, {}, &RdToTry::cost);
        RdToTry currTry = *itCheapest;
        erase_unsorted(open, itCheapest);

        if (itEndCell->visited() && currTry.cost > itEndCell->bestScore)
            continue;

        auto itCell = maze[currTry.cellPos];
        if (itCell->bestScore > currTry.cost)
        {
            itCell->bestScore = currTry.cost;
            itCell->bestEntryDir = currTry.facingDir;
        }

        if (itCell == itEndCell)
            break;

        addTriesFromCell(currTry.cellPos, currTry.facingDir, currTry.cost);
    }

    return maze[maze.endPos]->bestScore;
}

int day16_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day16()
{
    string sample =
R"(###############
#.......#....E#
#.#.###.#.###.#
#.....#.#...#.#
#.###.#####.#.#
#.#.#.......#.#
#.#.#####.###.#
#...........#.#
###.#.#####.#.#
#...#.....#.#.#
#.#.#.###.#.#.#
#.....#...#.#.#
#.###.#.#.#.#.#
#S..#.....#...#
###############)";
    string sample2 =
R"(#################
#...#...#...#..E#
#.#.#.#.#.#.#.#.#
#.#.#.#...#...#.#
#.#.#.#.###.#.#.#
#...#.#.#.....#.#
#.#.#.#.#.#####.#
#.#...#.#.#.....#
#.#.#####.#.###.#
#.#.#.......#...#
#.#.###.#####.###
#.#.#...#.....#.#
#.#.#.#####.###.#
#.#.#.........#.#
#.#.#.#########.#
#S#.............#
#################)";

    test(7036, day16(READ(sample)));
    test(11048, day16(READ(sample2)));
    nonono(day16(LOAD(16)), 85420);

    skip("later");
    //test(-100, day16_2(READ(sample)));
    //gogogo(day16_2(LOAD(16)));
}