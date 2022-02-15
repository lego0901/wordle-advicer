// Advice mode while playing another game instance

#pragma once

#include "brain/Guess.hpp"
#include "common/Dictionary.hpp"
#include "common/State.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace wordle {
class Advice {
public:
  Dictionary &dict;
  State &state;
  std::unique_ptr<Guess> guess = nullptr;

  bool finish;

  Advice(Dictionary &dict, State &state)
      : dict(dict), state(state), guess(nullptr) {
    initialize();
  }

  void initialize() {
    state.initialize();
    if (guess == nullptr)
      guess = std::make_unique<Guess>(dict, state);
    guess->initialize();
    finish = false;
  }

  // Update a new query and its result
  void update(const std::string &query,
              const std::vector<State::LetterState> &result) {
    state.update(query, result);
    guess->update(query, result);
  }

  // Undo the latest query if it is not empty
  void undo() {
    state.undo();
    guess->undo();
  }

  // (For CUI) Display on an output stream
  void print(std::ostream &os = std::cout);

  // Process one input if it is valid and print (for CUI) to an ostream.
  // If the input is valid (according to the parseResult function), it will
  // proceed one step and give you another great suggestion and returns true.
  bool proceed(const std::string &input, std::ostream *os = nullptr);

  bool isFinish() { return finish; }

private:
  // Parse a plaintext input to letter-only query and state result.
  // Returns if the parsing has finished successfully; meaning that
  // (1) The input only contains small-case alphabets ('a'-'z') or '0', '1', '2'
  // (2) The number of alphabet letters should be equal to WORD_LENGTH
  //
  // The letters that came after '0' (default) are EXCLUDES, '1' are INCLUDES,
  // and '2' are EXACT_MATCH. (See the state defs on include/common/State.hpp)
  // For example, the input "wo1r2d0s" is valid and the parsed query string is
  // "words", and the result state is {EXCLUDES, EXCLUDES, INCLUDES,
  // EXACT_MATCH, EXCLUDES}.
  bool parseResult(const std::string &input, std::string &query,
                   std::vector<State::LetterState> &result);
};
}; // namespace wordle