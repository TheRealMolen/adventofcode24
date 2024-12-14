#include "pch.h"
#include "harness.h"


int day13(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day13_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day13()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day13(READ(sample)));
    //gogogo(day13(LOAD(13)));

    //test(-100, day13_2(READ(sample)));
    //gogogo(day13_2(LOAD(13)));
}