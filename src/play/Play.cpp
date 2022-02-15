#include "play/Play.hpp"
#include "common/Utils.hpp"

using namespace std;

namespace wordle {
void Play::print(ostream &os) {
  printBanner(os, /*adviceMode=*/false);
  state.print(os);
  if (useGuess)
    guess->print(os);

  if (!isFinish()) {
    os << ">> ";
  } else {
    os << "The answer is \"";
    printWithColor(os, answer, ColorCode::GREEN);
    os << "\"\n";

    if (isWin())
      os << "You win!\n";
    else
      os << "You lose..\n";
  }
}

bool Play::proceed(const string &query, ostream *os) {
  // Special keywords "give up" and "undo"
  if (query == "give up") {
    finish = true;
    return true;
  }
  if (query == "undo") {
    undo();
    return true;
  }

  bool verbose = (os != nullptr);

  // Error cases
  if (!isValidWord(query)) {
    if (verbose)
      *os << "Invalid word!\n>> ";
    return false;
  }
  if (!dict.isWordInDictionary(query)) {
    if (verbose)
      *os << "Not in the word list!\n>> ";
    return false;
  }

  vector<State::LetterState> result;
  computeLetterState(query, answer, result);
  update(query, result);

  if (query == answer)
    win = true, finish = true;
  if (state.numAsked == MAX_QUERIES)
    finish = true;

  return true;
}
}; // namespace wordle

int main() {
  wordle::Dictionary dict(wordle::DICTIONARY_PATH);
  wordle::State state;
  bool useGuess =
      wordle::checkIfYesInput(cin, cout, "Do you want to use a great advicer?");

  wordle::Play play(dict, state, useGuess);

  while (1) {
    play.answer = "sates";
    while (!play.isFinish()) {
      play.print(cout);
      bool done = false;

      do {
        string query;
        getline(cin, query);
        done = play.proceed(query, &cout);
        // Do it again for an invalid query
      } while (!done);
    }

    // Game end
    play.print(cout);

    if (wordle::checkIfNoInput(cin, cout, "Do you want to play again?"))
      break;
    else
      play.initialize();
  }

  return 0;
}