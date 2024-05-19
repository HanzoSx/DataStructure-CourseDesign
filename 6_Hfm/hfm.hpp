#pragma once

#include <stddef.h>
#include <string>
#include <map>
#include <list>

class HfmTree
{
public:
    HfmTree() : m_treebuilded(false), m_root(nullptr) {}
    ~HfmTree() {}

    void buildTree(std::map<char, size_t> &freqMap);
    void code(const std::string &text, std::string &code);
    void decode(const std::string &code, std::string &text);

private:
    bool m_treebuilded;

    struct Node
    {
        char ch;
        size_t freq;
        Node *left;
        Node *right;
    };
    std::list<Node> m_treenodes;
    Node *m_root;

    // Helper functions
    void buildTreeHelper(std::map<char, size_t> &freqMap);
    Node* createNode(char ch, size_t freq);
    void encodeText(Node* node, std::string code, std::map<char, std::string> &encoding);
    void decodeText(Node* node, size_t &index, const std::string &code, std::string &text);
};
