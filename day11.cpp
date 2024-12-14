#include "pch.h"
#include "harness.h"


pair<i64, i64> blink(i64 stone)
{
    if (stone == 0)
        return { 1, -1 };

    if (stone >= 10 && stone < 100)
        return { i8(stone) / 10, i8(stone) % 10 };

    if (stone >= 1000 && stone < 10000)
        return { i16(stone) / 100, i16(stone) % 100 };

    if (stone >= 100'000 && stone < 1'000'000)
        return { i32(stone) / 1000, i32(stone) % 1000 };

    if (stone >= 10'000'000 && stone < 100'000'000)
        return { i32(stone) / 10000, i32(stone) % 10000 };

    if (stone >= 1'000'000'000 && stone < 10'000'000'000)
        return { stone / 100000, stone % 100000 };

    if (stone >= 100'000'000'000 && stone < 1'000'000'000'000)
        return { stone / 1'000'000, stone % 1'000'000 };

    assert(stone < 10'000'000'000'000);

    return { stone * 2024, -1 };
}

i64 count_blinking_stones(i64 stone, int blinks)
{
    if (blinks == 0)
        return 1;

    auto [left, right] = blink(stone);

    i64 newStones = count_blinking_stones(left, blinks - 1);

    if (right >= 0)
        newStones += count_blinking_stones(right, blinks - 1);

    return newStones;
}

i64 day11(const string& input, int numBlinks = 25)
{
    vector<int> initialStones = string_to_intlist(input);

    i64 newStones = 0;
    for (int stone : initialStones)
    {
        newStones += count_blinking_stones(stone, numBlinks);
    }

    return newStones;
}


void run_day11()
{
    string sample = "125 17";

    test(22, day11(sample, 6));
    test(55312, day11(sample));
    gogogo(day11(LOADSTR(11)));

    //gogogo(day11(LOADSTR(11), 75));
    skip("need to think about this!");
}