#include "common/State.hpp"
#include "common/Utils.hpp"

using namespace std;

namespace wordle {
void State::update(const string &query,
                   const vector<State::LetterState> &result) {
  queries.push_back(query);
  results.push_back(result);

  for (unsigned i = 0; i < WORD_LENGTH; ++i) {
    char currentLetter = query[i];
    State::LetterState &updateState = letterStates[currentLetter - MIN_CHAR];

    // Update states for each alphabet letter
    if (updateState == State::LetterState::UNKNOWN ||
        (updateState == State::LetterState::INCLUDES &&
         result[i] == State::LetterState::EXACT_MATCH)) {
      updateState = result[i];
    }
  }
  ++numAsked;
}

void State::undo() {
  if (numAsked > 0) {
    queries.pop_back();
    results.pop_back();
    --numAsked;

    // Since `undo' is not a common operation, the letterStates are computed in
    // a naive way
    letterStates.assign(NUM_CHAR, LetterState::UNKNOWN);
    for (unsigned idx = 0; idx < numAsked; ++idx) {
      auto &query = queries[idx];
      auto &result = results[idx];
      for (unsigned i = 0; i < WORD_LENGTH; ++i) {
        char currentLetter = query[i];
        State::LetterState &updateState =
            letterStates[currentLetter - MIN_CHAR];

        if (updateState == State::LetterState::UNKNOWN ||
            (updateState == State::LetterState::INCLUDES &&
             result[i] == State::LetterState::EXACT_MATCH)) {
          updateState = result[i];
        }
      }
    }
  }
}

void State::print(ostream &os) {
  // Queries and results
  for (unsigned i = 0; i < numAsked; ++i) {
    os << "(" << (i + 1) << ") ";
    for (unsigned j = 0; j < WORD_LENGTH; ++j) {
      printLetterOfStatus(os, queries[i][j], results[i][j]);
    }
    os << "\n";
  }
  for (unsigned i = numAsked; i < MAX_QUERIES; ++i) {
    os << "(" << (i + 1) << ") ";
    for (unsigned j = 0; j < WORD_LENGTH; ++j)
      os << BLANK_CHAR;
    os << "\n";
  }
  printHorizontalSplit(os);

  // Letter states
  for (char letter = MIN_CHAR; letter <= MAX_CHAR; ++letter) {
    printLetterOfStatus(os, letter, letterStates[letter - MIN_CHAR]);
  }
  os << "\n";
  printHorizontalSplit(os);
}
}; // namespace wordle