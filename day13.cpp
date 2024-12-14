#include "pch.h"
#include "harness.h"
#include "pt2.h"



i64 findMinTokens(Pt2i64 moveA, Pt2i64 moveB, Pt2i64 prize)
{
    // find intersection of line A thru origin and line B thru P
    // => find s & t such that: sA = P + tB
    const Pt2i64 perpA = moveA.perp();
    const Pt2i64 perpB = moveB.perp();
    i64 collinearityA = moveA.dot(perpB);
    if (collinearityA == 0)
        return -1;

    i64 modS = prize.dot(perpB) % collinearityA;
    if (modS != 0)
        return -1;

    i64 collinearityB = moveB.dot(perpA);
    assert(collinearityB != 0);

    i64 modT = (-prize).dot(perpA) % collinearityB;
    if (modT != 0)
        return -1;

    i64 s = prize.dot(perpB) / collinearityA;
    i64 t = prize.dot(perpA) / collinearityB;

    constexpr int tokensA = 3;
    constexpr int tokensB = 1;

    return s * tokensA + t * tokensB;
}



i64 day13(const stringlist& input)
{
    i64 minTokens = 0;
    for (auto itLine = begin(input); itLine != end(input); ++itLine)
    {
        if (itLine->empty())
            continue;

        Pt2i64 moveA, moveB, prize;

        istringstream isA(*(itLine++));
        isA >> "Button A: X+" >> moveA.x >> ", Y+" >> moveA.y;

        istringstream isB(*(itLine++));
        isB >> "Button B: X+" >> moveB.x >> ", Y+" >> moveB.y;

        istringstream isP(*itLine);
        isP >> "Prize: X=" >> prize.x >> ", Y=" >> prize.y;

        i64 tokens = findMinTokens(moveA, moveB, prize);
        if (tokens > 0)
            minTokens += tokens;
    }

    return minTokens;
}

i64 day13_2(const stringlist& input)
{
    i64 minTokens = 0;
    for (auto itLine = begin(input); itLine != end(input); ++itLine)
    {
        if (itLine->empty())
            continue;

        Pt2i64 moveA, moveB, prize;

        istringstream isA(*(itLine++));
        isA >> "Button A: X+" >> moveA.x >> ", Y+" >> moveA.y;

        istringstream isB(*(itLine++));
        isB >> "Button B: X+" >> moveB.x >> ", Y+" >> moveB.y;

        istringstream isP(*itLine);
        isP >> "Prize: X=" >> prize.x >> ", Y=" >> prize.y;

        prize += Pt2i64{ 10000000000000, 10000000000000 };

        i64 tokens = findMinTokens(moveA, moveB, prize);
        if (tokens > 0)
            minTokens += tokens;
    }

    return minTokens;
}


void run_day13()
{
    string sample =
R"(Button A: X+94, Y+34
Button B: X+22, Y+67
Prize: X=8400, Y=5400

Button A: X+26, Y+66
Button B: X+67, Y+21
Prize: X=12748, Y=12176

Button A: X+17, Y+86
Button B: X+84, Y+37
Prize: X=7870, Y=6450

Button A: X+69, Y+23
Button B: X+27, Y+71
Prize: X=18641, Y=10279)";

    test(480, day13(READ(sample)));
    gogogo(day13(LOAD(13)));

    gogogo(day13_2(LOAD(13)));
}