// Configurations about the game

#pragma once

#include <string>

namespace wordle {
// Basic word configurations
constexpr unsigned WORD_LENGTH = 5;
constexpr char MIN_CHAR = 'a';
constexpr char MAX_CHAR = 'z';
constexpr char NUM_CHAR = MAX_CHAR - MIN_CHAR + 1;
constexpr char BLANK_CHAR = '_';

// Maximum number of queries on a single game
constexpr unsigned MAX_QUERIES = 6;

// Maximum number of auto-generated suggestions
constexpr unsigned MAX_GUESSES = 8;

// Word dictionary path
const std::string DICTIONARY_PATH = "dictionary.txt";

// Color code literals for CUI
enum ColorCode { BLUE = 34, GREEN = 32, YELLOW = 33, DEFAULT = 39 };
}; // namespace wordle