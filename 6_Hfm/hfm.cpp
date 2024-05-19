#include <hfm.hpp>

#include <queue>

void HfmTree::buildTree(std::map<char, size_t> &freqMap)
{
    m_root = nullptr;
    m_treenodes.clear();

    buildTreeHelper(freqMap);
    m_treebuilded = true;
}

void HfmTree::buildTreeHelper(std::map<char, size_t> &freqMap)
{
    auto compare = [](Node* left, Node* right)
    { return left->freq > right->freq; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> minHeap(compare);

    for (const auto &pair : freqMap)
    {
        Node* newNode = createNode(pair.first, pair.second);
        minHeap.push(newNode);
    }

    while (minHeap.size() > 1)
    {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        Node* parent = createNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        minHeap.push(parent);
    }

    m_root = minHeap.top(); minHeap.pop();
}

void HfmTree::code(const std::string &text, std::string &code)
{
    if (!m_treebuilded) return;

    std::map<char, std::string> encoding;
    encodeText(m_root, "", encoding);

    for (char ch : text)
        code += encoding[ch];
}

void HfmTree::encodeText(Node* node, std::string code, std::map<char, std::string> &encoding)
{
    if (node->left == nullptr && node->right == nullptr)
    {
        encoding[node->ch] = code;
        return;
    }

    if (node->left != nullptr)
        encodeText(node->left, code + '0', encoding);

    if (node->right != nullptr)
        encodeText(node->right, code + '1', encoding);
}

void HfmTree::decode(const std::string &code, std::string &text)
{
    if (!m_treebuilded) return;

    size_t index = 0;
    while (index < code.size())
    {
        decodeText(m_root, index, code, text);
    }
}

void HfmTree::decodeText(Node* node, size_t &index, const std::string &code, std::string &text)
{
    if (node->left == nullptr && node->right == nullptr)
    {
        text += node->ch;
        return;
    }

    if (code[index] == '0')
        decodeText(node->left, ++index, code, text);
    else
        decodeText(node->right, ++index, code, text);
}

HfmTree::Node* HfmTree::createNode(char ch, size_t freq)
{
    m_treenodes.emplace_back(Node{ch, freq, nullptr, nullptr});
    return &m_treenodes.back();
}
