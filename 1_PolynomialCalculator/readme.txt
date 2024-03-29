程序支持类命令行操作，支持变量存储
支持的命令列表

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
	
参数说明
> [var]
	变量名，由 大小写字母/数字/下划线 组成，不能以数字开头；避免与自变量x混淆，不得为"x"
> [expression]
	表达式，支持变量/常量/计算符(+.-.*)/括号，支持二元与一元加减计算符
> [double]
	浮点数
	
语法检查
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

程序结构
	1_PolynomialCalcualtor
	|	Polynomial.hpp
	|	Polynomial.cpp
	|		Polynomial 类的定义与实现
	|	main.cpp
	|		主函数
	|		处理输入的命令
	|	readme.txt
	|		本文件

算法细节
多项式
	多项式类 Polynomial 使用单链表存储多项式，节点以指数递增的顺序存储每一项的指数和常数
	链表对外部不可见，保证指针正确被管理。接口支持修改与询问操作，重载了部分运算符

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
        int exp;
        double constant;
        LinkList *nxt;
    }   *m_data;


表达式求值
	将运算符与它的优先级视为一个整体，采用非递归的办法，直接根据运算符的优先级来分析与计算表达式
	参考 OI-wiki 上的 "杂项-表达式求值"

测试数据
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

其他测试
> a = x + 1
> ls
name      val
a         x + 1 
> b = x^2+2x
> ls
name      val
a         x + 1 
b         x^2 + 2 x 
> calc a*b
x^3 + 3 x^2 + 2 x 
> calc a+b
x^2 + 3 x + 1 
> calc a-b
- x^2 - x + 1 
> calc b
x^2 + 2 x 
> d b
> calc b
2 x + 2 
> d b  
> calc b
2 
> d b
> calc b
0
> c = x^-2 + x^2
> ls
name      val
a         1 
b         0
c         x^2 + x^-2 
> rm a
> rm b
> ls
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
> calc a * (x - b * ( x^2 + 1 - a))
- x^5 + 2 x^3 + x^2 
> b = a * (a - 2)
> ls
name      val
a         x + 1 
b         x^2 - 1 
c         120 x^-6 
> val a 1
value = 2
> val b 0.5
value = -0.75
> val b 1.5
value = 1.25
> exit

