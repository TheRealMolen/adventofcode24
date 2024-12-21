#include "pch.h"
#include "harness.h"


enum EOpCode { ASHR, BXL, BST, JNZ, BXC, OUT_, BSHR, CSHR };


class Machine17
{
public:
    Machine17(const stringlist& input);

    void run();
    void reset()
    {
        mIP = 0;
        mOutput.clear();
    }

    i64 getReg(int i) const { return mRegs[i]; }
    void setRegA(i64 val) { mRegs[0] = val; }

    const vector<u16>& getProgram() const { return mProgram; }
    const vector<u8>& getRawOutput() const { return mOutput; }
    string getOutputStr() const;

    // check that the program has "JNZ 0" at the end
    bool hasLoopAtEnd() const
    {
        return size(mProgram) > 1 && (mProgram.back() == 0) && (*(end(mProgram) - 2) == JNZ);
    }

    void dropTrailingJump()
    {
        assert(hasLoopAtEnd());
        mProgram.erase(end(mProgram) - 2, end(mProgram));
    }

private:
    void tick();

    i64 getComboVal(int operand)
    {
        if (operand < 4)
            return operand;
        assert(operand < 7);
        return mRegs[operand - 4];
    };
    i64 getShrVal(int operand)
    {
        i64 num = mRegs[0];
        return num >> getComboVal(operand);
    };


    i64 mRegs[3];
    int mIP = 0;
    vector<u16> mProgram;

    vector<u8> mOutput;
};

Machine17::Machine17(const stringlist& input)
{
    auto itLine = begin(input);

    istringstream regA(*(itLine++));
    istringstream regB(*(itLine++));
    istringstream regC(*(itLine++));
    ++itLine;
    istringstream program(*itLine);

    regA >> "Register A: " >> mRegs[0];
    regB >> "Register B: " >> mRegs[1];
    regC >> "Register C: " >> mRegs[2];

    string progStr;
    program >> "Program: " >> progStr;
    mProgram = string_to_intlist<u16>(progStr, ',');
}

void Machine17::run()
{
    while (size_t(mIP) < size(mProgram))
        tick();
}

void Machine17::tick()
{
    EOpCode instruction = EOpCode(mProgram[mIP++]);
    int operand = mProgram[mIP++];

    switch (EOpCode(instruction))
    {
    case ASHR:     // ADV
        mRegs[0] = getShrVal(operand);
        break;

    case BXL:     // BXL
        mRegs[1] = mRegs[1] ^ operand;
        break;

    case BST:     // BST
        mRegs[1] = getComboVal(operand) & 7;
        break;

    case JNZ:     // JNZ
        if (mRegs[0] != 0)
            mIP = operand;
        break;

    case BXC:     // BXC
        mRegs[1] = mRegs[1] ^ mRegs[2];
        break;

    case OUT_:     // OUT
        mOutput.push_back(u8(getComboVal(operand) & 0x7));
        break;

    case BSHR:     // BDV
        mRegs[1] = getShrVal(operand);
        break;

    case CSHR:     // CDV
        mRegs[2] = getShrVal(operand);
        break;

    default:
        assert(false);
    }
}

string Machine17::getOutputStr() const
{
    ostringstream os;
    for (const u8& i : mOutput)
    {
        if (&i != &mOutput.front())
            os << ',';

        os << int(i);
    }

    return os.str();
}


string day17(const stringlist& input)
{
    Machine17 machine(input);

    machine.run();

    return machine.getOutputStr();
}


i64 decodeRemainder(const vector<u16>& target, Machine17 machine, size_t ix, i64 prevA)
{
    if (ix == size(target))
        return prevA;

    for (int guess = 0; guess < 8; ++guess)
    {
        i64 aGuess = (prevA << 3) + guess;

        machine.reset();
        machine.setRegA(aGuess);
        machine.run();

        if (machine.getRawOutput().front() == target[ix])
        {
            i64 decodedA = decodeRemainder(target, machine, ix + 1, aGuess);
            if (decodedA > 0)
                return decodedA;
        }
    }

    return -1;
}

i64 day17_2(const stringlist& input)
{
    Machine17 rawMachine(input);

    Machine17 machine(rawMachine);
    machine.dropTrailingJump();

    vector<u16> target = rawMachine.getProgram() | views::reverse | ranges::to<vector<u16>>();

    return decodeRemainder(target, machine, 0, 0);
}


void run_day17()
{
    string sample =
        R"(Register A: 729
Register B: 0
Register C: 0

Program: 0,1,5,4,3,0)";

    string sample2 =
R"(Register A: 0
Register B: 0
Register C: 9

Program: 2,6)";

    string sample3 =
R"(Register A: 2024
Register B: 0
Register C: 0

Program: 0,3,5,4,3,0)";


    test(1, [&](const auto& input) { Machine17 m(input); m.run(); return m.getReg(1); }(READ(sample2)));
    test("4,6,3,5,6,3,5,2,1,0", day17(READ(sample)));
    gogogo(day17(LOAD(17)));

    test(117440, day17_2(READ(sample3)));
    gogogo(day17_2(LOAD(17)));
}