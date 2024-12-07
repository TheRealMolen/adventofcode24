#include "pch.h"
#include "harness.h"


pair<u64, vector<u32>> parseEquation(const string& line)
{
    istringstream is(line);
    u64 result;
    is >> result >> ":" >> skipws;

    vector<u32> operands;
    for (;;)
    {
        int o;
        is >> o;
        if (!is)
            break;
        
        operands.push_back(o);
    }

    return { result, operands };
}

u64 concatInts(u64 a, u64 b)
{
    u32 mul = 10;
    u64 shiftA = a * 10;
    while (mul <= b)
    {
        mul *= 10;
        shiftA *= 10;
    }
    return shiftA + b;
}


template<bool ConcatAllowed>
bool isEquationPossible(u64 result, auto itOperand, auto itEnd, u64 acc)
{
    if (itOperand == itEnd)
        return acc == result;
    if (acc > result)
        return false;

    u64 currOperand = *itOperand;
    ++itOperand;

    // PLUS
    if (isEquationPossible<ConcatAllowed>(result, itOperand, itEnd, acc + currOperand))
        return true;

    // MULT
    if (isEquationPossible<ConcatAllowed>(result, itOperand, itEnd, acc * currOperand))
        return true;

    // CONCAT
    if constexpr (ConcatAllowed)
    {
        if (isEquationPossible<ConcatAllowed>(result, itOperand, itEnd, concatInts(acc, currOperand)))
            return true;
    }

    return false;
}

template<bool ConcatAllowed = false>
bool isEquationPossible(u64 result, const vector<u32>& operands)
{
    u64 acc = operands.front();

    return isEquationPossible<ConcatAllowed>(result, operands.data() + 1, (&operands.back()) + 1, acc);
}


u64 day7(const stringlist& input)
{
    u64 calibration = 0;
    for (auto& line : input)
    {
        auto [result, operands] = parseEquation(line);
        if (isEquationPossible(result, operands))
        {
            calibration += result;
        }
    }

    return calibration;
}

u64 day7_2(const stringlist& input)
{
    u64 calibration = 0;
    for (auto& line : input)
    {
        auto [result, operands] = parseEquation(line);
        if (isEquationPossible<true>(result, operands))
        {
            calibration += result;
        }
    }

    return calibration;
}


void run_day7()
{
    string sample =
R"(190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20)";

    test(3749, day7(READ(sample)));
    gogogo(day7(LOAD(7)));

    test(11387, day7_2(READ(sample)));
    gogogo(day7_2(LOAD(7)));
}