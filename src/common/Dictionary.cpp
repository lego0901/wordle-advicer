#include "common/Dictionary.hpp"
#include "common/Config.hpp"

#include <cassert>
#include <cmath>
#include <fstream>

using namespace std;

namespace wordle {
Dictionary::Dictionary(const string &filepath) {
  ifstream reader(filepath);
  assert(reader.is_open() && "Cannot open the file");

  // Read words from the file, which are sorted in their frequencies
  string word;
  while (getline(reader, word)) {
    words.push_back(word);
    wordSet.insert(word);
  }
  reader.close();
  numWords = words.size();

  // Sigmoid distribution
  probabilities.resize(numWords);
  double sumProb = 0;
  for (unsigned i = 0; i < numWords; ++i) {
    double x = ((double)i / numWords);
    probabilities[i] =
        1.0 / (1.0 + exp(SIGMOID_COEFFICIENT * (x - SIGMOID_OFFSET)));
    sumProb += probabilities[i];
  }

  // Sum up to 1
  for (unsigned i = 0; i < numWords; ++i) {
    probabilities[i] = probabilities[i] / sumProb;
  }
}

string Dictionary::pickOneFrom(double uniformRandom) {
  assert(0 <= uniformRandom && uniformRandom <= 1);
  double aggregatedProbs = 0;
  for (unsigned i = 0; i < numWords; ++i) {
    aggregatedProbs += probabilities[i];
    if (uniformRandom < aggregatedProbs)
      return words[i];
  }
  return words.back();
}

bool Dictionary::isWordInDictionary(const string &query) const {
  return wordSet.find(query) != wordSet.end();
}
}; // namespace wordle