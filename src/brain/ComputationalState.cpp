#include "brain/ComputationalState.hpp"

#include <algorithm>

using namespace std;

namespace wordle {
bool ComputationalState::isPossibleWord(const string &word) {
  // Make count of each letter on the word
  unsigned counts[NUM_CHAR] = {};
  for (char letter : word)
    ++counts[letter - MIN_CHAR];

  // EXACT_MATCH cases should be exactly matched
  for (auto &[position, letter] : exactMatches) {
    if (word[position] != letter)
      return false;
  }

  // Check geq, leq for each letter's count
  for (char letter = MIN_CHAR; letter <= MAX_CHAR; ++letter) {
    unsigned index = letter - MIN_CHAR;
    if (!(counts[index] >= geq[index] && counts[index] <= leq[index]))
      return false;
  }

  // Includes but not that position
  for (unsigned i = 0; i < WORD_LENGTH; ++i) {
    for (char letter : includesButNotThere[i]) {
      if (word[i] == letter)
        return false;
    }
  }
  return true;
}

void ComputationalState::update(const string &query,
                                const vector<State::LetterState> &result) {
  vector<pair<State::LetterState, unsigned>> statesAndPositions;
  for (unsigned i = 0; i < WORD_LENGTH; ++i)
    statesAndPositions.emplace_back(result[i], i);
  // Sort states EXACT_MATCH -> INCLUDES -> EXCLUDES
  sort(statesAndPositions.begin(), statesAndPositions.end());

  // Update geq and leq
  unsigned counts[NUM_CHAR] = {};
  for (auto &[state, i] : statesAndPositions) {
    unsigned currentIndex = query[i] - MIN_CHAR;
    ++counts[currentIndex];

    switch (state) {
    case State::LetterState::EXACT_MATCH:
    case State::LetterState::INCLUDES: {
      // Minimum count requirement for the current letter
      geq[currentIndex] = max(geq[currentIndex], counts[currentIndex]);
      break;
    }
    case State::LetterState::EXCLUDES: {
      // Maximum count constraint of the current letter
      leq[currentIndex] = min(leq[currentIndex], counts[currentIndex] - 1);
      break;
    }
    default:
      break;
    }
  }

  // Update exactMatches and includesButNotThere
  for (unsigned i = 0; i < WORD_LENGTH; ++i) {
    char currentLetter = query[i];
    State::LetterState currentState = result[i];

    switch (currentState) {
    case State::LetterState::EXACT_MATCH: {
      bool hasExactMatch = false; // Check if it knows already
      for (auto &[position, letter] : exactMatches) {
        if (position == i && letter == currentLetter) {
          hasExactMatch = true;
          break;
        }
      }
      if (!hasExactMatch)
        exactMatches.emplace_back(i, currentLetter);
      break;
    }
    case State::LetterState::INCLUDES: {
      vector<char> &there = includesButNotThere[i];
      if (find(there.begin(), there.end(), currentLetter) == there.end()) {
        there.push_back(currentLetter);
      }
      break;
    }
    case State::LetterState::EXCLUDES:
      break;
    default:
      break;
    }
  }

  // Update possibleWordMasks
  for (unsigned i = 0; i < dict.numWords; ++i) {
    if (possibleWordMasks[i])
      possibleWordMasks[i] = isPossibleWord(dict.words[i]);
  }
}

void ComputationalState::copyDataFrom(const ComputationalState &otherState) {
  exactMatches = otherState.exactMatches;
  geq = otherState.geq;
  leq = otherState.leq;

  includesButNotThere = otherState.includesButNotThere;
  possibleWordMasks = otherState.possibleWordMasks;
}

vector<pair<double, string>> ComputationalState::possibleWordsWithProbs() {
  vector<pair<double, string>> result;
  double aggregatedProbs = 0;
  for (unsigned i = 0; i < dict.numWords; ++i) {
    if (possibleWordMasks[i]) {
      result.emplace_back(dict.probabilities[i], dict.words[i]);
      aggregatedProbs += dict.probabilities[i];
    }
  }
  // Sum to 1
  for (auto &item : result)
    item.first /= aggregatedProbs;
  return result;
}
}; // namespace wordle