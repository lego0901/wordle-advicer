#include "play/Advice.hpp"
#include "common/Utils.hpp"

using namespace std;

namespace wordle {
void Advice::print(ostream &os) {
  printBanner(os, /*adviceMode=*/true);
  state.print(os);
  guess->print(os);

  if (!isFinish()) {
    os << ">> ";
  }
}

bool Advice::parseResult(const string &input, string &query,
                         vector<State::LetterState> &result) {
  query.clear();
  result.clear();

  State::LetterState currentState = State::LetterState::EXCLUDES;

  for (unsigned i = 0; i < input.size(); ++i) {
    char current = input[i];

    if (MIN_CHAR <= current && current <= MAX_CHAR) {
      if (query.size() == WORD_LENGTH)
        return false;
      query.push_back(current);
      result.push_back(currentState);
    } else if ('0' <= current && current <= '2') {
      // '0' for EXCLUDES, '1' for INCLUDES, '2' for EXACT_MATCH
      currentState = static_cast<State::LetterState>('2' - current);
    } else {
      return false;
    }
  }

  if (query.size() != WORD_LENGTH)
    return false;

  return true;
}

bool Advice::proceed(const string &input, ostream *os) {
  // Special keywords "done" and "undo"
  if (input == "done") {
    finish = true;
    return true;
  }
  if (input == "undo") {
    undo();
    return true;
  }

  string query;
  vector<State::LetterState> result;

  bool verbose = (os != nullptr);
  if (!parseResult(input, query, result)) {
    if (verbose)
      *os << "Invalid input!\n>> ";
    return false;
  }

  update(query, result);
  return true;

  if (state.numAsked == MAX_QUERIES)
    finish = true;
  return true;
}
}; // namespace wordle

int main() {
  wordle::Dictionary dict(wordle::DICTIONARY_PATH);
  wordle::State state;

  wordle::Advice advice(dict, state);

  while (1) {
    while (!advice.isFinish()) {
      advice.print(cout);
      bool done = false;

      do {
        string input;
        getline(cin, input);
        done = advice.proceed(input, &cout);
        // Do it again for an invalid input
      } while (!done);
    }

    // Game end
    advice.print(cout);

    if (wordle::checkIfNoInput(cin, cout, "Do you want to play again?"))
      break;
    else
      advice.initialize();
  }

  return 0;
}