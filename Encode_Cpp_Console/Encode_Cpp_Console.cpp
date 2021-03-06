// Encode_Cpp_Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<list>
#include<map>
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<iomanip>
using namespace std;
///二叉树类
class Tree
{
public:
    Tree() {};
    ~Tree();
    bool IsLeaf{ false };               //是否为叶子节点，初始化为否
    char ch;                            //叶子节点的字符
    Tree* left{ nullptr };              //左子树，初始化为空指针
    Tree* right{ nullptr };             //右子树，初始化为空指针
    //将01串插入二叉树形成新的二叉树
    Tree* insert(char ch, list<bool> boolList);
};

Tree::~Tree()
{
    delete left;                        //析构             
    delete right;
}
Tree* Tree::insert(char ch, list<bool> boolList)
{
    Tree* t = this;
    for (bool b : boolList)             //逐个取出01串中的0与1，
    {                                   //根据0或1向左向右移动指针t。
        if (t->IsLeaf)                  //如果是叶子节点，
        {                               //说明该01串的前部分与之前的某一01串重合，
            break;                      //放弃将该01串放入二叉树中。
        }
        if (b)                          //如果为1，向右移动。
        {
            if (nullptr == t->right)    //如果右子树为空的，
            {
                t->right = new Tree();  //new一个右子树，
            }
            t = t->right;               //再向右移动。
        }
        else                            //如果为0，同理，向左移动t
        {
            if (nullptr == t->left)
            {
                t->left = new Tree();
            }
            t = t->left;
        }
    }
    t->ch = ch;                         //处理01串最后一位
    t->IsLeaf = true;                   //打上叶子标记
    return this;
}
class ShannonFano
{
public:
    list<bool> Encode(string str);      //编码函数
    string Decode(string str);          //解码函数
    void CreateCodeList(string str);    //编码生成codeList
    void CreateCodeTree();              //编码生成codeTree
    map<char, list<bool>> codeList;     //字符到01串的映射
    vector<pair<int, char>> countVector;//字符计数使用的向量
    Tree codeTree;                      //解码用的二叉树
    static void Show();                 //测试时使用的函数

private:
    //递归分解得到01串
    void GetList(int start, int end, int totalCount);
};
//编码函数
list<bool> ShannonFano::Encode(string str)
{
    list<bool> result;                  //编码的01串结果
    CreateCodeList(str);                //调用函数，生成codeList
    list<bool> addList;                 //保存每个字符的01串编码
    for (char ch : str)                 //遍历str，将每个字符的01串编码连起来
    {                                   //保存到result里
        addList = codeList.find(ch)->second;
        result.splice(result.end(), addList, addList.begin(), addList.end());
    }
    return result;
}
//递归分解得到01串
//该函数是根据之前字符计数得到的countVector进行字符编码
//start:开始位置
//end:结束位置
//totalCount:从开始到结束部分的所有字符的总共数目
void ShannonFano::GetList(int start, int end, int totalCount)
{
    if (start == end)                   //结束递归     
    {
        return;
    }
    int i = start;
    int j = i;
    int currentCount = 0;               //当前数过的字符数
    int restCount;                      //剩下的字符数
    int deltaCount;                     //数过的字符数与剩下的字符数之差
    int halfCount = totalCount >> 1;    //一半数目，是临界点
    while (i <= end)
    {
        currentCount += countVector[i].first;
        //到达临界点
        if (currentCount > halfCount)
        {
            //计算剩下的字符数和数过的字符数与剩下的字符数的差值
            restCount = totalCount - currentCount;
            deltaCount = currentCount - restCount;
            //判断两半差值是否是最小的
            if (deltaCount > abs(deltaCount - 2 * countVector[i].first))
            {
                //如果不是就将i减一，因为差值最小值一定在临界值前后
                //并重新计算剩下的字符数和数过的字符数与剩下的字符数的差值
                i--;
                currentCount -= countVector[i].first;
                restCount = totalCount - currentCount;
            }
            //将所有前一半的字符的编码添上一个0
            while (j <= i)
            {
                codeList.find(countVector[j].second)->second.push_back(0);
                j++;
            }
            //将所有后一半的字符的编码添上一个1
            while (j <= end)
            {
                codeList.find(countVector[j].second)->second.push_back(1);
                j++;
            }
            //前半部分字符递归添0或1直到每一半只有一个字符
            GetList(start, i, currentCount);
            //同理，后半部分字符递归添0或1直到每一半只有一个字符
            GetList(i + 1, end, restCount);
            break;
        }
        i++;
    }
}
//解码函数
//通过已有的二叉树解码
//str:字符01串
string ShannonFano::Decode(string str)
{
    string result;                      //解码结果
    Tree*t = &codeTree;                 //已有的解码二叉树
    for (char ch : str)
    {
        if (!('0' == ch || '1' == ch))  //跳过不是0或1的字符
        {
            continue;
        }
        if (false == t->IsLeaf)         //如果没到叶子，就根据0或1向左向右移动指针
        {
            t = '0' == ch ? t->left : t->right;
        }
        else                            //到叶子节点，向结果追加叶子节点上的字符
        {                               //同时依据0或1让指针t回到编码树的左子树或右子树
            result.push_back(t->ch);
            t = '0' == ch ? codeTree.left : codeTree.right;
        }
    }
    if (t->IsLeaf)                      //编码最后一位
    {                                   //如果01串不完整
        result.push_back(t->ch);        //舍弃掉最后一字符
    }
    return result;                      //返回结果
}
//编码生成codeList
//这个函数主要是计数，最后调用GetList函数生成codeList
void ShannonFano::CreateCodeList(string str)
{
    map<char, int> countMap;            //字符到字数出现次数的映射
    map<char, int >::iterator listIt;   //迭代器
    for (char ch : str)
    {
        //如果字符不在表里，就添加它
        if (countMap.end() == (listIt = countMap.find(ch)))
        {
            countMap.insert(pair<char, int>(ch, 1));
        }
        //如果出现过在表里，就将次数加一
        else
        {
            listIt->second++;
        }
    }
    //将表复制到向量里，因为向量不好查找字符
    for (auto p : countMap)
    {
        countVector.emplace_back(p.second, p.first);
    }
    //比较函数
    auto cmp = [](pair<int, char>a, pair<int, char>b)
    {
        return a.first > b.first;
    };
    //由小到大排序
    sort(countVector.begin(), countVector.end(), cmp);
    //把字符依次序放到链表中，这些字符是没有重复的
    for (auto v : countVector)
    {
        codeList.insert(pair<char, list<bool>>(v.second, list<bool>()));
    }
    //调用函数生成编码表
    GetList(0, (int)countVector.size() - 1, (int)str.length());
}
//根据输入的编码表生成解码用的二叉树
//主要还是调用二叉树类中的insert函数
//该函数没有什么算法，主要是与用户进行交互
//效果见测试图
void ShannonFano::CreateCodeTree()
{
    cout << endl;
    cout << "Create code binary tree." << endl;
    cout << "Please enter the number of characters:";
    int n;
    char ch;
    list<bool> decodeBoolList;
    string binString;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cout << "Please enter the character#" << i << " and its decoder binary string:" << endl;
        cin >> ch >> binString;
        decodeBoolList.clear();
        for (char c : binString)
        {
            decodeBoolList.push_back('1' == c);
        }
        codeList.insert(pair<char, list<bool>>(ch, decodeBoolList));
    }
    for (auto p : codeList)
    {
        //调用insert函数
        codeTree.insert(p.first, p.second);
    }
}
//打印函数，具体效果见测试图
void ShannonFano::Show()
{
    cout << "ShannonFano Encode" << endl;
    cout << "Please enter a string." << endl;
    string s;
    getline(cin, s);
    ShannonFano sf;
    auto encodedBin = sf.Encode(s);
    list<bool> boolList;
    cout << "The number of char:" << endl;
    cout << s.length() << " character(s) totally." << endl;
    for (auto p : sf.countVector)
    {
        cout << p.second << "\t" << p.first << endl;;
    }
    cout << endl;
    cout << "The encode list:" << endl;
    cout << sf.countVector.size() << " character(s) totally." << endl;
    for (auto p : sf.countVector)
    {
        cout << p.second << "\t";
        boolList = sf.codeList.find(p.second)->second;
        for (bool b : boolList)
        {
            cout << b;
        }
        cout << endl;
    }
    cout << endl;
    cout << "The binary string:" << endl;
    cout << encodedBin.size() << " bit(s) totally." << endl;
    for (bool b : encodedBin)
    {
        cout << b;
    }
    cout << endl;

    cout << endl;
    cout << "ShannonFano Decode" << endl;
    ShannonFano decodeSF;
    decodeSF.CreateCodeTree();
    cout << "Please enter the binary code." << endl;
    string decodeBinStr;
    cin >> decodeBinStr;
    cout << endl;
    cout << "Decode result:" << endl;
    cout << decodeSF.Decode(decodeBinStr) << endl;
}
class Haffman
{
public:
    static void show();
private:

};

void Haffman::show()
{

}

class Arithmetic
{
public:
    static void Show();                 //打印函数
    double Encode(string str);          //编码函数
    void CreateCodeList(string str);    //生成编码区间
    map<char, int> countMap;            //计数向量
    //字符与其编码区间的映射
    map<char, pair<double, double>> codeList;
};
//打印函数
//效果见侧视图
void Arithmetic::Show()
{
    cout << "Arithmetic Encode" << endl;
    cout << "Please enter a string." << endl;
    string s;
    getline(cin, s);
    Arithmetic a;
    double encodedDouble = a.Encode(s);
    list<bool> boolList;
    cout << "The number of char:" << endl;
    cout << s.length() << " character(s) totally." << endl;
    for (auto p : a.countMap)
    {
        cout << p.first << "\t" << p.second << endl;;
    }
    cout << endl;
    cout << "The encode interval list:" << endl;
    //打印字符总数
    cout << a.codeList.size() << " character(s) totally." << endl;
    cout << setprecision(4);
    cout << setiosflags(ios::left);
    cout << setw(16) << "character";
    cout << setw(16) << "left";
    cout << setw(16) << "size";
    cout << endl;
    //打印字符与其编码区间的映射表
    for (auto p : a.codeList)
    {
        cout << setw(16) << p.first;
        cout << setw(16) << p.second.first;
        cout << setw(16) << p.second.second;
        cout << endl;
    }
    cout << endl;
    cout << "The result:" << endl;
    cout << encodedDouble << endl;

}
void Arithmetic::CreateCodeList(string str)
{
    map<char, int >::iterator listIt;
    for (auto ch : str)
    {
        if (countMap.end() == (listIt = countMap.find(ch)))
        {
            countMap.insert(pair<char, int>(ch, 1));
        }
        else
        {
            listIt->second++;
        }
    }
    int currentCount = 0;
    int totalCount = (int)str.length();
    //对应区间左端点和区间长度
    double left, size;
    auto p = pair<double, double>(left, size);
    for (auto v : countMap)
    {
        left = currentCount;
        left /= totalCount;             //左端点为前面累积字符数除以字符总数
        size = v.second;
        size /= totalCount;             //区间长度为当前字符数除以字符总数
        p = pair<double, double>(left, size);
        //放入表中
        codeList.insert(pair<char, pair<double, double>>(v.first, p));
        currentCount += v.second;
    }
}

double Arithmetic::Encode(string str)
{
    if (str.length() <= 0)              //若字符串为空,直接返回0
    {
        return 0.0;
    }
    CreateCodeList(str);                //生成编码区间
    double left = 0, size = 1;          //初始区间左端点以及区间长度

    for (char ch : str)
    {
        auto p = codeList.find(ch)->second;
        left += p.first*size;           //依据算法,左端加上字符区间左端与当前区间长度之积
        size *= p.second;               //当前区间长度乘上字符的区间长度
    }

    return left;                        //以区间左端点作为返回结果
}

class Universal
{
public:
    list<bool> Gamma(unsigned int x);   //获取整数x的gamma code,返回01串
    list<bool> Delta(unsigned int x);   //获取整数x的delta code,返回01串
    list<bool> Encode(string str);      //使用delta code 编码字符串, 返回01串
    string Decode(string str);          //解码字符串类型的01一串,返回原始字符串
    void CreateCodeList(string str);    //编码生成codeList,与香农范诺编码中是一样的
    void CreateCodeTree();              //编码生成codeTree,与香农范诺编码中是一样的
    static void Show();                 //打印
    map<char, list<bool>> codeList;     //字符到01串的映射
    vector<pair<int, char>> countVector;//字符计数使用的向量
    Tree codeTree;                      //解码用的二叉树
};

void Universal::Show()
{
    cout << "Universal Encode" << endl;
    cout << "Please enter a string." << endl;
    string s;
    getline(cin, s);
    Universal u;
    auto encodedBin = u.Encode(s);
    list<bool> boolList;
    cout << "The number of char:" << endl;
    cout << s.length() << " character(s) totally." << endl;
    for (auto p : u.countVector)
    {
        cout << p.second << "\t" << p.first << endl;;
    }
    cout << endl;
    cout << "The encode list:" << endl;
    cout << u.countVector.size() << " character(s) totally." << endl;
    for (auto p : u.countVector)
    {
        cout << p.second << "\t";
        boolList = u.codeList.find(p.second)->second;
        for (bool b : boolList)
        {
            cout << b;
        }
        cout << endl;
    }
    cout << endl;
    cout << "The binary string:" << endl;
    cout << encodedBin.size() << " bit(s) totally." << endl;
    for (bool b : encodedBin)
    {
        cout << b;
    }
    cout << endl;

    cout << endl;
    cout << "Universal Decode" << endl;
    Universal decodeU;
    decodeU.CreateCodeTree();
    cout << "Please enter the binary code." << endl;
    string decodeBinStr;
    cin >> decodeBinStr;
    cout << endl;
    cout << "Decode result:" << endl;
    cout << decodeU.Decode(decodeBinStr) << endl;
}
void Universal::CreateCodeList(string str)
{
    map<char, int> countMap;
    map<char, int >::iterator listIt;
    for (auto ch : str)
    {
        if (countMap.end() == (listIt = countMap.find(ch)))
        {
            countMap.insert(pair<char, int>(ch, 1));
        }
        else
        {
            listIt->second++;
        }
    }
    for (auto p : countMap)
    {
        countVector.emplace_back(p.second, p.first);
    }
    auto cmp = [](pair<int, char>a, pair<int, char>b)
    {
        return a.first > b.first;
    };
    sort(countVector.begin(), countVector.end(), cmp);
    int i = 1;
    for (auto v : countVector)
    {
        codeList.insert(pair<char, list<bool>>(v.second, Delta(i++)));
    }
}
void Universal::CreateCodeTree()
{
    cout << endl;
    cout << "Create code binary tree." << endl;
    cout << "Please enter the number of characters:";
    int n;
    char ch;
    list<bool> decodeBoolList;
    string binString;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cout << "Please enter the character#" << i << " and its decoder binary string:" << endl;
        cin >> ch >> binString;
        decodeBoolList.clear();
        for (char c : binString)
        {
            decodeBoolList.push_back('1' == c);
        }
        codeList.insert(pair<char, list<bool>>(ch, decodeBoolList));
    }
    for (auto p : codeList)
    {
        //调用insert函数
        codeTree.insert(p.first, p.second);
    }
}
//获取整数x的gamma code,返回01串
list<bool> Universal::Gamma(unsigned int x)
{
    int n = (int)log2(x);               //取对数并向下取整
    list<bool> binX = list<bool>(n, 0); //这个链表是gamma code的反转01串,先初始化n个0
    while (x > 0)                       //再依次在链表最前添加反向的x的二进制
    {
        binX.push_front((x % 2) ? 1 : 0);
        x >>= 1;
    }
    binX.reverse();                     //将链表反转就得到x的gamma code
    return  binX;
}
//获取整数x的delta code,返回01串
list<bool> Universal::Delta(unsigned int x)
{
    //先获取log2x+1的gamma code
    list<bool> binGamma = Gamma((int)log2(x) + 1);
    bool isFirstOne = true;             //判断是否是第一个1
    bool bin;                           //x最低位
    list<bool> binX;                    //x的去掉第一个1的二进制
    while (x > 0)
    {
        bin = x % 2;                    //取得x的最低位
        if (isFirstOne)                 //如果没发现第一个1
        {
            if (bin)                    //如果是1
            {
                isFirstOne = false;     //设为已发现发现第一个1
            }                           //不对结果做任何操作
            else                        //如果是0
            {
                binX.push_front(0);     //添入结果链表最前面
            }
        }
        else                            //如果已发现第一个1
        {                               //直接添加x最低位到结果链表最前面
            binX.push_front(bin ? 1 : 0);
        }
        x >>= 1;                        //右移一位
    }
    //将x的去掉第一个1的二进制添到log2x+1的gamma code后面
    binGamma.splice(binGamma.end(), binX);
    return binGamma;
}
//
list<bool> Universal::Encode(string str)
{
    list<bool> result;
    CreateCodeList(str);
    list<bool> addList;
    for (char ch : str)
    {
        addList = codeList.find(ch)->second;
        result.splice(result.end(), addList, addList.begin(), addList.end());
    }
    return result;
}

string Universal::Decode(string str)
{
    string result;
    Tree*t = &codeTree;
    for (char ch : str)
    {
        if (!('0' == ch || '1' == ch))
        {
            continue;
        }
        if (false == t->IsLeaf)
        {
            t = '0' == ch ? t->left : t->right;
        }
        else
        {
            result.push_back(t->ch);
            t = '0' == ch ? codeTree.left : codeTree.right;
        }
    }
    if (t->IsLeaf)
    {
        result.push_back(t->ch);
    }
    return result;
}

class BSTW
{
public:
    void CreateTmpList(string str);
    //void CreateCodeList(string str);
    list<bool> Encode(string str);
    static void Show();
    map<char, int> charMap;
    list<int> tmpList;
private:

};

void BSTW::CreateTmpList(string str)
{
    for (char ch : str)
    {
        auto it = charMap.find(ch);
        if (charMap.end() == it)
        {
            tmpList.push_back((int)charMap.size() + 1);
            tmpList.push_back(-ch);
            for (auto i = charMap.begin(); i != charMap.end(); i++)
            {
                i->second++;
            }
            charMap.insert(pair<char, int>(ch, 1));
        }
        else
        {
            tmpList.push_back(it->second);
            for (auto i = charMap.begin(); i != charMap.end(); i++)
            {
                if (i->second < it->second)
                {
                    i->second++;
                }
            }
            it->second = 1;
        }
    }
}
list<bool> BSTW::Encode(string str)
{
    list<bool> result;
    CreateTmpList(str);
    return result;
}

void BSTW::Show()
{
    cout << "BSTW Encode" << endl;
    cout << "Please enter a string." << endl;
    string s;
    getline(cin, s);
    BSTW bstw;
    auto encodedBin = bstw.Encode(s);
    cout << bstw.charMap.size() << " character(s) totally." << endl;
    for (int n : bstw.tmpList)
    {
        if (n > 0)
        {
            cout << " " << n;
        }
        else
        {
            cout << " (" << (char)-n << ")";
        }
    }
    cout << endl;
}
class LempelZiv
{
public:
    list<bool> Encode(string str);
    static void Show();
private:

};

list<bool> LempelZiv::Encode(string str)
{
    return list<bool>();
}

void LempelZiv::Show()
{
}

int main()
{
    BSTW::Show();
    return 0;
}
