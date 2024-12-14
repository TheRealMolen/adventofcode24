#include "pch.h"
#include "harness.h"
#include "pt2.h"


using PlotId = u16;
constexpr PlotId NO_PLOT = 0xffffu;

enum EFence
{
    EFence_Left = 1 << 0,
    EFence_North = 1 << 1,
    EFence_Right = 1 << 2,
    EFence_South = 1 << 3,
};


struct Plant
{
    char Type = 0;
    u8 Fences = 0;  // EFence
    PlotId Plot = NO_PLOT;
};

struct Plot
{
    u16 Area = 0;
    Pt2u8 Pos{ 0xffu, 0xffu };
    u16 NumFences = 0;
    char Type = 0;
};
struct Region
{
    int W = 0;
    int H = 0;
    vector<Plant> Cells;
    vector<Plot> Plots;

    Region(const stringlist& input)
    {
        W = int(input.front().size());
        H = int(input.size());
        Cells.reserve(W * H);
        for (auto& line : input)
        {
            for (char c : line)
                Cells.emplace_back(c);
        }
    }
};

void flood_fill(Region& region, int startX, int startY)
{
    auto itStart = begin(region.Cells) + ((startY * region.W) + startX);
    PlotId plotId = PlotId(region.Plots.size());
    region.Plots.push_back({ .Pos{u8(startX), u8(startY)}, .Type{itStart->Type} });
    Plot& plot = region.Plots.back();

    unordered_map<Pt2u8, decltype(itStart)> open;
    open.try_emplace(Pt2u8{ u8(startX), u8(startY) }, itStart);
    while (!open.empty())
    {
        auto curr = open.extract(begin(open));
        Pt2u8 pos = curr.key();
        auto itCell = curr.mapped();

        itCell->Plot = plotId;
        ++plot.Area;

        // left
        if (pos.x > 0)
        {
            auto itNeighbour = itCell - 1;
            if (itNeighbour->Type != (itCell->Type))
            {
                ++plot.NumFences;
                itCell->Fences |= EFence_Left;
            }
            else if (itNeighbour->Plot != plotId)
                open.try_emplace(Pt2u8{ u8(pos.x - 1), pos.y }, itNeighbour);
        }
        else
        {
            ++plot.NumFences;
            itCell->Fences |= EFence_Left;
        }

        // right
        if (pos.x + 1 < region.W)
        {
            auto itNeighbour = itCell + 1;
            if (itNeighbour->Type != (itCell->Type))
            {
                ++plot.NumFences;
                itCell->Fences |= EFence_Right;
            }
            else if (itNeighbour->Plot != plotId)
                open.try_emplace(Pt2u8{ u8(pos.x + 1), pos.y }, itNeighbour);
        }
        else
        {
            ++plot.NumFences;
            itCell->Fences |= EFence_Right;
        }

        // north
        if (pos.y > 0)
        {
            auto itNeighbour = itCell - region.W;
            if (itNeighbour->Type != (itCell->Type))
            {
                ++plot.NumFences;
                itCell->Fences |= EFence_North;
            }
            else if (itNeighbour->Plot != plotId)
                open.try_emplace(Pt2u8{ pos.x, u8(pos.y - 1) }, itNeighbour);
        }
        else
        {
            ++plot.NumFences;
            itCell->Fences |= EFence_North;
        }

        // south
        if (pos.y + 1 < region.H)
        {
            auto itNeighbour = itCell + region.W;
            if (itNeighbour->Type != (itCell->Type))
            {
                ++plot.NumFences;
                itCell->Fences |= EFence_South;
            }
            else if (itNeighbour->Plot != plotId)
                open.try_emplace(Pt2u8{ pos.x, u8(pos.y + 1) }, itNeighbour);
        }
        else
        {
            ++plot.NumFences;
            itCell->Fences |= EFence_South;
        }
    }
}


i64 day12(const stringlist& input)
{
    Region region(input);

    auto itCell = begin(region.Cells);
    for (int y = 0; y < region.H; ++y)
    {
        for (int x = 0; x < region.H; ++x, ++itCell)
        {
            if (itCell->Plot == NO_PLOT)
            {
                flood_fill(region, x, y);
            }
        }
    }

    i64 cost = 0;
    for (auto [id, plot] : region.Plots | views::enumerate)
        cost += plot.Area * plot.NumFences;

    return cost;
}

i64 day12_2(const stringlist& input)
{
    Region region(input);

    auto itCell = begin(region.Cells);
    for (int y = 0; y < region.H; ++y)
    {
        for (int x = 0; x < region.H; ++x, ++itCell)
        {
            if (itCell->Plot == NO_PLOT)
            {
                flood_fill(region, x, y);
            }
        }
    }

    // group fences -- vert edges
    auto itCol = begin(region.Cells);
    for (int x = 0; x < region.W; ++x, ++itCol)
    {
        auto itPrev = itCol;
        auto itCurr = itPrev;

        for (int y = 1; y < region.H; ++y, itPrev = itCurr)
        {
            itCurr += region.W;

            if (itPrev->Plot != itCurr->Plot)
                continue;

            // left
            const bool prevLeftEdge = (itPrev->Fences & EFence_Left) != 0;
            const bool currLeftEdge = (itCurr->Fences & EFence_Left) != 0;
            if (prevLeftEdge && currLeftEdge)
                --region.Plots[itCurr->Plot].NumFences;

            // right
            const bool prevRightEdge = (itPrev->Fences & EFence_Right) != 0;
            const bool currRightEdge = (itCurr->Fences & EFence_Right) != 0;
            if (prevRightEdge && currRightEdge)
                --region.Plots[itCurr->Plot].NumFences;
        }
    }

    // group fences -- horiz edges
    auto itRow = data(region.Cells);
    for (int y = 0; y < region.H; ++y, itRow += region.W)
    {
        auto itPrev = itRow;
        auto itCurr = itRow;

        for (int x = 1; x < region.W; ++x, itPrev = itCurr)
        {
            ++itCurr;

            if (itPrev->Plot != itCurr->Plot)
                continue;

            // north
            const bool prevNorthEdge = (itPrev->Fences & EFence_North) != 0;
            const bool currNorthEdge = (itCurr->Fences & EFence_North) != 0;
            if (prevNorthEdge && currNorthEdge)
                --region.Plots[itCurr->Plot].NumFences;

            // south
            const bool prevSouthEdge = (itPrev->Fences & EFence_South) != 0;
            const bool currSouthEdge = (itCurr->Fences & EFence_South) != 0;
            if (prevSouthEdge && currSouthEdge)
                --region.Plots[itCurr->Plot].NumFences;
        }
    }

    i64 cost = 0;
    for (auto [id, plot] : region.Plots | views::enumerate)
        cost += plot.Area * plot.NumFences;

    return cost;
}


void run_day12()
{
    string sample =
R"(AAAA
BBCD
BBCC
EEEC)";
    string sample2 =
R"(RRRRIICCFF
RRRRIICCCF
VVRRRCCFFF
VVRCCCJFFF
VVVVCJJCFE
VVIVCCJJEE
VVIIICJJEE
MIIIIIJJEE
MIIISIJEEE
MMMISSJEEE)";
    string sample3 =
R"(EEEEE
EXXXX
EEEEE
EXXXX
EEEEE)";
    string sample4 =
R"(AAAAAA
AAABBA
AAABBA
ABBAAA
ABBAAA
AAAAAA)";

    test(140, day12(READ(sample)));
    test(1930, day12(READ(sample2)));
    gogogo(day12(LOAD(12)));

    test(80, day12_2(READ(sample)));
    test(1206, day12_2(READ(sample2)));
    test(236, day12_2(READ(sample3)));
    test(368, day12_2(READ(sample4)));
    gogogo(day12_2(LOAD(12)));
}