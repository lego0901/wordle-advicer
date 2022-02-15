#include "common/Utils.hpp"

#include <algorithm>
#include <cmath>

using namespace std;

namespace wordle {
inline string colorForStdout(ColorCode code) {
  return "\033[" + std::to_string(code) + "m";
}

void computeLetterState(const string &query, const string &candidateAnswer,
                        vector<State::LetterState> &result) {
  result.assign(WORD_LENGTH, State::LetterState::UNKNOWN);

  // Make count of each letter on the candidateAnswer
  int count[NUM_CHAR] = {};
  for (char letter : candidateAnswer)
    ++count[letter - MIN_CHAR];

  for (unsigned i = 0; i < WORD_LENGTH; ++i) {
    char currentLetter = query[i];
    // Clear EXACT_MATCH case
    if (currentLetter == candidateAnswer[i]) {
      result[i] = State::LetterState::EXACT_MATCH;
      --count[currentLetter - MIN_CHAR];
      continue;
    }
    // Clear EXCLUDES case
    if (find(candidateAnswer.begin(), candidateAnswer.end(), currentLetter) ==
        candidateAnswer.end())
      result[i] = State::LetterState::EXCLUDES;
  }

  for (unsigned i = 0; i < WORD_LENGTH; ++i) {
    char currentLetter = query[i];
    if (result[i] == State::LetterState::UNKNOWN) {
      // If the number of currentLetter in the candidateAnswer is >=.
      if (--count[currentLetter - MIN_CHAR] >= 0)
        result[i] = State::LetterState::INCLUDES;
      else
        result[i] = State::LetterState::EXCLUDES;
    }
  }
}

bool isValidWord(const string &query) {
  if (query.size() != WORD_LENGTH)
    return false;
  for (char letter : query) {
    if (!(letter >= MIN_CHAR && letter <= MAX_CHAR))
      return false;
  }
  return true;
}

unsigned encodeLetterState(const vector<State::LetterState> &state) {
  // From LetterState vector to unsigned integer (for faster random access)
  unsigned result = 0;
  for (unsigned i = 0; i < state.size(); ++i) {
    result = result * 3 + state[i];
  }
  return result;
}

unsigned numLetterStates() {
  unsigned result = 1;
  for (unsigned i = 0; i < WORD_LENGTH; ++i)
    result *= 3;
  return result;
}

double getEntropy(vector<pair<double, string>> &wordsWithProbs) {
  double entropy = 0.0;
  for (auto &[prob, _] : wordsWithProbs) {
    if (prob < 1e-7)
      continue;
    entropy += -prob * log2(prob);
  }
  return entropy;
}

void printWithColor(ostream &os, char letter, ColorCode code) {
  os << colorForStdout(code) << letter << colorForStdout(ColorCode::DEFAULT);
}

void printWithColor(ostream &os, const string &text, ColorCode code) {
  os << colorForStdout(code) << text << colorForStdout(ColorCode::DEFAULT);
}

void printLetterOfStatus(ostream &os, char letter, State::LetterState result) {
  printWithColor(os, letter, STATE_COLOR_CODE_MAP.find(result)->second);
}

void printHorizontalSplit(std::ostream &os) {
  os << "----------------------------------------\n";
}

void printBanner(std::ostream &os, bool adviceMode) {
  printHorizontalSplit(os);
  os << "Welcome to ";
  printWithColor(os, "WO", ColorCode::BLUE);
  printWithColor(os, "RD", ColorCode::GREEN);
  printWithColor(os, "LE", ColorCode::YELLOW);
  os << "!\n";

  if (adviceMode) {
    printWithColor(os, "0 for excludes  ", ColorCode::DEFAULT);
    printWithColor(os, "1 for includes  ", ColorCode::YELLOW);
    printWithColor(os, "2 for exact matches", ColorCode::GREEN);
    os << "\n";
  }

  if (adviceMode)
    os << "** Special commands: \"done\", \"undo\"\n";
  else
    os << "** Special commands: \"give up\", \"undo\"\n";
  printHorizontalSplit(os);
}

bool checkIfYesInput(istream &is, ostream &os, string description) {
  if (description != "")
    os << description << " ";
  os << "(type 'y' for yes)\n"
     << ">> ";

  string input;
  getline(is, input);
  return input == "y";
}

bool checkIfNoInput(istream &is, ostream &os, string description) {
  if (description != "")
    os << description << " ";
  os << "(type 'n' for no)\n"
     << ">> ";

  string input;
  getline(is, input);
  return input == "n";
}
}; // namespace wordle