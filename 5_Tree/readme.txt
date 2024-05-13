1 需求分析

2 概要
2.1 功能实现

程序支持类命令行操作，支持变量存储
2.2 支持的命令列表

> add [Name]
	添加新人员
> setc [Name1] [Name2]
	组成夫妻关系
> setf [Name1] [Name2]
	组成父子关系
> query {name]
	查询某人的上下代关系
> del {Name]
	删除某人并解除与之相关的关系
> del-relation [Name]
	解除某人的所有关系

3 实现细节
3.1 关系处理
	由于一个人有多个父代，也可以有多个子代，不是严格的树结构，处理子代关系时，添加了 Couple 类节点，父代指针指向 Couple 类节点而不是使用两个指针指向双亲，同时组成夫妻关系时会创建 Couple 类节点储存夫妻信息。
	这么做的好处是，如果 Couple 信息已经存在，指定某人的父代时能够直接查询到父节点对应的 Couple 节点，查询某人的父代能用一个元素返回双亲信息；如果 Couple 信息不存在，可以提醒用户夫妻信息不完整。
	del-relation 会解除所有关系，如果仅需要改变部分关系，直接输入新的关系，就可以解除旧的关系；如果仅想解除关系，留空第二个参数即可。如： setc [Name1] 会解除某人的现有夫妻关系（如果有）
	
3.2 运行实例

> add A
> add B
> add C
> add D
> add E
> add F
> setc A B
> setf C A // 此处 setf C B 也是可以的，因为 A, B 有couple关系
> setc C D
> setf E D
> setf F C
> query C
A---B
  |
  C---D
    |
   ---
   | |
   E F
> exit

3.3 code
3.3.1 类结构

class Node
{
public:    
    
private:

};


class People : public Node
{
public:

    struct Date
    {
        Date(int year, int month, int day) : year(year), month(month), day(day) {}
        int year, month, day;
    };

    People(const std::string &name, const Date &date);

private:

    std::string m_name;
    Date m_borndate;
    Couple *m_couple;
    Couple *m_father;

};

People::People(const std::string &name, const Date &date) :
m_name    (name),
m_borndate(date),
m_couple  (nullptr)
{
}

class Couple : public Node
{
public:

    Couple(People &people1, People &people2);

private:
    
    People *m_p1, *m_p2;
    std::list<People*> m_son;

};

Couple::Couple(People &people1, People &people2) :
m_p1(&people1),
m_p2(&people2)
{
}

3.3.2 命令处理


void getCommand(std::string &command, std::vector<std::string> &args)
{
    command.clear(); args.clear();

    std::cout << "> ";
    std::string str;
    getline(std::cin, str);
    if (!str.size()) return;

    size_t pos = str.find_first_not_of(' ');
    if (pos != std::string::npos)
    {
        str.erase(0, pos);
        pos = str.find_first_of(' ');
        if (pos != std::string::npos)
        {
            command = str.substr(0, str.find_first_of(' '));
            str.erase(0, pos);
        }
        else
        {
            command = str;
            str.clear();
        }
    }
    else return;

    while (str.size())
    {
        pos = str.find_first_not_of(' ');
        if (pos != std::string::npos)
        {
            str.erase(0, pos);
            pos = str.find_first_of(' ');
            if (pos != std::string::npos)
            {
                args.push_back(str.substr(0, pos));
                str.erase(0, pos);
            }
            else
            {
                args.push_back(str);
                str.clear();
            }
        }
        else break;
    }
}

