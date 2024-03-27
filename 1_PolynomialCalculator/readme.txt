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

算法细节
多项式
	多项式类 Polynomial 使用单链表存储多项式，节点以指数递增的顺序存储每一项的指数和常数
	链表对外部不可见，保证指针正确被管理。接口支持修改与询问操作，重载了部分运算符
表达式求值
	将运算符与它的优先级视为一个整体，采用非递归的办法，直接根据运算符的优先级来分析与计算表达式
	参考 OI-wiki 上的 "杂项-表达式求值"

