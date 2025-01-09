#pragma once

#include <cstdint>
#include <vector>

#include "move.h"
#include "zobrist.h"
#include "types.h"

enum TTFlag
{
    TT_EXACT,
    TT_UPPERBOUND,
    TT_LOWERBOUND
};

struct TTEntry
{
    uint64_t key;   // Zobrist hash key
    int depth;      // Search depth
    int score;      // Evaluation score
    TTFlag flag;    // Type of score
    Move best_move; // Best move from this position
};

class TranspositionTable
{
    public:
        // Constructor: Initializes the TT with specified size (16 million entries by default)
        TranspositionTable(size_t size = (1 << 24));

        // Probes the TT for a given hash key
        bool probe(uint64_t hash, int depth, int alpha, int beta, int &score, Move &best_move) const;

        // Stores an entry in the TT
        void store(uint64_t hash, int depth, int score, TTFlag flag, Move best_move);

        // Clears the TT
        void clear();

    private:
        // Computes the index in the TT array from the hash key
        size_t get_index(uint64_t hash) const;

        std::vector<TTEntry> table; // The TT storage
        size_t mask;                // Mask for indexing (TT size must be a power of two)
};