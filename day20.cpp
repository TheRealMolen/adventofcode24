#include "pch.h"
#include "harness.h"


int day20(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day20_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day20()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day20(READ(sample)));
    //gogogo(day20(LOAD(20)));

    //test(-100, day20_2(READ(sample)));
    //gogogo(day20_2(LOAD(20)));
}