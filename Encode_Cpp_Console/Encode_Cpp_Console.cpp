// Encode_Cpp_Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<list>
#include<map>
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;
class Tree
{
public:
    Tree();
    ~Tree();
    bool IsLeaf;
    char ch;
    Tree* left;
    Tree* right;
    Tree* insert(char ch, list<bool> boolList);
private:

};

Tree::Tree()
{
    left = NULL;
    right = NULL;
    IsLeaf = false;
}

Tree::~Tree()
{
    delete left;
    delete right;
}

Tree* Tree::insert(char ch, list<bool> boolList)
{
    Tree* t = this;
    for (bool b : boolList)
    {
        if (t->IsLeaf)
        {
            break;
        }
        if (b)
        {
            if (NULL == t->right)
            {
                t->right = new Tree();

            }
            t = t->right;
        }
        else
        {
            if (NULL == t->left)
            {
                t->left = new Tree();
            }
            t = t->left;
        }
    }
    t->ch = ch;
    t->IsLeaf = true;
    return this;
}
class ShannonFano
{
public:
    void Encode(string str);
    string Decode(string str);
    void createCodeTree();
    map<char, list<bool>> codeList;
    vector<pair<int, char>> countVector;
    Tree codeTree;
    static void Show();

private:
    void GetList(int start, int end, int totalCount);
};

void ShannonFano::Encode(string str)
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
    GetList(0, countVector.size() - 1, str.length());
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


void ShannonFano::createCodeTree()
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
    sf.Encode(s);
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
    for (char ch : s)
    {
        boolList = sf.codeList.find(ch)->second;
        for (bool b : boolList)
        {
            cout << b;
        }
    }
    cout << endl;

    cout << "ShannonFano Decode" << endl;
    ShannonFano decodeSF;
    decodeSF.createCodeTree();
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
class Universal
{
public:
    list<bool> Gamma(int x);
    list<bool> Delta(int x);
    void Encode(string str);
    string Decode(string str);
    void CreateCodeTree();
    static void Show();
    map<char, list<bool>> codeList;
    vector<pair<int, char>> countVector;
    Tree codeTree;
private:

};

void Universal::Show()
{
    cout << "Universal Encode" << endl;
    cout << "Please enter a string." << endl;
    string s;
    getline(cin, s);
    Universal u;
    u.Encode(s);
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
    for (char ch : s)
    {
        boolList = u.codeList.find(ch)->second;
        for (bool b : boolList)
        {
            cout << b;
        }
    }
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
    int n = (int)log2f(x);
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
    list<bool> binGamma = Gamma((int)log2f(x)+1);
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
void Universal::Encode( string str)
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


int main()
{
    //ShannonFano::show();
    Universal::Show();
    return 0;
}
