1 需求分析

2 概要
2.1 功能实现

程序支持类命令行操作，支持变量存储
2.1.1 支持的命令列表

> [var] = [expression]
	为变量赋值，若变量不存在则创建
> list
> ls
	输出 变量-值 列表
> remove [var]
> rm [var]
	移除变量
> calc [expression]
> c [expression]
	输出表达式的值
> d [var]
	将变量求导
> value [var] [double]
> val [var] [double]
	对变量求实际值(x = [double])
> exit
	结束程序
	
2.1.2 参数说明
> [var]
	变量名，由 大小写字母/数字/下划线 组成，不能以数字开头；避免与自变量x混淆，不得为"x"
> [expression]
	表达式，支持变量/常量/计算符(+.-.*)/括号，支持二元与一元加减计算符
> [double]
	浮点数
	
2.1.3 语法检查
所有指令均有语法检查，可能给出的错误列表
ERROR: Variable does not exist [arg]
	变量 [arg] 不存在
ERROR: Illegal variable name [arg]
	变量名 [arg] 不合法
ERROR: Illegal expression [arg]
	表达式 [arg] 不合法
ERROR: Command not found [arg]
	命令 [arg] 不存在
ERROR: Wrong number of parameters
	参数数量不正确
	
2.2 算法概要

多项式类 Polynomial 使用单链表存储多项式，节点以指数递增的顺序存储每一项的指数和常数
链表对外部不可见，保证指针正确被管理。接口支持修改与询问操作，重载了部分运算符
使用 map<string, Polynomial> 管理多项式变量的名称与值

  public:

    Polynomial(); // 默认构造函数
    Polynomial(const Polynomial &other); // 拷贝构造函数
    ~Polynomial(); // 析构函数

    void clear(); // 清空数据，使多项式为0
    void remove(int exp); // 移除exp次项
    void set(int exp, double constant); // 将exp次项常数部分设置为constant

    void derivative(); // 求导
    double value(double x); // 求实际值

    double at(int exp) const; // 查询exp次项的常数部分
    double operator [](int exp) const; // 重载[]，等价与at()
    size_t size() const; // 项数
    std::pair<int, double> node(size_t index) const; // 返回从低次项到高次项第index项的节点数据

    Polynomial& operator = (const Polynomial &other); // 深拷贝

    Polynomial operator + (const Polynomial &other) const; // 重载加法运算符
    Polynomial operator - () const; // 重载一元减法运算符
    Polynomial operator - (const Polynomial &other) const; // 重载二元减法运算符
    Polynomial operator * (const Polynomial &other) const; // 重载乘法运算符
    Polynomial& operator += (const Polynomial &other); // 重载+=
    Polynomial& operator -= (const Polynomial &other); // 重载-=
    Polynomial& operator *= (const Polynomial &other); // 重载*=

    void Print(); // 输出类数学表达式
    
  private:
    
    struct LinkList
    {
        int exp; // 指数
        double constant; // 常数
        LinkList *nxt; // 链表指针
    }   *m_data;

3 算法细节
3.1 主要操作

void Polynomial::set(int exp, double constant) // 将exp次项常数部分设置为constant
{
    // 为了在单链表中间插入元素， ptrLast 存储 ptr 指向元素的前一个元素
    LinkList *ptr = m_data, *ptrLast = nullptr;
    while (ptr != nullptr and ptr->exp < exp) // 寻找 exp 次项的位置
    {
        ptrLast = ptr;
        ptr = ptr->nxt;
    }
    if (ptr != nullptr and ptr->exp == exp) // exp 次项已经存在，直接修改常数
        ptr->constant = constant;
    else // exp 次项不存在
    {
        if (ptrLast == nullptr) // 特殊情况：多项式为空
        {
            m_data = new LinkList;
            m_data->exp = exp;
            m_data->constant = constant;
            m_data->nxt = ptr;
        }
        else
        {
            ptrLast->nxt = new LinkList; // 新建节点
            ptrLast->nxt->exp = exp;
            ptrLast->nxt->constant = constant;
            ptrLast->nxt->nxt = ptr;
        }
    }
}

Polynomial Polynomial::operator + (const Polynomial &other) const // 二元加法
{
    Polynomial tmp; tmp = *this;
    for (size_t index = 0; index < other.size(); ++ index)
    {
        auto [exp, constant] = other.node(index);
        tmp.set(exp, constant + tmp.at(exp)); // 对应项常数相加
    }
    return tmp;
}

Polynomial Polynomial::operator - () const // 一元减法
{
    Polynomial tmp;
    for (size_t index = 0; index < size(); ++ index)
    {
        auto [exp, constant] = node(index);
        tmp.set(exp, -constant); // 常数部分取相反数
    }
    return tmp;
}

Polynomial Polynomial::operator - (const Polynomial &other) const // 二元减法
{
    return *this + -other; // 转化为二元加法与一元减法
}

Polynomial Polynomial::operator * (const Polynomial &other) const // 乘法
{
    Polynomial tmp;
    for (size_t index = 0; index < size(); ++ index) // 枚举项
    {
        auto [exp, constant] = node(index);
        for (size_t _index = 0; _index < other.size(); ++ _index) // 枚举项
        {
            auto [_exp, _constant] = other.node(_index);
            tmp.set(exp+_exp, tmp.at(exp+_exp) + constant*_constant); // 两项相乘
        }
    }
    return tmp;
}

Polynomial& Polynomial::operator += (const Polynomial &other)
{
    return *this = *this + other;
}

Polynomial& Polynomial::operator -= (const Polynomial &other)
{
    return *this = *this - other;
}

Polynomial& Polynomial::operator *= (const Polynomial &other)
{
    return *this = *this * other;
}

void Polynomial::Print() // 输出类数学表达式
{
    for (size_t index = 0; index < size(); ++ index)
        if (std::abs(node(index).second) < 1e-6) // 删除精度误差导致的 0 项
        {
            remove(node(index).first);
            -- index;
        }

    for (size_t index = 0; index < size(); ++ index)
    {
        auto [exp, constant] = node(size() - index - 1);
        if (index > 0 or constant < 0)
            std::cout << (constant > 0 ? "+" : "-") << " "; // 输出符号
        if (exp == 0 or std::abs(std::abs(constant) - 1) > 1e-6)
            std::cout << std::abs(constant) << " "; // 输出常数
        if (exp != 0) // 输出 x(^exp)
        {
            std::cout << "x";
            if (exp != 1)
                std::cout << "^" << exp;
            std::cout << " ";
        }
    }

    if (size() == 0) // 特殊情况：多项式为 0
        std::cout << 0;
}

void Polynomial::derivative() // 求导
{
    LinkList *ptr = m_data;
    while (ptr != nullptr)
    {
        ptr->constant *= ptr->exp --;
        ptr = ptr->nxt;
    }
}

double Polynomial::value(double x) // 求值
{
    double value = 0;
    LinkList *ptr = m_data;
    while (ptr != nullptr)
    {
        value += std::pow(x, ptr->exp) * ptr->constant;
        ptr = ptr->nxt;
    }
    return value;
}



3.2 其他成员函数

Polynomial::Polynomial() : m_data(nullptr) {} // default constructor

Polynomial::Polynomial(const Polynomial &other) : Polynomial() // 拷贝构造函数
{
    *this = other; // 深拷贝
}

Polynomial::~Polynomial() // 析构函数
{
    clear(); // 清空
}

void Polynomial::clear() // 清空，释放内存
{
    std::list<LinkList*> ptrList;
    LinkList *ptr = m_data;
    while (ptr != nullptr)
    {
        ptrList.push_front(ptr);
        ptr = ptr->nxt;
    }
    for (auto &it : ptrList)
        delete it;

    m_data = nullptr;
}

void Polynomial::remove(int exp) // 移除 exp 次项
{
    LinkList *ptr = m_data, *ptrLast = nullptr;
    while (ptr->exp != exp)
    {
        if (ptr->nxt == nullptr) return;
        ptrLast = ptr;
        ptr = ptr->nxt;
    }
    if (ptr->exp != exp) return;
    if (ptrLast == nullptr)
        m_data = ptr->nxt;
    else
        ptrLast->nxt = ptr->nxt;
    delete ptr;
}

double Polynomial::at(int exp) const
{
    const LinkList *ptr = m_data;
    while (ptr != nullptr)
    {
        if (ptr->exp == exp)
            return ptr->constant;
        ptr = ptr->nxt;
    }
    return 0.;
}

double Polynomial::operator [](int exp) const
{
    return at(exp);
}

size_t Polynomial::size() const
{
    size_t size = 0;
    const LinkList *ptr = m_data;
    while (ptr != nullptr)
    {
        ptr = ptr->nxt;
        ++ size;
    }
    return size;
}

std::pair<int, double> Polynomial::node(size_t index) const
{
    if (index >= size()) return std::make_pair(0, 0.);
    const LinkList *ptr = m_data;
    for (size_t i = 0; i < index; ++ i)
        ptr = ptr->nxt;
    return std::make_pair(ptr->exp, ptr->constant);
}

// 保证 Polynomial 类在容器中的独立性
Polynomial& Polynomial::operator = (const Polynomial &other) // 深拷贝
{
    if (this == &other) return *this;
    
    clear();
    for (size_t index = 0; index < other.size(); ++ index)
    {
        auto [exp, constant] = other.node(index);
        set(exp, constant);
    }
    return *this;
}

3.3 指令处理、变量存储与多项式求值
void getCommand(std::string &command, std::vector<std::string> &args) // 解析指令与参数
bool parseVariable(std::string str, MapIterator &out_ptrPoly, bool creat) // 解析/创建变量
bool parseExpression(std::string str, Polynomial &out_Poly) // 解析多项式
bool process_op(std::stack<Polynomial>& st, char op) // 处理多项式操作符


4 测试数据
4.1
> calc (2x + 5x^8 - 3.1x^11)+(7-5x^8 + 11x^9)
- 3.1 x^11 + 11 x^9 + 2 x + 7 

> calc (6x^-3 - x + 4.4x^2 - 1.2x^9) - (-6x^-3 + 5.4x^2 - x^2 + 7.8x^15)
- 7.8 x^15 - 1.2 x^9 - x + 12 x^-3 

> calc (1+x+x^2+x^3+x^4+x^5)+(-x^3-x^4)
x^5 + x^2 + x + 1 

> calc (x+x^3) + (-x-x^3)
0

> calc (x+x^100)+(x^100+x^200)
x^200 + 2 x^100 + x 

> calc (x+x^2+x^3)+0
x^3 + x^2 + x

4.2 其他测试
> a = x + 1 // 创建变量 a
> ls // 变量列表
name      val
a         x + 1 
> b = x^2+2x // 创建变量 b
> ls
name      val
a         x + 1 
b         x^2 + 2 x 
> calc a*b // 带变量的多项式计算
x^3 + 3 x^2 + 2 x 
> calc a+b
x^2 + 3 x + 1 
> calc a-b
- x^2 - x + 1 
> calc b
x^2 + 2 x 
> d b // 变量求导
> calc b
2 x + 2 
> d b  
> calc b
2 
> d b
> calc b
0
> c = x^-2 + x^2 // 创建变量 c
> ls
name      val
a         1 
b         0
c         x^2 + x^-2 
> rm a // 移除变量 a
> rm b // 移除变量 b
> ls // 变量列表
name      val
c         x^2 + x^-2 
> d c
> ls
name      val
c         2 x - 2 x^-3 
> d c
> ls
name      val
c         2 + 6 x^-4 
> d c
> ls
name      val
c         - 24 x^-5 
> d c
> ls
name      val
c         120 x^-6 
> a = x+1
> b = x^2-1
> calc a * (x - b * ( x^2 + 1 - a)) // 带括号、变量的表达式求值
- x^5 + 2 x^3 + x^2 
> b = a * (a - 2) // 带变量表达式赋值给变量
> ls
name      val
a         x + 1 
b         x^2 - 1 // a * (a - 2) = (x^2 + 1) * (x^2 + 1 - 2) = x^2 - 1
c         120 x^-6 
> val a 1 // 变量求值
value = 2 // a = x + 1 (x = 1) = 2
> val b 0.5
value = -0.75 // b = x^2 - 1 (x= 0.5) = -0.75
> val b 1.5
value = 1.25 // b = x^2 - 1 (x= 1.5) = 1.25
> exit // 退出程序

