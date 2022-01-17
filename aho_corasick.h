#pragma once

#include <unordered_set>
#include <string>

namespace AhoCorasick {

/**
 * Algorithm for finding all words of a given dictionary that appear in an input text
 * O(n) time complexity for the search, with an additional factor for building
 * the Trie based automaton. In this implementation (for simplicity), we are assuming 
 * an alphabet from 'a' to 'z'
 */
std::unordered_set<std::string> Search(
    const std::vector<std::string>& words_to_look_for,
    const std::string& text_to_search
);

}
