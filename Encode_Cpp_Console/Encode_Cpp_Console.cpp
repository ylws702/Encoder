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
    ~Tree();
    bool IsLeaf;
    T node;
    Tree* left;
    Tree* right;
    Tree* insert(T node, list<bool> boolList);
    list<bool> find(T node,list<bool> boolList);
    Tree* operator+(Tree<T>& t);
private:

};

template<class T>
Tree<T>::Tree()
{
    left = nullptr;
    right = nullptr;
    IsLeaf = false;
}
template<class T>
Tree<T>::~Tree()
{
    delete left;
    delete right;
}
template<class T>
Tree<T>* Tree<T>::insert(T node, list<bool> boolList)
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
list<bool> Tree<T>::find(T node, list<bool> boolList)
{
    if (nullptr==this)
    {
        return list<bool>();
    }
    if (true==this->IsLeaf)
    {
        if (node == this->node)
        {
            return boolList;
        }
        else
        {
            return list<bool>();
        }
    }

    auto l1 = boolList;
    auto l2 = l1;
    l1.push_back(false);
    l2.push_back(true);

    auto L1 = this->left->find(node, l1);
    auto L2 = this->left->find(node, l2);

    if (list<bool>() ==L1&& list<bool>() ==L2)
    {
        return list<bool>();
    }
    else if( L1!= list<bool>())
    {
        return L1;
    }
    else
    {
        return L2;
    }
    // TODO: insert return statement here
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
        codeTree.insert(p.first, p.second);
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

    auto L = decodeSF.codeTree.find('1', list<bool>());
    for (auto b : L)
    {
        cout << b;
    }
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
        codeTree.insert(p.first, p.second);
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
    ShannonFano::Show();
    //Universal::Show();
    return 0;
}
