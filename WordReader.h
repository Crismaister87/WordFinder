#pragma once
#include <fstream>
#include <vector>

typedef unsigned int bitData;

struct BitWord
{
	BitWord(std::string word, bitData bit) : originalWord(word), bitReprezentation(bit) {};

	std::string originalWord;
	bitData bitReprezentation;
};

class WordReader
{
private:
	std::ifstream fReader;

	bitData ValidateString(const std::string& word) const;

public:
	WordReader(const std::string& path) : fReader(path) {};
	~WordReader() { fReader.close(); }

	void ReadValidWords(std::vector<BitWord>& bitWords, short allowedWordLength);
};