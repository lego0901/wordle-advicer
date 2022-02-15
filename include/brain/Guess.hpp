// Class for the automatic guess

#pragma once

#include "brain/ComputationalState.hpp"
#include "common/Dictionary.hpp"
#include "common/State.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace wordle {
class Guess {
public:
  ComputationalState compState;

  // Trace all compStates (for undo)
  std::vector<ComputationalState> history;

  Guess(Dictionary &dict, State &state)
      : compState(ComputationalState(dict, state)) {
    initialize();
  }

  void initialize() {
    compState.initialize();
    history.clear();
  }

  // Get top-k best words in terms of minimum expected number of guesses
  std::vector<std::pair<double, std::string>> topKGuesses(unsigned k);

  // Expected number of guesses when asking `query' as the next word
  double expectedNumOfGuessesAfterAsking(
      const std::string query, double stateEntropy,
      const std::vector<std::pair<double, std::string>> &wordsWithProbs,
      double probOfThisGuess);

  // Update a new query and its result
  void update(const std::string &query,
              const std::vector<State::LetterState> &result);

  // Undo the latest query if it is not empty
  void undo();

  // (For CUI) Print the current state
  void print(std::ostream &os);

private:
  // The first opening guess. Since it takes much time on this computation, we
  // suggest you to use this precomputed value
  const std::vector<std::pair<double, std::string>> FIRST_GUESS = {
      {3.64, "tares"}, {3.66, "rates"}, {3.66, "tales"}, {3.67, "tears"},
      {3.68, "tires"}, {3.68, "nares"}, {3.68, "reals"}, {3.68, "tries"}};

  // Estimate the number of remaining steps from remainingEntropy
  double estimateRemainingSteps(double remainingEntropy);
};
}; // namespace wordle