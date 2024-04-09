1 需求分析

2 概要
Stack类使用单链表实现了栈的常用操作，求全部可能出栈序列采用了递归算法

class Stack{
public:
    Stack(); // 默认构造函数
    Stack(const Stack &other); // 拷贝构造函数
    ~Stack(); // 析构函数

    void push(int val); // 推入栈顶
    void pop(); // 出栈
    void clear(); // 清空栈

    bool empty() const; // 返回栈是否为空
    size_t size() const; // 返回栈内元素数
    int top() const; // 返回栈顶元素，若栈为空返回 0

    Stack& operator = (const Stack& other); // 深拷贝

private:

    struct LinkList // 单链表
    {
        int val; // 值
        LinkList *nxt; // 链表指针
    }   *m_data, *m_top; // m_data存储栈底元素指针， m_top存储栈顶元素指针
}

3 算法细节

3.1 Stack类
3.1.1 主要操作
void Stack::push(int val) // 入栈
{
    if (empty())
    {
        m_top = new LinkList;
        m_data = m_top;
    }
    else
    {
        m_top->nxt = new LinkList;
        m_top = m_top->nxt;
    }
    m_top->val = val;
    m_top->nxt = nullptr;
}

void Stack::pop() // 出栈
{
    if (empty()) return;
    if (m_data == m_top)
    {
        delete m_top;
        m_data = m_top = nullptr;
        return;
    }
    LinkList* tmp = m_data;
    while (tmp->nxt != m_top)
        tmp = tmp->nxt;
    delete m_top;
    m_top = tmp;
}

int Stack::top() const // 获取栈顶元素值
{
    if (!empty()) return m_top->val;
    return 0;
}

3.1.2 其他操作
void Stack::clear() // 清空
{
    while (!empty()) pop();
}

bool Stack::empty() const //  判断是否为空栈
{
    return m_data == nullptr;
}

size_t Stack::size() const // 栈内元素个数
{
    size_t size = 0;
    LinkList *tmp = m_data;
    while (tmp != nullptr)
    {
        tmp = tmp->nxt;
        ++ size;
    }
    return size;
}

3.2 StackOrderCalcer 类
StackOrderCalcer 类计算出栈序列

public:

    typedef std::vector<int> ResArr;
    StackOrderCalcer() : m_size(0) {}

    int build(int size); // 构建元素个数为 size 的解
    size_t size() const { return m_res.size(); } // 解个数
    const ResArr& get(size_t index) const
    {
        return m_res.at(index);
    }


private:

    int m_size;

    std::vector<ResArr> m_res;

    // 使用递归的方式搜索所有可能解
    void buildRecursion(int val)
    {
        static Stack stack;
        static ResArr res;

        if (val == 0) stack.clear(), res.clear();
        if (val == m_size) // 所有元素都已经入栈过
        {
            Stack tmpStack = stack;
            ResArr tmpRes = res;
            while (!tmpStack.empty()) // 让剩余元素全部出栈
            {
                tmpRes.push_back(tmpStack.top());
                tmpStack.pop();
            }
            m_res.push_back(tmpRes);
            return;
        }

        if (!stack.empty()) // 栈不为空，则可以选择出栈
        {
            int tmp = stack.top();
            res.push_back(tmp);
            stack.pop();
            buildRecursion(val);
            res.pop_back();
            stack.push(tmp);
        }
        
        // 选择入栈
        stack.push(val);
        buildRecursion(val + 1);
        stack.pop();
    }

    
4 测试
程序以第一个参数作为 size 构建解

./main 1
output:
[0] 0

./main 2
output:
[0] 0 1
[1] 1 0

./main 3
output:
[0] 0 1 2 
[1] 0 2 1 
[2] 1 0 2 
[3] 1 2 0 
[4] 2 1 0 

./main 4
output:
[0] 0 1 2 3 
[1] 0 1 3 2 
[2] 0 2 1 3 
[3] 0 2 3 1 
[4] 0 3 2 1 
[5] 1 0 2 3 
[6] 1 0 3 2 
[7] 1 2 0 3 
[8] 1 2 3 0 
[9] 1 3 2 0 
[10] 2 1 0 3 
[11] 2 1 3 0 
[12] 2 3 1 0 
[13] 3 2 1 0 

./main 5 > res.txt
output:
见 res.txt

