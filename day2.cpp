#include "pch.h"
#include "harness.h"


int day2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day2_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day2()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day2(READ(sample)));
    //gogogo(day2(LOAD(2)));

    //test(-100, day2_2(READ(sample)));
    //gogogo(day2_2(LOAD(2)));
}