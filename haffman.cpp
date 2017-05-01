#include <stdio.h>
#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <vector>
using namespace std;

struct Node
{
  int pos;//вероятность символа
  char c;//cам символ
  Node *left;//указатель на левого сына(для дерева)
  Node *right;//указатель на правого сына(для дерева)
};

struct Code
{
  char smbl;//символ
  vector<int> cd;//его код
};

struct CompNode
{
  bool operator() (Node* l, Node* r)
  {
    return (l->pos<r->pos);
  }
};

void print(Node* root,int k=0)
{
  if(root!=NULL)
  {
    print(root->left,k+3);
    for(int i=0;i<k;i++)
    {
      cout<<" ";
    }
    if (root->c) cout<<root->pos<<" ("<<root->c<<")"<<endl;
    else cout<<root->pos<<endl;
    print(root->right,k+3);
  }
}

void CreateList(list <Node*>& t,int* a)
{
  Node *p=new Node;
  for(int j=0;j<256;j++)
  {
    if (a[j]!=0)
    {
      Node *p=new Node;
      p->pos=a[j];
      p->c=j;
      p->left=p->right=NULL;
      t.push_back(p);
    }
  }
}

void CreateTree(list<Node*>& t)
{
  while(t.size()!=1)
  {
    t.sort(CompNode());
    Node *ls=t.front();
    t.pop_front();
    Node *rs=t.front();
    t.pop_front();
    Node *p=new Node;
    p->pos=ls->pos+rs->pos;
    p->left=ls;
    p->right=rs;
    t.push_back(p);
  }
  Node *root=t.front();
  print(root);
}

void CreateCode(Node* root, char b, Code& code, ofstream& codes)
{
  if(root->left!=NULL)
  {
    code.cd.push_back(0);
    CreateCode(root->left,b,code,codes);
  }
  if(root->right!=NULL)
  {
    code.cd.push_back(1);
    CreateCode(root->right,b,code,codes);
  }
  if ((root->left==NULL) && (root->right==NULL) && (root->c==b))
  {
    cout<<b<<"     ";
   for(int i=0; i<code.cd.size(); i++)
    {
      cout<<code.cd[i];
      codes<<code.cd[i];
    }
    cout<<endl;

  }
    code.cd.pop_back();
}

void encode(ifstream& in, Node* root,ofstream& out)
{
  char b;
  Code code;
  in.get(b);
  while(!(in.eof()))
  {
    CreateCode(root,b,code,out);
    in.get(b);
    code.cd.clear();
  }
}

void decode(ifstream& in,Node* root, Node* startroot,char x,ofstream& out)
{
  if(x != EOF)
  {
    if((x=='0') && (root->left!=NULL))
    {
      x=in.get();
      decode(in,root->left,startroot,x,out);
    }
    if((x=='1') && (root->right!=NULL))
    {
      x=in.get();
      decode(in,root->right,startroot,x,out);
    }
    if ((root->left==NULL) && (root->right==NULL))
    {
      out<<root->c;
      decode(in,startroot,startroot,x,out);
      //сout<<root->c;
    }
  }
}

void CreateArray(int a[], ifstream& in)
{
  int i=0;
  char b;
  for (i=0;i<256;i++)
  {
    a[i]=0;
  }
  for(i=0;!in.eof();i++)
  {
    in.get(b);
    a[b]++;
  }
}


int main()
{
  int j;
  int p[256]; //массив с вероятностями символов
  list <Node*> tree;
  Code* code;
  ifstream in("in.txt");
  CreateArray(p,in);
  in.close();
  CreateList(tree,p);
  int size=tree.size();//количество различных символов в тексте
  CreateTree(tree);
  Node *root=tree.front();
  ifstream inn("in.txt");
  ofstream out("out.txt");
  encode(inn,root,out);//кодирование файла
  out.close();
  inn.close();
  ifstream oout("out.txt");//открываем на чтение файл с кодом
  ofstream in2("in2.txt");
  char x=oout.get();
  decode(oout,root,root,x,in2);
  in2.close();
  oout.close();
}
