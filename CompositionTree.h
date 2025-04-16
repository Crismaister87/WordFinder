#pragma once
#include "WordReader.h"
#include <mutex>

class TreeNode
{
public:
    std::vector<TreeNode> nodes;
    BitWord word;

    TreeNode(BitWord w) : word(w) {};
    ~TreeNode() {}

    void Insert(const BitWord& insertedBitWord, unsigned short targetDepth, unsigned short curDepth,
        std::vector<std::string>& combination, std::vector<std::string>& combinations);
};

class CompositionTree
{
private:
    std::ofstream fWriter;
    std::mutex writerMutex;

    void Compose(const std::vector<BitWord>& bitWords, unsigned short numWordsRequired, unsigned int startingIndex, unsigned int endIndex);

public:
    CompositionTree(const std::string& path) : fWriter(path) {}
    ~CompositionTree() { fWriter.close(); }

    void GenerateCombinations(const std::vector<BitWord>& bitWords, unsigned short numWordsRequired);
    void GenerateCombinations_singleThread(const std::vector<BitWord>& bitWords, unsigned short numWordsRequired);
};