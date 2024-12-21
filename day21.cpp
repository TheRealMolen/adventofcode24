#include "pch.h"
#include "harness.h"


int day21(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day21_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day21()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day21(READ(sample)));
    //gogogo(day21(LOAD(21)));

    //test(-100, day21_2(READ(sample)));
    //gogogo(day21_2(LOAD(21)));
}