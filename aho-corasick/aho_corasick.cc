#include "aho_corasick.h" 

#include <vector>

namespace AhoCorasick {

class TrieNode
{
public:
    TrieNode(): children_{}
    {}

    ~TrieNode()
    {
        for (const TrieNode* node : children_)
        {
            delete node;
        }
    }

    TrieNode* ChildNode(const char c) const
    {
        return children_[c - 'a'];
    }

    void InsertChildNode(const char c, TrieNode* node)
    {
        children_[c - 'a'] = node;
    }

    bool has_word() const
    {
        return word_.size() != 0;
    }

    void word(const std::string& word)
    {
        word_ = word;
    }

    std::string word() const
    {
        return word_;
    }

    TrieNode* failure_node() const
    {
        return failure_node_;
    }

    void failure_node(TrieNode* node)
    {
        failure_node_ = node;
    }

private:
    static const int alphabet_size_ = 26;
    // assuming for simplicity 26 letter alphabet ('a' - 'z')
    TrieNode* children_[alphabet_size_];
    // where this node is the end of a suffix, that suffix is the prefix to another word
    // this is what allows the transitions between nodes within the automaton
    TrieNode* failure_node_; 
    std::string word_;

};

class Trie
{
public:
    Trie(const std::vector<std::string>& words) : root_(new TrieNode())
    {
        root_->failure_node(root_);

        // build the trie with our dictionary
        for (const std::string& word : words)
        {
            Add(word);
        }

        // set the terminal links within the trie
        for (const std::string& word : words)
        {
            SetTerminalLinks(word);
        }
    }

    ~Trie()
    {
        delete root_;
    }

    /**
     * Cycle through the Trie based automaton - either going to the next character's
     * child Node for the current Node, or following failure links until we either
     * find a Node with the next character as a child, or find ourselves back at
     * the root of the Trie
     */
    std::unordered_set<std::string> Search(const std::string& text_to_search)
    {
        std::unordered_set<std::string> found;
        TrieNode* node = root_;
        TrieNode* next_node = nullptr;

        for (const char& c : text_to_search)
        {
            next_node = node->ChildNode(c);

            // we can't keep walking down the Trie
            if (next_node == nullptr)
            {
                // keep checking the current node & then following failure
                // links until we either find a valid path or find ourselves
                // back at the root of the Trie. The successive failure links
                // will basically be shorter and shorter prefixes based on
                // the matching suffix we started with
                while (next_node == nullptr && node != root_)
                {
                    node = node->failure_node();
                    next_node = node->ChildNode(c);
                }
            }

            // this covers the case where we couldn't match a child node, ended
            // up back at the root node, & the root node still didn't contain the
            // child node
            if (next_node != nullptr)
            {
                node = next_node;
            }

            if (node->has_word())
            {
                found.insert(node->word());
            }
        }

        return found;
    }

private:
    TrieNode* root_;

    /**
     * Normal Trie add - traverse down the Trie following the input word character
     * by character, creating any missing Nodes on the way. Then add this word
     * to the Node for the last character in the word
     */
    void Add(const std::string& word)
    {
        TrieNode* current_node = root_;

        for (const char& c : word)
        {
            TrieNode* node = current_node->ChildNode(c);
            if (node == nullptr) {
                node = new TrieNode();
                current_node->InsertChildNode(c, node);
            }
            current_node = node;
        }

        current_node->word(word);
    }

    /**
     * For each suffix of a word, set where to go for each char (excluding the first
     * last single char suffixes because they would point back to themselves at the
     * root) if the next char doesn't match. We want to point to a prefix matching
     * the suffix we were just on
     */
    void SetTerminalLinks(const std::string& word)
    {
        TrieNode* current_node = root_->ChildNode(word.at(0));
        current_node->failure_node(root_);

        // try each progressively smaller suffix starting with the full word
        for (int i = 1; i < word.size(); i++)
        {
            const char& c = word.at(i);
            current_node = current_node->ChildNode(c);

            // attempt to traverse root_ with substring of word from inx j to i
            // during the traversal, we may hit dead ends which will put us back
            // at the beginning (root_). This is expected & as we go down the Trie,
            // this will give us the loggest subsequence for the progressive nodes
            TrieNode* failure_link_node = root_;
            for (int j = 1; j <= i; j++)
            {
                const char& c = word.at(j);
                failure_link_node = failure_link_node->ChildNode(c);

                // we don't have this prefix in the Trie, so start back at root_
                if (failure_link_node == nullptr)
                {
                    failure_link_node = root_;
                }

                if (j == i)
                {
                    current_node->failure_node(failure_link_node);
                }
            }
        }
    }
};

std::unordered_set<std::string> Search(
    const std::vector<std::string>& words_to_look_for,
    const std::string& text_to_search
) {
    Trie trie(words_to_look_for);
    return trie.Search(text_to_search);
}

}
