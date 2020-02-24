#include<iostream>
using namespace std;

//Max-Fibonacci Heap Structure
struct Node{
  int frequency;      //Frequency of HashTags
  string name;        //Name of HashTags
  int degree;         //Degree of Node
  bool childCut;      //ChildCut value of Node
  Node *parent;       //Parernt of Node
  Node *children;     //Childrens of Node
  Node *left;         //Left Sibling Nodes
  Node *right;        //Right Sibling Nodes
}*head;



int int main(int argc, char const *argv[]) {
  return 0;
}
