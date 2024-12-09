#include "pch.h"
#include "harness.h"


i64 day9(const string& input)
{
    i64 checksum = 0;
    int diskPos = 0;    // current write pos
    
    auto writeBlocks = [&checksum, &diskPos](int fileId, int numBlocks) -> void
        {
            for (int i = 0; i < numBlocks; ++i)
            {
                checksum += fileId * diskPos;
                ++diskPos;
            }
        };

    auto getFileId = [](int inputPos) { return inputPos / 2; };

    int tailPos = int(size(input)) - 1;
    int tailUnused = input[tailPos] - '0';

    int headPos = 0;
    while (tailPos > headPos)
    {
        // head file
        writeBlocks(getFileId(headPos), input[headPos] - '0');

        // tail file(s) in the gap
        int gapSize = input[headPos + 1] - '0';
        while (gapSize > 0 && tailPos > headPos)
        {
            if (gapSize < tailUnused)
            {
                writeBlocks(getFileId(tailPos), gapSize);
                tailUnused -= gapSize;
                gapSize = 0;
            }
            else
            {
                writeBlocks(getFileId(tailPos), tailUnused);
                gapSize -= tailUnused;

                tailPos -= 2;
                tailUnused = input[tailPos] - '0';
            }
        }

        headPos += 2;
    }
    writeBlocks(getFileId(tailPos), tailUnused);

    return checksum;
}

i64 day9_2(const string& input)
{
    int diskSize = int(size(input));
    string diskBuf = input;
    char* disk = diskBuf.data();

    i64 checksum = 0;
    int diskPos = 0;    // current write pos

    auto writeBlocks = [&checksum, &diskPos](int fileId, int numBlocks) -> void
        {
            for (int i = 0; i < numBlocks; ++i)
            {
                checksum += fileId * diskPos;
                ++diskPos;
            }
        };
    auto writeSpace = [&checksum, &diskPos](int numBlocks) -> void
        {
            diskPos += numBlocks;
        };

    auto getFileId = [](int inputPos) { return inputPos / 2; };

    int headPos = 0;
    while (headPos < diskSize)
    {
        // head file
        if (disk[headPos] < 'A')
            writeBlocks(getFileId(headPos), disk[headPos] - '0');
        else
            writeSpace(disk[headPos] - 'A');

        // find tail file(s) to fit in the gap
        int gapSize = disk[headPos + 1] - '0';
        while (gapSize > 0)
        {
            int tailPos = diskSize - 1;
            int tailSize = -1;
            for ( ; tailPos > headPos; tailPos -= 2)
            {
                if (disk[tailPos] < 'A')
                {
                    tailSize = disk[tailPos] - '0';
                    if (tailSize <= gapSize)
                        break;
                }
            }
            // no more tail files to fit into this gap
            if (tailPos <= headPos)
                break;

            writeBlocks(getFileId(tailPos), tailSize);
            gapSize -= tailSize;
            disk[tailPos] += 'A' - '0';

            tailPos -= 2;
        }

        writeSpace(gapSize);

        headPos += 2;
    }

    return checksum;
}


void run_day9()
{
    string sample = "2333133121414131402";

    test(1928, day9(sample));
    gogogo(day9(LOADSTR(9)));

    test(2858, day9_2(sample));
    gogogo(day9_2(LOADSTR(9)));
}