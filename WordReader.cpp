#include  "WordReader.h"
#include <unordered_set>
#include <string>

bitData WordReader::ValidateString(const std::string& word) const
{
    bitData bitReprezentation = 0;
    unsigned short numWowels = 0;
    for (const char& c : word)
    {
        bitData newBit = 1;
        if (c > 64 && c < 91)
            newBit = newBit << (c - 65);
        else if (c > 96 && c < 123)
            newBit = newBit << (c - 97);
        else
            return 0;

        if ((bitReprezentation | newBit) == (bitReprezentation ^ newBit))
            bitReprezentation |= newBit;
        else
            return 0;
    }
    return bitReprezentation;
}

void WordReader::ReadValidWords(std::vector<BitWord>& bitWords, short allowedWordLength)
{
    std::string word;
    std::unordered_set<bitData> anagramChecker;

    while (getline(fReader, word)) {
        if (word.length() == allowedWordLength)
        {
            bitData bitReprezentation = ValidateString(word);
            if (bitReprezentation != 0 && anagramChecker.find(bitReprezentation) == anagramChecker.end())
            {
                anagramChecker.emplace(bitReprezentation);
                bitWords.push_back({ word, bitReprezentation });
            }
        }
    }
}