#include "pch.h"
#include "harness.h"


int day1(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day1_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day1()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day1(READ(sample)));
    //gogogo(day1(LOAD(1)));

    //test(-100, day1_2(READ(sample)));
    //gogogo(day1_2(LOAD(1)));
}