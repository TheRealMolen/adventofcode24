#include "pch.h"
#include "harness.h"


class Machine17
{
public:
    Machine17(const stringlist& input);

    void run();
    int checkForQuine() const;  // 1 == yes, -1 == no, 0 == maybe

    i64 getReg(int i) const { return mRegs[i]; }
    void setReg(int i, i64 val) { mRegs[i] = val; }

    string getOutput() const;

private:
    void tick();


    i64 mRegs[3];
    int mIP = 0;
    vector<u16> mProgram;

    vector<u8> mOutput;
    bool mPotentialQuine = true;
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

int Machine17::checkForQuine() const
{
    if (!mPotentialQuine)
        return -1;

    if (size(mOutput) == size(mProgram))
        return 1;

    return 0;
}

void Machine17::tick()
{
    enum EOpCode { ADV, BXL, BST, JNZ, BXC, OUT_, BDV, CDV };
    EOpCode instruction = EOpCode(mProgram[mIP++]);
    int operand = mProgram[mIP++];

    auto getComboVal = [=, this]() -> i64
        {
            if (operand < 4)
                return operand;
            assert(operand < 7);
            return mRegs[operand - 4];
        };

    auto getDivVal = [=, this, &getComboVal]() -> i64
        {
            i64 num = mRegs[0];
            i32 denom = 1 << getComboVal();
            return num / denom;
        };

    switch (EOpCode(instruction))
    {
    case ADV:     // ADV
        mRegs[0] = getDivVal();
        break;

    case BXL:     // BXL
        mRegs[1] = mRegs[1] ^ operand;
        break;

    case BST:     // BST
        mRegs[1] = getComboVal() & 7;
        break;

    case JNZ:     // JNZ
        if (mRegs[0] != 0)
            mIP = operand;
        break;

    case BXC:     // BXC
        mRegs[1] = mRegs[1] ^ mRegs[2];
        break;

    case OUT_:     // OUT
        mOutput.push_back(u8(getComboVal() & 0x7));

        if (size(mOutput) > size(mProgram))
            mPotentialQuine = false;
        else if (mOutput.back() != mProgram[size(mOutput) - 1])
            mPotentialQuine = false;

        break;

    case BDV:     // BDV
        mRegs[1] = getDivVal();
        break;

    case CDV:     // CDV
        mRegs[2] = getDivVal();
        break;

    default:
        assert(false);
    }
}

string Machine17::getOutput() const
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

    return machine.getOutput();
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

    skip("done in python");
}