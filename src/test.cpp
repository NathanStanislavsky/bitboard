#include <iostream>
#include <string>
#include <vector>

#include "test.h"
#include "pos.h"
#include "move_gen.h"
#include "timer.h"

struct TestCase
{
    std::string fen;
    int depth;
    int expected;
};

void test_fens()
{
    std::vector<TestCase> test_cases = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6, 119060324},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 5, 193690690},
        {"4k3/8/8/8/8/8/8/4K2R w K - 0 1", 6, 764643},
        {"4k3/8/8/8/8/8/8/R3K3 w Q - 0 1", 6, 846648},
        {"4k2r/8/8/8/8/8/8/4K3 w k - 0 1", 6, 899442},
        {"r3k3/8/8/8/8/8/8/4K3 w q - 0 1", 6, 1001523},
        {"4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1", 6, 2788982},
        {"r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1", 6, 3517770},
        {"8/8/8/8/8/8/6k1/4K2R w K - 0 1", 6, 185867},
        {"8/8/8/8/8/8/1k6/R3K3 w Q - 0 1", 6, 413018},
        {"4k2r/6K1/8/8/8/8/8/8 w k - 0 1", 6, 179869},
        {"r3k3/1K6/8/8/8/8/8/8 w q - 0 1", 6, 367724},
        {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", 6, 179862938},
        {"r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1", 6, 195629489},
        {"r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1", 6, 184411439},
        {"r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1", 6, 189224276},
        {"1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", 6, 198328929},
        {"2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", 6, 185959088},
        {"r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1", 6, 190755813},
        {"4k3/8/8/8/8/8/8/4K2R b K - 0 1", 6, 899442},
        {"4k3/8/8/8/8/8/8/R3K3 b Q - 0 1", 6, 1001523},
        {"4k2r/8/8/8/8/8/8/4K3 b k - 0 1", 6, 764643},
        {"r3k3/8/8/8/8/8/8/4K3 b q - 0 1", 6, 846648},
        {"4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1", 6, 3517770},
        {"r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1", 6, 2788982},
        {"4k2r/6K1/8/8/8/8/8/8 b k - 0 1", 6, 185867},
        {"r3k3/1K6/8/8/8/8/8/8 b q - 0 1", 6, 413018},
        {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", 6, 179862938},
        {"r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1", 6, 198328929},
        {"r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1", 6, 185959088},
        {"r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1", 6, 190755813},
        {"1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", 6, 195629489},
        {"2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1", 6, 184411439},
        {"r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1", 6, 189224276},
        {"8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1", 6, 8107539},
        {"8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1", 6, 2594412},
        {"8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1", 6, 19870403},
        {"K7/8/2n5/1n6/8/8/8/k6N w - - 0 1", 6, 588695},
        {"k7/8/2N5/1N6/8/8/8/K6n w - - 0 1", 6, 688780},
        {"8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1", 6, 8503277},
        {"8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1", 6, 3147566},
        {"8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1", 6, 4405103},
        {"K7/8/2n5/1n6/8/8/8/k6N b - - 0 1", 6, 688780},
        {"k7/8/2N5/1N6/8/8/8/K6n b - - 0 1", 6, 588695},
        {"B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1", 6, 22823890},
        {"8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1", 6, 28861171},
        {"k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1", 6, 7881673},
        {"K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1", 6, 7382896},
        {"B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1", 6, 9250746},
        {"8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1", 6, 29027891},
        {"k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1", 6, 7382896},
        {"K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1", 6, 7881673},
        {"7k/RR6/8/8/8/8/rr6/7K w - - 0 1", 6, 44956585},
        {"R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1", 6, 525169084},
        {"7k/RR6/8/8/8/8/rr6/7K b - - 0 1", 6, 44956585},
        {"R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1", 6, 524966748},
        {"6kq/8/8/8/8/8/8/7K w - - 0 1", 6, 391507},
        {"6KQ/8/8/8/8/8/8/7k b - - 0 1", 6, 391507},
        {"K7/8/8/3Q4/4q3/8/8/7k w - - 0 1", 6, 3370175},
        {"6qk/8/8/8/8/8/8/7K b - - 0 1", 6, 419369},
        {"6KQ/8/8/8/8/8/8/7k b - - 0 1", 6, 391507},
        {"K7/8/8/3Q4/4q3/8/8/7k b - - 0 1", 6, 3370175},
        {"8/8/8/8/8/K7/P7/k7 w - - 0 1", 6, 6249},
        {"8/8/8/8/8/7K/7P/7k w - - 0 1", 6, 6249},
        {"K7/p7/k7/8/8/8/8/8 w - - 0 1", 6, 2343},
        {"7K/7p/7k/8/8/8/8/8 w - - 0 1", 6, 2343},
        {"8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1", 6, 34834},
        {"8/8/8/8/8/K7/P7/k7 b - - 0 1", 6, 2343},
        {"8/8/8/8/8/7K/7P/7k b - - 0 1", 6, 2343},
        {"K7/p7/k7/8/8/8/8/8 b - - 0 1", 6, 6249},
        {"7K/7p/7k/8/8/8/8/8 b - - 0 1", 6, 6249},
        {"8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1", 6, 34822},
        {"8/8/8/8/8/4k3/4P3/4K3 w - - 0 1", 6, 11848},
        {"4k3/4p3/4K3/8/8/8/8/8 b - - 0 1", 6, 11848},
        {"8/8/7k/7p/7P/7K/8/8 w - - 0 1", 6, 10724},
        {"8/8/k7/p7/P7/K7/8/8 w - - 0 1", 6, 10724},
        {"8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1", 6, 53138},
        {"8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1", 6, 157093},
        {"8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1", 6, 158065},
        {"k7/8/3p4/8/3P4/8/8/7K w - - 0 1", 6, 20960},
        {"8/8/7k/7p/7P/7K/8/8 b - - 0 1", 6, 10724},
        {"8/8/k7/p7/P7/K7/8/8 b - - 0 1", 6, 10724},
        {"8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1", 6, 53138},
        {"8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1", 6, 158065},
        {"8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1", 6, 157093},
        {"k7/8/3p4/8/3P4/8/8/7K b - - 0 1", 6, 21104},
        {"7k/3p4/8/8/3P4/8/8/K7 w - - 0 1", 6, 32191},
        {"7k/8/8/3p4/8/8/3P4/K7 w - - 0 1", 6, 30980},
        {"k7/8/8/7p/6P1/8/8/K7 w - - 0 1", 6, 41874},
        {"k7/8/7p/8/8/6P1/8/K7 w - - 0 1", 6, 29679},
        {"k7/8/8/6p1/7P/8/8/K7 w - - 0 1", 6, 41874},
        {"k7/8/6p1/8/8/7P/8/K7 w - - 0 1", 6, 29679},
        {"k7/8/8/3p4/4p3/8/8/7K w - - 0 1", 6, 22886},
        {"k7/8/3p4/8/8/4P3/8/7K w - - 0 1", 6, 28662},
        {"7k/3p4/8/8/3P4/8/8/K7 b - - 0 1", 6, 32167},
        {"7k/8/8/3p4/8/8/3P4/K7 b - - 0 1", 6, 30749},
        {"k7/8/8/7p/6P1/8/8/K7 b - - 0 1", 6, 41874},
        {"k7/8/7p/8/8/6P1/8/K7 b - - 0 1", 6, 29679},
        {"k7/8/8/6p1/7P/8/8/K7 b - - 0 1", 6, 41874},
        {"k7/8/6p1/8/8/7P/8/K7 b - - 0 1", 6, 29679},
        {"k7/8/8/3p4/4p3/8/8/7K b - - 0 1", 6, 22579},
        {"k7/8/3p4/8/8/4P3/8/7K b - - 0 1", 6, 28662},
        {"7k/8/8/p7/1P6/8/8/7K w - - 0 1", 6, 41874},
        {"7k/8/p7/8/8/1P6/8/7K w - - 0 1", 6, 29679},
        {"7k/8/8/1p6/P7/8/8/7K w - - 0 1", 6, 41874},
        {"7k/8/1p6/8/8/P7/8/7K w - - 0 1", 6, 29679},
        {"k7/7p/8/8/8/8/6P1/K7 w - - 0 1", 6, 55338},
        {"k7/6p1/8/8/8/8/7P/K7 w - - 0 1", 6, 55338},
        {"3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1", 6, 199002},
        {"7k/8/8/p7/1P6/8/8/7K b - - 0 1", 6, 41874},
        {"7k/8/p7/8/8/1P6/8/7K b - - 0 1", 6, 29679},
        {"7k/8/8/1p6/P7/8/8/7K b - - 0 1", 6, 41874},
        {"7k/8/1p6/8/8/P7/8/7K b - - 0 1", 6, 29679},
        {"k7/7p/8/8/8/8/6P1/K7 b - - 0 1", 6, 55338},
        {"k7/6p1/8/8/8/8/7P/K7 b - - 0 1", 6, 55338},
        {"3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1", 6, 199002},
        {"8/Pk6/8/8/8/8/6Kp/8 w - - 0 1", 6, 1030499},
        {"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1", 6, 37665329},
        {"8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1", 6, 28859283},
        {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1", 6, 71179139},
        {"8/Pk6/8/8/8/8/6Kp/8 b - - 0 1", 6, 1030499},
        {"n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1", 6, 37665329},
        {"8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1", 6, 28859283},
        {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 6, 71179139}};

    for (const TestCase &test : test_cases)
    {
        std::cout << "Testing FEN: " << test.fen << "\nDepth: " << test.depth << std::endl;

        Pos pos(test.fen);

        pos.print_board();

        Timestamp start = get_current_ms();

        int result = perft(pos, test.depth, true);

        print_time_diff(start);

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