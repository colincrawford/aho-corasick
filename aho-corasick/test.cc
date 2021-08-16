#include <vector>
#include <iostream>
#include <unordered_set>

#include "aho_corasick.h"


int test_case(std::vector<std::string>, std::string, std::vector<std::string>);

int main()
{
    std::vector<int> results {
        test_case({ "cat", "bat", "tiny", "phenom" }, "catatonic", { "cat" }),
        test_case({ "acc", "atc", "cat", "gcg" }, "gcatcg", { "atc", "cat" }),
        test_case({ "amster", "bambino", "cabam", "merger" }, "cabamerger", { "cabam", "merger" })
    };

    for (const int result : results)
    {
        if (result == 1)
        {
            return 1;
        }
    }

    return 0;
}
int test_case(
    std::vector<std::string> dictionary,
    std::string input_text,
    std::vector<std::string> expected_words_found
) {
    std::cout << std::endl;
    std::cout << "Searching: " << input_text << std::endl;
    std::cout << "For: ";
    for (const std::string& word : dictionary)
    {
        std::cout << word << " ";
    }
    std::cout << std::endl;

    std::unordered_set<std::string> found = AhoCorasick::Search(dictionary, input_text);
    if (found.size() != expected_words_found.size())
    {
        std::cout << "Didn't find the correct number of words" << std::endl;
        std::cout << "FAIL" << std::endl;
        return 1;
    }

    std::unordered_set<std::string> found_words(found.begin(), found.end());
    for (const std::string& word : expected_words_found)
    {
        if (found_words.find(word) == found_words.end())
        {
            std::cout << "You didn't find the word \"" << word << "\"" << std::endl;
            std::cout << "FAIL" << std::endl;
            return 1;
        }
    }

    std::cout << "PASS" << std::endl;
    return 0;
}
