// Word dictionary management class

#pragma once

#include <set>
#include <string>
#include <vector>

namespace wordle {
class Dictionary {
public:
  unsigned numWords;
  std::vector<std::string> words;
  std::set<std::string> wordSet;

  // Probability distributions of each word, sum to 1
  std::vector<double> probabilities;

  Dictionary(const std::string &filepath);

  // Pick a random word from dictionary by an uniformly generated random
  // value in [0, 1]
  std::string pickOneFrom(double uniformRandom);

  // Check if the given query word is in the dictionary
  bool isWordInDictionary(const std::string &query) const;

private:
  // Constants to define the words' probability distribution.
  // Let x be a word's rank for the frequency and T be the total number of words
  // in the dictionary. Then, its probability distribution to be an answer is:
  //   P(word) = C * 1 / (1 + exp(COEFFICIENT * (x / T - OFFSET)))
  // where C is a constant to make the sum 1.
  static constexpr double SIGMOID_OFFSET = 0.2;
  static constexpr double SIGMOID_COEFFICIENT = 1;
};
}; // namespace wordle