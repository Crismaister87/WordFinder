#include "CompositionTree.h"

#include <iostream>
#include <sstream>
#include <thread>

void TreeNode::Insert(const BitWord& insertedBitWord, unsigned short targetDepth, unsigned short curDepth,
    std::vector<std::string>& parentWords, std::vector<std::string>& wordCombinations)
{
    parentWords[curDepth] = this->word.originalWord;
    curDepth++;
    if (curDepth == targetDepth - 1)
    {
        std::stringstream ss;
        for (int i = 0; i < curDepth; i++)
            ss << parentWords[i] << "\t";
        ss << insertedBitWord.originalWord << "\n";

        wordCombinations.push_back(ss.str());
    }
    else {
        for (TreeNode& node : nodes)
            if ((node.word.bitReprezentation | insertedBitWord.bitReprezentation)
                == (node.word.bitReprezentation ^ insertedBitWord.bitReprezentation))
                node.Insert(insertedBitWord, targetDepth, curDepth, parentWords, wordCombinations);
        nodes.push_back(TreeNode(insertedBitWord));
    }
}

void CompositionTree::GenerateCombinations_singleThread(const std::vector<BitWord>& bitWords, unsigned short numWordsRequired)
{
    Compose(bitWords, numWordsRequired, 0, bitWords.size());
}

void CompositionTree::GenerateCombinations(const std::vector<BitWord>& bitWords, unsigned short numWordsRequired)
{
    unsigned int processorCount = std::thread::hardware_concurrency();
    unsigned int numThreads = bitWords.size() < processorCount ? bitWords.size() : processorCount;

    long totalWork = static_cast<long>(bitWords.size()) * (static_cast<long>(bitWords.size()) + 1) / 2;
    long threadTargetWork = totalWork / numThreads;

    std::vector<std::thread> composerThreads;
    int threadStartIndex = 0;
    for (unsigned int i = 0; i < numThreads; ++i) {
        long threadCumulativeWork = 0;
        int threadEndIndex = threadStartIndex;

        while (threadEndIndex < bitWords.size() && threadCumulativeWork < threadTargetWork) {
            threadCumulativeWork += (bitWords.size() - 0);          // how many elements are processed per curent index
            threadEndIndex++;
        }

        if (i == numThreads - 1)
            threadEndIndex = bitWords.size();

        composerThreads.emplace_back(&CompositionTree::Compose, this, std::ref(bitWords), numWordsRequired, threadStartIndex, threadEndIndex);
        threadStartIndex = threadEndIndex;
    }

    for (std::thread& composerThread : composerThreads)
        composerThread.join();
}

void CompositionTree::Compose(const std::vector<BitWord>& bitWords, unsigned short numWordsRequired, unsigned int startingIndex, unsigned int endIndex)
{
    std::vector<std::string> parentWords;
    parentWords.resize(numWordsRequired - 1);
    std::vector<std::string> wordCombinations;

    for (unsigned int i = startingIndex; i < endIndex; ++i)
    {
        TreeNode root(bitWords[i]);
        for (int j = i + 1; j < bitWords.size(); ++j)
        {
            if ((root.word.bitReprezentation | bitWords[j].bitReprezentation)
                == (root.word.bitReprezentation ^ bitWords[j].bitReprezentation))
                root.Insert(bitWords[j], numWordsRequired, 0, parentWords, wordCombinations);
        }
    }   

    writerMutex.lock();
    std::cout << "*";
    for (const std::string& s : wordCombinations)
        fWriter << s;
    writerMutex.unlock();
}