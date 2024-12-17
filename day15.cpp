#include "pch.h"
#include "harness.h"
#include "pt2.h"


void dumpGrid(const vector<char>& grid, int w, int h, Pt2i pos)
{
    auto itCell = begin(grid);
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x, ++itCell)
        {
            if (x == pos.x && y == pos.y)
            {
                assert(*itCell == '.');
                cout << '@';
            }
            else
            {
                cout << *itCell;
            }
        }

        cout << '\n';
    }

    cout << endl;
}

i64 day15(const stringlist& input)
{
    auto itGridStart = begin(input);
    auto itGridEnd = find(begin(input), end(input), ""s);
    const int w = int(itGridStart->size());
    const int h = int(itGridEnd - itGridStart);
    vector<char> cells;
    cells.reserve(w * h);
    auto itLine = itGridStart;
    Pt2i startPos{ -1, -1 };
    for (int y = 0; y < h; ++y, ++itLine)
    {
        auto itCell = begin(*itLine);
        for (int x = 0; x < w; ++x, ++itCell)
        {
            char c = *itCell;
            if (c != '@')
                cells.push_back(c);
            else
            {
                cells.push_back('.');
                startPos = { x, y };
            }
        }
    }

    // process moves
    Pt2i pos = startPos;
    for (++itLine; itLine != end(input); ++itLine)
    {
        for (char move : *itLine)
        {
            auto itCell = begin(cells) + (pos.x + w * pos.y);
            switch (move)
            {
            case '<':
            {
                auto itStartOfRow = itCell - pos.x;
                auto itSpace = itCell - 1;
                for (; itSpace != itStartOfRow; --itSpace)
                    if (*itSpace == '.' || *itSpace == '#')
                        break;

                if (itSpace != itStartOfRow && *itSpace != '#')    // valid move!
                {
                    swap(*(itCell - 1), *itSpace);
                    --pos.x;
                }
                break;
            }

            case '^':
            {
                auto itStartOfCol = itCell - (w * pos.y);
                auto itSpace = itCell - w;
                for (; itSpace != itStartOfCol; itSpace -= w)
                    if (*itSpace == '.' || *itSpace == '#')
                        break;

                if (itSpace != itStartOfCol && *itSpace != '#')    // valid move!
                {
                    swap(*(itCell - w), *itSpace);
                    --pos.y;
                }
                break;
            }

            case '>':
            {
                auto itEndOfRow = itCell + (w - pos.x - 1);
                auto itSpace = itCell + 1;
                for (; itSpace != itEndOfRow; ++itSpace)
                    if (*itSpace == '.' || *itSpace == '#')
                        break;

                if (itSpace != itEndOfRow && *itSpace != '#')    // valid move!
                {
                    swap(*(itCell + 1), *itSpace);
                    ++pos.x;
                }
                break;
            }

            case 'v':
            {
                auto itEndOfCol = itCell + (w * (h - pos.y - 1));
                auto itSpace = itCell + w;
                for (; itSpace != itEndOfCol; itSpace += w)
                    if (*itSpace == '.' || *itSpace == '#')
                        break;

                if (itSpace != itEndOfCol && *itSpace != '#')    // valid move!
                {
                    swap(*(itCell + w), *itSpace);
                    ++pos.y;
                }
                break;
            }

            default:
                assert(!"bad move");
            }
        }
    }

    i64 sumGps = 0;
    auto itCell = begin(cells);
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x, ++itCell)
        {
            if (*itCell == 'O')
                sumGps += x + (100 * y);
        }
    }

    return sumGps;
}


// -------------------------------------------------------------------------------------------------------------------------

struct Box
{
    int minX, maxX, y;
    bool isWall = false;

    void move(int dx, int dy)
    {
        assert(!isWall);
        minX += dx;
        maxX += dx;
        y += dy;
    }
};
using BoxP = unique_ptr<Box>;
using Warehouse = vector<Box*>;


Box* find_box_at(int x, int y, const Warehouse& warehouse)
{
    if (x < 0 || y < 0)
        return nullptr;

    auto itFound = ranges::find_if(warehouse, [x,y](const Box* box)
        {
            return (box->y == y && box->minX <= x && box->maxX >= x);
        });

    if (itFound == end(warehouse))
        return nullptr;
    return *itFound;
}

optional<vector<Box*>> try_gather_boxes_to_move_left(const Warehouse& warehouse, int startX, int startY)
{
    vector<Box*> boxes;

    for (int x = startX - 1; ; x -= 2)
    {
        Box* box = find_box_at(x, startY, warehouse);
        if (!box)
            return { boxes };
        if (box->isWall)
            return {};
        boxes.push_back(box);
    }
}

optional<vector<Box*>> try_gather_boxes_to_move_right(const Warehouse& warehouse, int startX, int startY)
{
    vector<Box*> boxes;

    for (int x = startX + 1; ; x += 2)
    {
        Box* box = find_box_at(x, startY, warehouse);
        if (!box)
            return { boxes };
        if (box->isWall)
            return {};
        boxes.push_back(box);
    }
}

optional<vector<Box*>> try_gather_boxes_to_move_vert(const Warehouse& warehouse, int startX, int startY, int dir)
{
    vector<Box*> boxes;

    Box* firstBox = find_box_at(startX, startY + dir, warehouse);
    if (!firstBox)
        return { boxes };   // it's an empty space we can just move into
    if (firstBox->isWall)
        return {};

    vector<Box*> open = { firstBox };
    while (!open.empty())
    {
        Box* box = open.back();
        open.pop_back();

        if (!ranges::contains(boxes, box))
            boxes.push_back(box);

        for (int x = box->minX; x <= box->maxX; ++x)
        {
            Box* nextBox = find_box_at(x, box->y + dir, warehouse);
            if (!nextBox)
                continue;

            if (nextBox->isWall)
                return {};

            if (!ranges::contains(open, nextBox))
                open.push_back(nextBox);
        }
    }

    return { boxes };
}

void make_move_vert(Pt2i& pos, Warehouse& warehouse, int dir)
{
    optional<vector<Box*>> boxes = try_gather_boxes_to_move_vert(warehouse, pos.x, pos.y, dir);
    if (!boxes.has_value())
        return;  // hit a wall

    for (Box* box : *boxes)
        box->move(0, dir);

    pos.y += dir;
}

void make_move(char move, Pt2i& pos, Warehouse& warehouse)
{
    switch (move)
    {
    case '<':   // LEFT
    {
        optional<vector<Box*>> boxes = try_gather_boxes_to_move_left(warehouse, pos.x, pos.y);
        if (!boxes.has_value())
            break;  // hit a wall

        for (Box* box : *boxes)
            box->move(-1, 0);

        --pos.x;

        break;
    }

    case '>':   // RIGHT
    {
        optional<vector<Box*>> boxes = try_gather_boxes_to_move_right(warehouse, pos.x, pos.y);
        if (!boxes.has_value())
            break;  // hit a wall

        for (Box* box : *boxes)
            box->move(1, 0);

        ++pos.x;

        break;
    }

    case '^':   // UP
        make_move_vert(pos, warehouse, -1);
        break;

    case 'v':   // DOWN
        make_move_vert(pos, warehouse, 1);
        break;
    }
}

void dump_warehouse(const Warehouse& warehouse, int w, int h, const Pt2i& pos)
{
    string emptyLine(w, '.');
    vector<string> lines(h, emptyLine);

    for (const Box* box : warehouse)
    {
        string& line = lines[box->y];
        if (box->isWall)
        {
            for (int x = box->minX; x <= box->maxX; ++x)
                line[x] = '#';
        }
        else
        {
            assert(box->maxX == box->minX + 1);
            line[box->minX] = '[';
            line[box->maxX] = ']';
        }
    }

    lines[pos.y][pos.x] = '@';

    for (const string& line : lines)
        cout << line << '\n';

    cout << endl;
}

i64 day15_2(const stringlist& input)
{
    vector<unique_ptr<Box>> all_boxes;
    Warehouse warehouse;

    auto itGridStart = begin(input);
    auto itGridEnd = find(begin(input), end(input), ""s);
    const int inWidth = int(itGridStart->size());
    const int expWidth = inWidth * 2;
    const int h = int(itGridEnd - itGridStart);
    auto itLine = itGridStart;
    Pt2i startPos{ -1, -1 };

    // top wall
    all_boxes.push_back(make_unique<Box>(0, expWidth - 1, 0, true));
    warehouse.push_back(all_boxes.back().get());
    ++itLine;

    for (int y = 1; y < h-1; ++y, ++itLine)
    {
        auto itCell = begin(*itLine);
        for (int x = 0; x < inWidth; ++x, ++itCell)
        {
            switch (*itCell)
            {
            case '@':
                startPos = { x*2, y };
                break;

            case '#':
                all_boxes.push_back(make_unique<Box>(x * 2, x * 2 + 1, y, true));
                warehouse.push_back(all_boxes.back().get());
                break;

            case 'O':
                all_boxes.push_back(make_unique<Box>(x * 2, x * 2 + 1, y, false));
                warehouse.push_back(all_boxes.back().get());
                break;

            case '.':
                break;

            default:
                assert(false);
            }
        }
    }

    // bottom wall
    all_boxes.push_back(make_unique<Box>(0, expWidth - 1, h - 1, true));
    warehouse.push_back(all_boxes.back().get());
    ++itLine;


    //cout << "Initial warehouse --------------------\n";
    //dump_warehouse(warehouse, expWidth, h, startPos);

    // process moves
    Pt2i pos = startPos;
   // int moveIx = 1;
    for (++itLine; itLine != end(input); ++itLine)
    {
        for (char move : *itLine)
        {
            make_move(move, pos, warehouse);

            //cout << "After move " << (moveIx++) << ": " << move << " ---------------------\n";
            //dump_warehouse(warehouse, expWidth, h, pos);


            //if (moveIx > 25)
            //    return -1;
        }
    }


    i64 sumGps = 0;
    for (const Box* box : warehouse)
    {
        if (!box->isWall)
            sumGps += box->minX + (100 * box->y);
    }

    return sumGps;
}


void run_day15()
{
    string sample =
R"(########
#..O.O.#
##@.O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

<^^>>>vv<v>>v<<)";
    string sample2 =
R"(##########
#..O..O.O#
#......O.#
#.OO..O.O#
#..O@..O.#
#O#..O...#
#O..O..O.#
#.OO.O.OO#
#....O...#
##########

<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^
vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v
><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<
<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^
^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><
^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^
>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^
<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>
^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>
v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^)";

    test(2028, day15(READ(sample)));
    test(10092, day15(READ(sample2)));
    gogogo(day15(LOAD(15)));

    test(9021, day15_2(READ(sample2)));
    nononoD(day15_2(LOAD(15)));
}