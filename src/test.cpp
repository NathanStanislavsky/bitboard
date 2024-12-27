#include <iostream>
#include <string>
#include <vector>
#include "test.h"

struct TestCase
{
    std::string fen;
    int depth;
    int expected;
};

void test_fens()
{
    std::vector<TestCase> test_cases = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         6,
         119060324},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
         5,
         193690690},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
         7,
         193690690},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
         5,
         15833292},
        {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
         5,
         89941194},
        {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
         5,
         164075551}
    };

    for (const TestCase& test : test_cases)
    {
        std::cout << "Testing FEN: " << test.fen << "\nDepth: " << test.depth << std::endl;

        Pos pos(test.fen);

        pos.print_board();

        int result = perft(pos, test.depth, true);

        if (result == test.expected)
        {
            std::cout << "Perft Result: " << result << " [PASS]" << std::endl;
        }
        else
        {
            std::cout << "Perft Result: " << result << " [FAIL] Expected: " << test.expected << std::endl;
        }

        std::cout << "----------------------------------------" << std::endl;
    }
}