#include "brain/Guess.hpp"
#include "common/Utils.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <queue>

using namespace std;

namespace wordle {
vector<pair<double, string>> Guess::topKGuesses(unsigned k) {
  if (compState.state.numAsked == 0) {
    // Obvious but time-spending case
    return FIRST_GUESS;
  }

  vector<pair<double, string>> wordsWithProbs =
      compState.possibleWordsWithProbs();

  double stateEntropy = getEntropy(wordsWithProbs);

  priority_queue<pair<double, string>> reasonableGuesses;
  unsigned readIndex = 0; // Index on wordsWithProbs
  double maxScore = 0;    // To eliminate redundant guesses

  for (unsigned i = 0; i < compState.dict.numWords; ++i) {
    const string &guess = compState.dict.words[i];
    double probOfThisGuess = 0;
    if (compState.possibleWordMasks[i]) {
      // The guess is one of the candidate answers
      assert(wordsWithProbs[readIndex].second == guess);
      probOfThisGuess = wordsWithProbs[readIndex].first;
      ++readIndex;
    }

    // Estimates the number of guesses when selecting guess word as next
    double score = expectedNumOfGuessesAfterAsking(
        guess, stateEntropy, wordsWithProbs, probOfThisGuess);

    maxScore = max(maxScore, score);

    // Conserve top-k elements in terms of the low expected numbers of guesses
    reasonableGuesses.emplace(score, guess);
    if (reasonableGuesses.size() > k)
      reasonableGuesses.pop();
  }

  // Fetch it into a vector
  vector<pair<double, string>> guesses;
  while (!reasonableGuesses.empty()) {
    guesses.push_back(reasonableGuesses.top());
    reasonableGuesses.pop();
  }

  // Small expected number should go first
  reverse(guesses.begin(), guesses.end());

  // Remove all redundant guesses on the tail
  while (!guesses.empty() && guesses.back().first == maxScore)
    guesses.pop_back();

  return guesses;
}

double Guess::expectedNumOfGuessesAfterAsking(
    const string query, double stateEntropy,
    const vector<pair<double, string>> &wordsWithProbs,
    double probOfThisGuess) {
  vector<double> stateProbs(numLetterStates(), 0.0);
  vector<State::LetterState> state;

  // Make probabilities for each WORD_LENGTH letter states
  for (auto &[prob, candidateAnswer] : wordsWithProbs) {
    computeLetterState(query, candidateAnswer, state);
    unsigned encodedState = encodeLetterState(state);
    stateProbs[encodedState] += prob;
  }

  // Entropy decrease when selecting this query and the next guess
  double actionEntropy = 0.0;
  for (double prob : stateProbs) {
    if (prob < 1e-7)
      continue;
    actionEntropy += -prob * log2(prob);
  }

  // Calculate the expected number of guesses
  double nextScore = compState.state.numAsked + 1;
  double score =
      probOfThisGuess * nextScore +
      (1 - probOfThisGuess) *
          (nextScore + estimateRemainingSteps(stateEntropy - actionEntropy));
  return score;
}

void Guess::update(const std::string &query,
                   const std::vector<State::LetterState> &result) {
  history.push_back(compState);
  compState.update(query, result);
}

void Guess::undo() {
  if (!history.empty()) {
    compState.copyDataFrom(history.back());
    history.pop_back();
  }
}

void Guess::print(ostream &os) {
  os << "Computing...\n";
  auto guessResult = topKGuesses(MAX_GUESSES);
  for (auto &[prob, word] : guessResult) {
    os << setprecision(3) << prob << ' ' << word << "  ";
  }
  os << "\n";
}

double Guess::estimateRemainingSteps(double remainingEntropy) {
  // An experimental value
  // y = -0.0095x2 + 0.305x + 1.1094
  double x = max<double>(0, remainingEntropy);
  return -0.0095 * x * x + 0.305 * x + 0.1094;
}
}; // namespace wordle
