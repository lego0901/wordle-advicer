// Game states storage class

#pragma once

#include "common/Config.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace wordle {
class State {
public:
  enum LetterState {
    EXACT_MATCH = 0, // Same letter and right position
    INCLUDES = 1,    // Same letter but in a different position
    EXCLUDES = 2,    // No occurrence at all
    UNKNOWN = 3,
  };

  // The number of questions
  unsigned numAsked;

  // List of words that user has asked
  std::vector<const std::string> queries;

  // Responses from the user's queries
  std::vector<std::vector<LetterState>> results;

  // States of each letter from 'a' to 'z'
  std::vector<LetterState> letterStates;

  State() { initialize(); }

  void initialize() {
    numAsked = 0;
    queries.clear();
    results.clear();
    letterStates.assign(NUM_CHAR, LetterState::UNKNOWN);
  }

  // Update a new query and its result
  void update(const std::string &query, const std::vector<LetterState> &result);

  // Undo the latest query if it is not empty
  void undo();

  // (For CUI) Print the current state
  void print(std::ostream &os = std::cout);

private:
};

const std::map<State::LetterState, ColorCode> STATE_COLOR_CODE_MAP = {
    {State::LetterState::EXACT_MATCH, ColorCode::GREEN},
    {State::LetterState::INCLUDES, ColorCode::YELLOW},
    {State::LetterState::EXCLUDES, ColorCode::DEFAULT},
    {State::LetterState::UNKNOWN, ColorCode::BLUE},
};
}; // namespace wordle