#include <iostream>
#include <chrono>
#include <conio.h>

#include "CompositionTree.h"

int main()
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    WordReader wr("Resources/words_alpha.txt");
    std::vector<BitWord> bitWords;
    wr.ReadValidWords(bitWords, 5);

    CompositionTree compTree("Resources/solution.txt");
    compTree.GenerateCombinations(bitWords, 5);
    //compTree.GenerateCombinations_singleThread(bitWords, 5);
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "\nRuntime = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
}
