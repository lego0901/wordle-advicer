// Game play mode of Wordle, optionally with a guesser instance

#pragma once

#include "brain/Guess.hpp"
#include "common/Dictionary.hpp"
#include "common/State.hpp"

#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

namespace wordle {
class Play {
public:
  Dictionary &dict;
  State &state;
  std::unique_ptr<Guess> guess = nullptr;

  std::string answer;
  bool finish;
  bool win;

  bool useGuess;

  Play(Dictionary &dict, State &state, bool useGuess = false)
      : dict(dict), state(state), useGuess(useGuess) {
    engine = std::default_random_engine(device());
    distr = std::uniform_real_distribution<double>(0, 1);
    initialize();
  }

  void initialize() {
    state.initialize();
    answer = dict.pickOneFrom(distr(engine));
    finish = false;
    win = false;
    if (useGuess) {
      // If useGuess, create and guess instance which is a great advicer
      if (guess == nullptr)
        guess = std::make_unique<Guess>(dict, state);
      guess->initialize();
    }
  }

  // Update a new query and its result
  void update(const std::string &query,
              const std::vector<State::LetterState> &result) {
    state.update(query, result);
    if (useGuess)
      guess->update(query, result);
  }

  // Undo the latest query if it is not empty
  void undo() {
    state.undo();
    if (useGuess)
      guess->undo();
  }

  // (For CUI) Display on an output stream
  void print(std::ostream &os = std::cout);

  // Process one input if it is valid and print (for CUI) to an ostream.
  // If the query is valid, it will proceed one step and give you another great
  // suggestion and returns true.
  // A query is valid if both followings hold:
  // (1) It should only contain small-case alphabets, with length WORD_LENGTH
  // (2) It should be listed on the dictionary
  bool proceed(const std::string &query, std::ostream *os = nullptr);

  bool isFinish() { return finish; }
  bool isWin() { return win; }

private:
  // RNG for pick an answer word randomly
  std::random_device device;
  std::default_random_engine engine;
  std::uniform_real_distribution<double> distr;
};
}; // namespace wordle