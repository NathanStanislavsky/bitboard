#include <cassert>

#include "transposition.h"

// Constructor: Initializes the TT with a given size
TranspositionTable::TranspositionTable(size_t size) {
    // Ensure size is a power of two for efficient indexing
    assert((size & (size - 1)) == 0 && "Transposition Table size must be a power of two.");

    table.resize(size);
    mask = size - 1;
}

// Computes the index in the TT array from the hash key
size_t TranspositionTable::get_index(uint64_t hash) const {
    return hash & mask; // Equivalent to hash % size when size is a power of two
}

// Probes the TT for a given hash key
bool TranspositionTable::probe(uint64_t hash, int depth, int alpha, int beta, int& score, Move& best_move) const {
    size_t index = get_index(hash);
    const TTEntry& entry = table[index];

    // Check if the entry matches the current position and has sufficient depth
    if (entry.key != hash || entry.depth < depth) {
        return false; // Entry not usable
    }

    // Use the stored score based on the flag
    if (entry.flag == TT_EXACT) {
        score = entry.score;
        best_move = entry.best_move;
        return true;
    }
    else if (entry.flag == TT_UPPERBOUND && entry.score <= alpha) {
        score = entry.score;
        return true;
    }
    else if (entry.flag == TT_LOWERBOUND && entry.score >= beta) {
        score = entry.score;
        return true;
    }

    return false;
}

// Stores an entry in the TT
void TranspositionTable::store(uint64_t hash, int depth, int score, TTFlag flag, Move best_move) {
    size_t index = get_index(hash);
    TTEntry& entry = table[index];

    // Replacement scheme: Replace if the new entry has greater depth or the existing entry is empty
    if (entry.depth < depth || entry.key == 0) { // Assuming key 0 is invalid
        entry.key = hash;
        entry.depth = depth;
        entry.score = score;
        entry.flag = flag;
        entry.best_move = best_move;
    }
}

// Clears the TT (optional)
void TranspositionTable::clear() {
    std::fill(table.begin(), table.end(), TTEntry());
}