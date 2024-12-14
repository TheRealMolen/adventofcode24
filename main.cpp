﻿#include "pch.h"

#include "harness.h"

// -------------------------------------------------------------------

void run_day1();
void run_day2();
void run_day3();
void run_day4();
void run_day5();
void run_day6();
void run_day7();
void run_day8();
void run_day9();
void run_day10();
void run_day11();
void run_day12();
void run_day13();
void run_day14();
void run_day15();
void run_day16();
void run_day17();
void run_day18();
void run_day19();
void run_day20();
void run_day21();
void run_day22();
void run_day23();
void run_day24();
void run_day25();

// -------------------------------------------------------------------

int main()
{
    initcolours();
    srand((unsigned int)time(0));

    cout << GARLAND(2) << "  advent of code 2024  " << GARLAND(2) << endl;

    {
        TIME_SCOPE(old);

        run_day1();
        run_day2();
        run_day3();
        run_day4();
        run_day5();
        run_day6();
        run_day7();
        run_day8();
        run_day9();
        run_day10();
        run_day11();
        run_day12();
    }
    {
        TIME_SCOPE(new);

        run_day13();
        // run_day14();
        // run_day15();
        // run_day16();
        // run_day17();
        //run_day18();
        //run_day19();
        //run_day20();
        //run_day21();
        //run_day22();
        //run_day23();
        //run_day24();
        //run_day25();
    }

    // animate snow falling behind the characters in the console until someone presses a key
    return twinkleforever();
}

