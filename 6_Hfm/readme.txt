1 需求分析
2 概要
2.1 功能实现
	1 从频率表文件生成哈夫曼树
	2 text与code互相转化
	3 以二进制格式存取文件
2.2 实现细节
频率表存储
	使用 std::map, 适应任意数量字符
哈夫曼树构建
	使用小根堆，每次取出两个最小元素新建共同父节点，权相加重新加入堆，直到堆只有一个节点，此节点即为哈夫曼树的根节点
2.3 测试结果

> build test.freqmap
> t2c ASLNK
Encoded: 1010001110110011011000011
> c2t 1010001110110011011000011
Decoded: ASLNK
> t2f AIOSND ./code.bin
Binary file saved successfully.
> f2t ./code.bin
Binary file loaded successfully.
Decoded from file: AIOSND

3 代码
3.1 哈夫曼树类

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

3.2 文件操作


typedef std::map<char, size_t> FreqMap;
void load_frepmap_from_file(const std::string &path, FreqMap &freqmap)
{
    std::ifstream input_file(path);
    if (!input_file) return;

    freqmap.clear();
    char ch;
    while ((ch = input_file.get()) != EOF)
    {
        size_t freq;
        input_file >> freq;
        input_file.get();
        freqmap.insert(std::make_pair(ch, freq));
    }
}

void save_bin_file(const std::string &path, const std::string &str) {
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.write(str.c_str(), str.size());
        file.close();
        std::cout << "Binary file saved successfully." << std::endl;
    } else {
        std::cerr << "Failed to save binary file." << std::endl;
    }
}

void load_bin_file(const std::string &path, std::string &str) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        str.resize(size);
        if (file.read(&str[0], size))
            std::cout << "Binary file loaded successfully." << std::endl;
        else
            std::cerr << "Failed to load binary file." << std::endl;

        file.close();
    } else {
        std::cerr << "Unable to open binary file." << std::endl;
    }
}


