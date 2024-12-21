#include "pch.h"
#include "harness.h"


bool pattern_is_possible(const string& target, const vector<vector<string>>& availableByFirstChar, vector<u8>& ruledOutBeyond, size_t offs=0)
{
    if (offs == target.length())
        return true;
    if (offs > target.length())
        return false;
    if (ruledOutBeyond[offs])
        return false;

    const char* pCurrTargetBase = target.c_str() + offs;

    for (const string& pattern : availableByFirstChar[*pCurrTargetBase - 'a'])
    {
        const char* pCurrTarget = pCurrTargetBase;
        const char* pCurrPatt = pattern.c_str();

        // we know the first chars match
        ++pCurrTarget;
        ++pCurrPatt;

        // skip to end of shortest, or first mismatch
        for (; *pCurrTarget == *pCurrPatt && *pCurrTarget; ++pCurrTarget, ++pCurrPatt)
        { /**/ }

        // mismatch detected before end of pattern; that's a no
        if (*pCurrPatt)
            continue;

        size_t newOffs = offs + pattern.length();
        if (pattern_is_possible(target, availableByFirstChar, ruledOutBeyond, newOffs))
            return true;
        else
            ruledOutBeyond[newOffs] = 1;
    }

    return false;
}

int day19(const stringlist& input)
{
    vector<vector<string>> availableByFirstChar('z' - 'a', {});
    for (string& avail : split(input.front(), ", "s))
        availableByFirstChar[avail[0] - 'a'].emplace_back(move(avail));
    for (vector<string>& patterns : availableByFirstChar)
        ranges::sort(patterns);

    int numPossible = 0;
    for (auto& target : input | views::drop(2))
    {
        vector<u8> ruledOutBeyond(size(target), 0);
        const bool possible = pattern_is_possible(target, availableByFirstChar, ruledOutBeyond);
        if (possible)
            ++numPossible;
    }

    return numPossible;
}



void count_solutions(const string& target, const vector<vector<string>>& availableByFirstChar, vector<i64>& solutionsForRemainder, size_t offs = 0)
{
    const size_t targetLength = target.length();
    assert(offs < targetLength);

    const char* pCurrTargetBase = target.c_str() + offs;

    for (const string& pattern : availableByFirstChar[*pCurrTargetBase - 'a'])
    {
        const size_t patternLength = pattern.length();
        if (offs + patternLength > targetLength)
            continue;

        const char* pCurrTarget = pCurrTargetBase;
        const char* pCurrPatt = pattern.c_str();

        // we know the first chars match
        ++pCurrTarget;
        ++pCurrPatt;

        // skip to end of shortest, or first mismatch
        for (; *pCurrTarget == *pCurrPatt && *pCurrTarget; ++pCurrTarget, ++pCurrPatt)
        { /**/ }

        // mismatch detected before end of pattern; that's a no
        if (*pCurrPatt)
            continue;

        size_t newOffs = offs + patternLength;
        if (newOffs == targetLength)
            ++solutionsForRemainder[offs];
        else
            solutionsForRemainder[offs] += solutionsForRemainder[newOffs];
    }
}

i64 day19_2(const stringlist& input)
{
    vector<vector<string>> availableByFirstChar('z' - 'a', {});
    for (string& avail : split(input.front(), ", "s))
        availableByFirstChar[avail[0] - 'a'].emplace_back(move(avail));
    for (vector<string>& patterns : availableByFirstChar)
        ranges::sort(patterns);

    i64 numSolutions = 0;
    for (const auto& target : input | views::drop(2))
    {
        vector<i64> solutionsForRemainder(size(target), 0);

        for (size_t offs = target.length() - 1; offs < target.length(); --offs)
            count_solutions(target, availableByFirstChar, solutionsForRemainder, offs);

        numSolutions += solutionsForRemainder.front();
    }

    return numSolutions;
}


void run_day19()
{
    string sample =
R"(r, wr, b, g, bwu, rb, gb, br

brwrr
bggr
gbbr
rrbgbr
ubwu
bwurrg
brgr
bbrgwb)";

    test(6, day19(READ(sample)));
    gogogo(day19(LOAD(19)));

    test(16, day19_2(READ(sample)));
    gogogo(day19_2(LOAD(19)));
}