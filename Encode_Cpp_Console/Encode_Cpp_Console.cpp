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
using namespace std;
template<typename T>
class Tree
{
public:
    Tree();
    Tree(T node ,bool isLeaf = false);
    ~Tree();
    bool IsLeaf;
    T node;
    Tree* left;
    Tree* right;
    Tree* Insert(T node, list<bool> boolList);
    bool AddNodePath(T node,list<bool>& boolList);
    Tree* operator+(Tree<T>& t);
private:

};

template<typename T>
Tree<T>::Tree()
{
    left = nullptr;
    right = nullptr;
    IsLeaf = false;
}
template<typename T>
Tree<T>::Tree(T t, bool isLeaf)
{
    Tree();
    node = t;
    IsLeaf = isLeaf;
}
template<typename T>
Tree<T>::~Tree()
{
        delete left;
        delete right;
}
template<typename T>
Tree<T>* Tree<T>::Insert(T node, list<bool> boolList)
{
    Tree<T>* t = this;
    for (bool b : boolList)
    {
        if (t->IsLeaf)
        {
            break;
        }
        if (b)
        {
            if (nullptr == t->right)
            {
                t->right = new Tree();
            }
            t = t->right;
        }
        else
        {
            if (nullptr == t->left)
            {
                t->left = new Tree();
            }
            t = t->left;
        }
    }
    t->node = node;
    t->IsLeaf = true;
    return this;
}
template<typename T>
bool Tree<T>::AddNodePath(T node, list<bool>& boolList)
{
    if (nullptr==this)
    {
        boolList.pop_back();
        return false;
    }
    if (true==this->IsLeaf)
    {
        if (node == this->node)
        {
            return true;
        }
        else
        {
            boolList.pop_back();
            return false;
        }
    }
    boolList.push_back(false);
    if (this->left->AddNodePath(node, boolList))
    {
        return true;
    }

    boolList.push_back(true);
    if (this->right->AddNodePath(node, boolList))
    {
        return true;
    }
    boolList.pop_back();
    return false;
}
template<typename T>
Tree<T> * Tree<T>::operator+(Tree<T>& t)
{
    Tree<T> *parent = new Tree<T>();
    parent->left = this;
    parent->right = &t;
    return parent;
}

class ShannonFano
{
public:
    list<bool> Encode(string str);
    string Decode(string str);
    void CreateCodeList(string str);
    void CreateCodeTree();
    map<char, list<bool>> codeList;
    vector<pair<int, char>> countVector;
    Tree<char> codeTree;
    static void Show();

private:
    void GetList(int start, int end, int totalCount);
};

list<bool> ShannonFano::Encode(string str)
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
void ShannonFano::GetList(int start, int end, int totalCount)
{
    if (start == end)
    {
        return;
    }
    int i = start;
    int j = i;
    int currentCount = 0;
    int restCount;
    int deltaCount;
    int halfCount = totalCount >> 1;
    while (i <= end)
    {
        currentCount += countVector[i].first;
        if (currentCount > halfCount)
        {
            restCount = totalCount - currentCount;
            deltaCount = currentCount - restCount;
            if (deltaCount > abs(deltaCount - 2 * countVector[i].first))
            {
                i--;
                currentCount -= countVector[i].first;
                restCount = totalCount - currentCount;
            }
            while (j <= i)
            {
                codeList.find(countVector[j].second)->second.push_back(0);
                j++;
            }
            while (j <= end)
            {
                codeList.find(countVector[j].second)->second.push_back(1);
                j++;
            }
            GetList(start, i, currentCount);
            GetList(i + 1, end, restCount);
            break;
        }
        i++;
    }
}

string ShannonFano::Decode(string str)
{
    string result;
    Tree<char>*t = &codeTree;
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
            result.push_back(t->node);
            t = '0' == ch ? codeTree.left : codeTree.right;
        }
    }
    if (t->IsLeaf)
    {
        result.push_back(t->node);
    }
    return result;
}

void ShannonFano::CreateCodeList(string str)
{
    map<char, int> chMap;
    map<char, int >::iterator listIt;
    for (auto ch : str)
    {
        if (chMap.end() == (listIt = chMap.find(ch)))
        {
            chMap.insert(pair<char, int>(ch, 1));
        }
        else
        {
            listIt->second++;
        }
    }
    for (auto p : chMap)
    {
        countVector.emplace_back(p.second, p.first);
    }
    auto cmp = [](pair<int, char>a, pair<int, char>b)
    {
        return a.first > b.first;
    };
    sort(countVector.begin(), countVector.end(), cmp);
    for (auto v : countVector)
    {
        codeList.insert(pair<char, list<bool>>(v.second, list<bool>()));
    }
    GetList(0, (int)countVector.size() - 1, (int)str.length());
}

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
    for (int i = 0; i < n; i++)
    {
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
        codeTree.Insert(p.first, p.second);
    }
}

void ShannonFano::Show()
{
    cout << "ShannonFano Encode" << endl;
    cout << "Please enter a string." << endl;
    string s;
    getline(cin, s);
    ShannonFano sf;
    auto encodedBin=sf.Encode(s);
    list<bool> boolList;
    cout << "The number of char:" << endl;
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

    list<bool> Encode(string str);
    string Decode(string str);
    void CreateCodeList(string str);
    map<char, list<bool>> codeList;
    vector<pair<int, char>> countVector;
    static void Show();
private:

};

void Haffman::CreateCodeList(string str)
{
    map<char, int> chMap;
    map<char, int >::iterator listIt;
    for (auto ch : str)
    {
        if (chMap.end() == (listIt = chMap.find(ch)))
        {
            chMap.insert(pair<char, int>(ch, 1));
        }
        else
        {
            listIt->second++;
        }
    }
    for (auto p : chMap)
    {
        countVector.emplace_back(p.second, p.first);
    }
    auto cmp = [](pair<int, char>a, pair<int, char>b)
    {
        return a.first > b.first;
    };
    sort(countVector.begin(), countVector.end(), cmp);
    for (auto v : countVector)
    {
        codeList.insert(pair<char, list<bool>>(v.second, list<bool>()));
    }

}

void Haffman::Show()
{

}
class Universal
{
public:
    list<bool> Gamma(int x);
    list<bool> Delta(int x);
    list<bool> Encode(string str);
    string Decode(string str);
    void CreateCodeList(string str);
    void CreateCodeTree();
    static void Show();
    map<char, list<bool>> codeList;
    vector<pair<int, char>> countVector;
    Tree<char> codeTree;
private:

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
    map<char, int> chMap;
    map<char, int >::iterator listIt;
    for (auto ch : str)
    {
        if (chMap.end() == (listIt = chMap.find(ch)))
        {
            chMap.insert(pair<char, int>(ch, 1));
        }
        else
        {
            listIt->second++;
        }
    }
    for (auto p : chMap)
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
    for (int i = 0; i < n; i++)
    {
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
        codeTree.Insert(p.first, p.second);
    }
}
list<bool> Universal::Gamma(int x)
{
    int n = (int)log2(x);
    list<bool> binX = list<bool>(n, 0);
    while (x > 0)
    {
        binX.push_front((x % 2) ? 1 : 0);
        x >>= 1;
    }
    binX.reverse();
    return  binX;
}
list<bool> Universal::Delta(int x)
{
    list<bool> binGamma = Gamma((int)log2(x)+1);
    bool isFirstOne = true;
    bool bin;
    list<bool> binX;
    while (x > 0)
    {
        bin = x % 2;
        if (isFirstOne)
        {
            if (bin)
            {
                isFirstOne = false;
            }
            else
            {
                binX.push_back(0);
            }
        }
        else
        {
            binX.push_back(bin ? 1 : 0);
        }
        x >>= 1;
    }
    binX.reverse();
    binGamma.splice(binGamma.end(), binX);
    return binGamma;
}
list<bool> Universal::Encode( string str)
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
    Tree<char>*t = &codeTree;
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
            result.push_back(t->node);
            t = '0' == ch ? codeTree.left : codeTree.right;
        }
    }
    if (t->IsLeaf)
    {
        result.push_back(t->node);
    }
    return result;
}
int main()
{
    //ShannonFano::Show();
    //Universal::Show();
    /**/
#define TEST_TREE 1
#if TEST_TREE
    auto t = new Tree<char>();
    auto t0 = t->left = new Tree<char>();
    auto t00= t0->left = new Tree<char>('1',true);
    auto t01 = t0->right = new Tree<char>();
    auto t010 = t01->left = new Tree<char>('2', true);
    auto t011 = t01->right = new Tree<char>('3', true);
    auto t1 = t->right = new Tree<char>();
    auto t10 = t1->left = new Tree<char>();
    auto t11 = t1->right = new Tree<char>();
    auto t101 = t10->right = new Tree<char>('4', true);
    auto t110 = t11->left = new Tree<char>('5', true);
    list<bool> L = list<bool>();
    t->AddNodePath('5', L);
    cout << "start";
    for (bool b : L)
    {
        cout << "->" << b;
    }
    cout << endl;
#endif // TEST_TREE

    return 0;
}
