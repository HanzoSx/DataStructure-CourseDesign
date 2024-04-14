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
> exit
	结束程序
	
2.1.2 参数说明
> [var]
	变量名，由 大小写字母/数字/下划线 组成，不能以数字开头
> [expression]
	表达式，支持变量/常量/计算符(+.-.*,/)/括号，支持二元与一元加减计算符
	
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
ERROR: div 0
	除数为 0
	
2.2 算法概要


使用 map<string, int> 管理变量的名称与值


3 算法细节
3.1 栈的表达式求值

std::stack<int> st; // 数值栈
std::stack<char> op; // 操作符栈
bool may_be_unary = true; // 用于判断一元运算符

for (size_t i = 0; i < str.size(); ++ i)  // 顺序访问字符串
{
    if (str[i] == '(') // 左括号直接入栈
    {
        op.push('(');
        may_be_unary = true;
    }
    else if (str[i] == ')') // 右括号：一直提出运算符与运算数直到遇到左括号
    {
        while (!op.empty() and op.top() != '(')
        {
            if (!process_op(st, op.top()))
            {
                err(2, str);
                return false;
            }
            op.pop();
        }
        if (op.empty()) // 没有遇到左括号，提示表达式非法
        {
            err(2, str);
            return false;
        }
        op.pop();
        may_be_unary = false;
    }
    else if (is_op(str[i])) // 遇到运算操作符，取出操作符与操作数直到遇到优先级更低的运算符
    {
        char cur_op = str[i];
        if (may_be_unary && is_unary(cur_op)) cur_op = -cur_op; // 一元操作符
        while (!op.empty() &&
                ((cur_op >= 0 && priority(op.top()) >= priority(cur_op)) ||
                (cur_op < 0 && priority(op.top()) > priority(cur_op)))) {
            if (!process_op(st, op.top()))
            {
                err(2, str);
                return false;
            }
            op.pop();
        }
        op.push(cur_op);
        may_be_unary = true;
    }
    else // 遇到操作数或变量
    {
        std::string tmp; tmp.clear();
        while (i < str.size() && !is_op(str[i]) && str[i] != '(' && str[i] != ')')
            tmp += str[i], ++ i;
        -- i;

        int poly;
        if (!parseExpression(tmp, poly)) return false; // 解析内容
        st.push(poly);
        may_be_unary = false;
    }
}

while (!op.empty()) // 取出操作符与操作数直到操作符栈为空
{
    if (op.top() == '(' or op.top() == ')')
    {
        err(2, str);
        return false;
    }
    if (!process_op(st, op.top()))
    {
        err(2, str);
        return false;
    }
    op.pop();
}
out_Int = st.top();
return true;

3.2 变量存储与多项式求值
void getCommand(std::string &command, std::vector<std::string> &args) // 解析指令与参数
bool parseVariable(std::string str, MapIterator &out_ptrInt, bool creat) // 解析/创建变量
bool parseExpression(std::string str, int &out_Int) // 解析多项式
bool process_op(std::stack<int>& st, char op) // 处理多项式操作符


4 测试数据
> calc 3*(7-2)    
15
> calc 1024/4*8
2048
> calc (20+2)*(6/2)
66
> calc 2*(6+2*(3+6*(6+6)))
312
