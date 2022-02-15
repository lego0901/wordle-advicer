// Additional states that are used for suggestions

#pragma once

#include "common/Dictionary.hpp"
#include "common/State.hpp"

#include <string>
#include <vector>

namespace wordle {
class ComputationalState {
public:
  Dictionary &dict;
  State &state;

  // (position, matched letter) pairs
  std::vector<std::pair<unsigned, char>> exactMatches;

  // 26-sized integer array. For each letter, the occurrence of it on any
  // possible answers is >= geq and <= leq
  std::vector<unsigned> geq;
  std::vector<unsigned> leq;

  // For each position, one is sure that these letters are not there
  std::vector<std::vector<char>> includesButNotThere;

  // Possibility to be an answer for each word in the dictionary
  std::vector<bool> possibleWordMasks;

  ComputationalState(Dictionary &dict, State &state)
      : dict(dict), state(state) {
    initialize();
  }

  void initialize() {
    exactMatches.clear();
    geq.assign(NUM_CHAR, 0);
    leq.assign(NUM_CHAR, WORD_LENGTH);
    includesButNotThere.assign(WORD_LENGTH, std::vector<char>());
    possibleWordMasks.assign(dict.numWords, true);
  }

  // Check if the given word can be an answer
  bool isPossibleWord(const std::string &word);

  // Update a new query and its result
  void update(const std::string &query,
              const std::vector<State::LetterState> &result);

  // Copy data from another instance
  void copyDataFrom(const ComputationalState &otherState);

  // Fetch a list of possible answers with probabilities sum to 1
  std::vector<std::pair<double, std::string>> possibleWordsWithProbs();

private:
};
}; // namespace wordle