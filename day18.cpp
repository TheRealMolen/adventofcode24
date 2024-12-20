#include "pch.h"
#include "harness.h"

#include "pt2.h"


static void dumpGrid(const vector<char>& grid, int w, int h)
{
    auto itCell = begin(grid);
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x, ++itCell)
        {
            cout << *itCell;
        }

        cout << '\n';
    }

    cout << endl;
}

static void dumpCosts(auto& grid, int w, int h)
{
    using el_type = remove_reference<decltype(grid)>::type::value_type;
    constexpr el_type unvisited = numeric_limits<el_type>::max();

    auto itCell = begin(grid);
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x, ++itCell)
        {
            if (*itCell != unvisited)
                print(cout, "{0:02d} ", *itCell);
            else
                print(cout, "xx ");
        }

        cout << '\n';
    }

    cout << endl;
}


int day18(const stringlist& input, int gridMax = 70, int drops = 1024)
{
    const int w = gridMax + 1;
    const int h = w;

    const Pt2i8 startPos{ 0, 0 };
    const Pt2i8 endPos{ i8(w - 1), i8(h - 1) };

    vector<char> grid(w * h, '.');
    for (auto& line : input | views::take(drops))
    {
        istringstream is(line);
        int x, y;
        is >> x >> "," >> y;

        grid[x + (y * w)] = '#';
    }

    vector<Pt2i8> open;
    vector<i16> bestCost(w * h, numeric_limits<i16>::max());
    bestCost[0] = 0;

    auto addTry = [w, &grid, &open, &bestCost](Pt2i8 newPos, int newCost)
        {
            auto itCell = begin(grid) + (newPos.x + w * newPos.y);
            if (*itCell == '#')
                return;

            auto itBestCost = begin(bestCost) + (newPos.x + w * newPos.y);
            if (*itBestCost > newCost)
            {
                *itBestCost = i16(newCost);

                auto itExisting = ranges::find(open, newPos);
                if (itExisting == end(open))
                    open.push_back(newPos);
            }
        };
    auto addTriesFromCell = [w, h, &addTry](Pt2i8 currPos, int currCost)
        {
            if (currPos.x != 0)
            {
                Pt2i8 newPos = currPos;
                --newPos.x;
                addTry(newPos, currCost + 1);
            }
            if (currPos.y != 0)
            {
                Pt2i8 newPos = currPos;
                --newPos.y;
                addTry(newPos, currCost + 1);
            }
            if (currPos.x + 1 < w)
            {
                Pt2i8 newPos = currPos;
                ++newPos.x;
                addTry(newPos, currCost + 1);
            }
            if (currPos.y + 1 < h)
            {
                Pt2i8 newPos = currPos;
                ++newPos.y;
                addTry(newPos, currCost + 1);
            }
        };

    // add the initial tries
    addTriesFromCell({ 0,0 }, 0);

    while (!open.empty())
    {
        auto cellCost = [w, &bestCost, endPos](const auto& p) -> i16
            {
                i16 currCost = *(bestCost.data() + (p.x + p.y * w));
                i16 h = (endPos - p).manhattan();

                return currCost + h;
            };
        auto itCheapest = ranges::min_element(open, {}, cellCost);

        Pt2i8 pos = *itCheapest;
        erase_unsorted(open, itCheapest);

        if (pos == endPos)
            break;

        addTriesFromCell(pos, *(bestCost.data() + (pos.x + pos.y * w)));
    }

    return bestCost[endPos.x + w * endPos.y];
}


bool is_completable_after(const auto& drops, int gridMax = 70)
{
    const int w = gridMax + 1;
    const int h = w;

    vector<char> grid(w * h, '.');
    for (auto& pos : drops)
        grid[pos.x + (pos.y * w)] = '#';
  //  dumpGrid(grid, w, h);

    const Pt2i8 startPos{ 0, 0 };
    const Pt2i8 endPos{ i8(w - 1), i8(h - 1) };


    vector<Pt2i8> open;
    vector<i16> bestCost(w * h, numeric_limits<i16>::max());
    bestCost[0] = 0;

    auto addTry = [w, &grid, &open, &bestCost](Pt2i8 newPos, int newCost)
        {
            auto itCell = begin(grid) + (newPos.x + w * newPos.y);
            if (*itCell == '#')
                return;

            auto itBestCost = begin(bestCost) + (newPos.x + w * newPos.y);
            if (*itBestCost > newCost)
            {
                *itBestCost = i16(newCost);

                auto itExisting = ranges::find(open, newPos);
                if (itExisting == end(open))
                    open.push_back(newPos);
            }
        };
    auto addTriesFromCell = [w, h, &addTry](Pt2i8 currPos, int currCost)
        {
            if (currPos.x != 0)
            {
                Pt2i8 newPos = currPos;
                --newPos.x;
                addTry(newPos, currCost + 1);
            }
            if (currPos.y != 0)
            {
                Pt2i8 newPos = currPos;
                --newPos.y;
                addTry(newPos, currCost + 1);
            }
            if (currPos.x + 1 < w)
            {
                Pt2i8 newPos = currPos;
                ++newPos.x;
                addTry(newPos, currCost + 1);
            }
            if (currPos.y + 1 < h)
            {
                Pt2i8 newPos = currPos;
                ++newPos.y;
                addTry(newPos, currCost + 1);
            }
        };

    // add the initial tries
    addTriesFromCell({ 0,0 }, 0);

    while (!open.empty())
    {
        auto cellCost = [w, &bestCost](const auto& p) -> i16 { return *(bestCost.data() + (p.x + p.y * w)); };
        auto itCheapest = ranges::min_element(open, {}, cellCost);

        Pt2i8 pos = *itCheapest;
        erase_unsorted(open, itCheapest);

        if (pos == endPos)
        {
           // dumpCosts(bestCost, w, h);
            return true;
        }

        addTriesFromCell(pos, cellCost(pos));
    }

   // dumpCosts(bestCost, w, h);
    return false;
}


string day18_2(const stringlist& input, int gridMax = 70)
{
    const int w = gridMax + 1;
    const int h = w;

    const Pt2i8 startPos{ 0, 0 };
    const Pt2i8 endPos{ i8(w - 1), i8(h - 1) };

    vector<Pt2i8> drops;
    drops.reserve(size(input));
    for (auto& line : input)
    {
        istringstream is(line);
        int x, y;
        is >> x >> "," >> y;
        drops.emplace_back(i8(x), i8(y));
    }

    size_t loIx = 0;
    size_t hiIx = size(drops) - 1;
    while (loIx <= hiIx)
    {
        size_t midIx = (loIx + hiIx) / 2;

        if (is_completable_after(drops | views::take(midIx), gridMax))
            loIx = midIx + 1;
        else
            hiIx = midIx - 1;
    }
    if (loIx > hiIx)
        loIx = hiIx;

    assert(is_completable_after(drops | views::take(loIx), gridMax));
    assert(!is_completable_after(drops | views::take(loIx+1), gridMax));

    Pt2i8 breaking = drops[loIx];
    return format("{0},{1}", breaking.x, breaking.y);
}


void run_day18()
{
    string sample =
R"(5,4
4,2
4,5
3,0
2,1
6,3
2,4
1,5
0,6
3,3
2,6
5,1
1,2
5,5
2,5
6,5
1,4
0,4
6,4
1,1
6,1
1,0
0,5
1,6
2,0)";

    test(22, day18(READ(sample), 6, 12));
    gogogo(day18(LOAD(18)));

    test("6,1", day18_2(READ(sample), 6));
    gogogo(day18_2(LOAD(18)));
}