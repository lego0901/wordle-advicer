// Utility module

#pragma once

#include "common/Config.hpp"
#include "common/State.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace wordle {
// Assuming that the answer is `candidateAnswer', make the result responding to
// the user's query
void computeLetterState(const std::string &query,
                        const std::string &candidateAnswer,
                        std::vector<State::LetterState> &result);

// Check if the query has a valid length and consists of valid letters
bool isValidWord(const std::string &query);

// Encode vector of letter states into an unsigned integer
unsigned encodeLetterState(const std::vector<State::LetterState> &state);
// Total number of the letter states
unsigned numLetterStates();

// Compute entropy from the probability distribution
double getEntropy(std::vector<std::pair<double, std::string>> &wordsWithProbs);

// (For CUI) Print text with a color
void printWithColor(std::ostream &os, char letter, ColorCode code);
void printWithColor(std::ostream &os, const std::string &text, ColorCode code);
// (For CUI) Print letter of the status
void printLetterOfStatus(std::ostream &os, char letter,
                         State::LetterState result);
// (For CUI) Print a horizontal split line
void printHorizontalSplit(std::ostream &os);
// (For CUI) Print banner for game
void printBanner(std::ostream &os, bool adviceMode = false);

// (For CUI) Ask and check if the user's response is "y"
bool checkIfYesInput(std::istream &is, std::ostream &os,
                     std::string description = "");
// (For CUI) Ask and check if the user's response is "n"
bool checkIfNoInput(std::istream &is, std::ostream &os,
                    std::string description = "");
}; // namespace wordle