#include<iostream>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include<vector>
#include <fstream>
using namespace std;

//Max-Fibonacci Heap Structure
struct Node{
  int frequency;          //Frequency of HashTags
  string name;            //Name of HashTags
  int degree;             //Degree of Node
  bool childCut;          //ChildCut value of Node
  Node *parent;           //Parernt of Node
  Node *children;         //Childrens of Node
  Node *left;             //Left Sibling Nodes
  Node *right;            //Right Sibling Nodes
  Node(string name,int frequency);
};

class FibonacciHeap{
public:
  Node *head;
  unordered_map<string,Node *> umap;
  Node *max;
  int numNodes=0;
  void insert(string name,int frequency);
  Node* extract_max();
  void remove_max();
  void pairwise_combine();
  void add_child(Node *Child,Node *Parent);
  void find_max();
  void set_parent(Node *start);
  void reinsert(string name,int frequency);
  void remove_child(Node *heap_node,Node *parent_node);
  void cascade_cut(Node *parent_node);
  void increase_key(string hashtag, int value);
  void print_heap();
  vector<string> top_k_hashtags(long k);
};


Node :: Node(string name, int frequency){
  this->frequency = frequency;
  this->name = name;
  this->degree = 0;
  this->childCut = false;
  this->parent = nullptr;
  this->children = nullptr;
  this->left = nullptr;
  this->right = nullptr;
}

vector<string> FibonacciHeap :: top_k_hashtags(long k){
  vector<string> k_freq;
  vector<Node*> k_freq_nodes;
  for (int i = 0; i < k; i++) {
    Node *max_node = extract_max();
    //cout<<"max is : "<<max_node->name<<endl;
    k_freq.push_back(max_node->name);
    k_freq_nodes.push_back(max_node);
  }
  for(auto a:k_freq_nodes){
    reinsert(a->name,a->frequency);
  }
  return k_freq;
}

void FibonacciHeap :: insert(string name,int frequency){
  Node* heap_node = new Node(name,frequency);
  if(head == nullptr){
    head = heap_node;
  }
  else{
    Node* temp = head->left;
    if(temp == nullptr){
      head->right = heap_node;
      head->left = heap_node;
      heap_node->left = head;
      heap_node->right = head;
    }
    else{
      head->left = heap_node;
      temp->right = heap_node;
      heap_node->left = temp;
      heap_node ->right = head;
    }
  }
  umap.insert({name,heap_node});
  if(heap_node->frequency > head->frequency){
    head = heap_node;
  }
  numNodes++;
}

void FibonacciHeap :: add_child(Node *m, Node *n){
  m->right->left = m->left;
  m->left->right = m->right;
  m->parent = n;
  if(n->children == nullptr){
    n->children = m;
    m->left = m;
    m->right = m;
  }
  else{
    m->left = n->children;
    m->right = n->children->right;
    n->children->right->left = m;
    n->children->right = m;
  }
  m->childCut = false;
  n->degree = n->degree + 1;
}

void FibonacciHeap :: reinsert(string name, int frequency){
  if(umap.count(name)>0){
    increase_key(name,frequency);
  }else{
    insert(name,frequency);
  }
  //print_heap();
}

void FibonacciHeap :: print_heap(){
  Node *temp = head;
  if(temp == nullptr){
    return;
  }
  while(temp->right != head){
    cout<<"("<<temp->name<<" : "<<temp->frequency<<" : "<<temp->degree<<") -> ";
    temp = temp->right;
  }
  cout<<"("<<temp->name<<" : "<<temp->frequency<<" : "<<temp->degree<<")";
}

Node* FibonacciHeap :: extract_max(){
  //cout<<"extracting max"<<endl;
  if(head == nullptr){
    cout<<"extract max failed"<<endl;
  }
  Node *max = head;
  umap.erase(max->name);
  remove_max();
  numNodes--;
  return max;
}

void FibonacciHeap :: remove_max(){
  Node *temp = head;
  if(head == nullptr){
    return;
  }
  if(head->children == nullptr){
    //cout<<"case 1"<<endl;
    Node *temp = head->right;
    temp->left = head->left;
    temp->left->right = temp;
    head = temp;
  }
  else{
    //cout<<"case 2"<<endl;
    Node *temp = head;
    Node *head_children = head->children;
    set_parent(head_children);
    head_children->left->right = temp->right;
    temp->right->left = head_children->left;
    head_children->left = temp->left;
    temp->left->right = head_children;
    head = head->children;
  }
   pairwise_combine();
   find_max();
}

void FibonacciHeap :: set_parent(Node *start){
  Node *temp = start;
  while(temp->right != start){
    temp->parent = nullptr;
    temp = temp->right;
  }
  temp->parent = nullptr;
}

void FibonacciHeap :: pairwise_combine(){
  //cout<<"pairwise combining"<<endl;
  if(head == nullptr){
    return;
  }
  int nodes = numNodes;
  bool flag = false;
  Node *temp = head;
  Node *p;
  while(temp->right != head){
    p = temp->right;
    //cout<<"Checking "<<temp->name<<" "<<p->name<<endl;
    while(p->degree != temp->degree && p != head){
      //cout<<"temp->name "<<temp->name<<" p->name : "<<p->name<<endl;
      p = p->right;
    }
    if(temp->degree == p->degree){
      if(temp->frequency >= p->frequency && temp->name != p->name){
        p->left->right = p->right;
        p->right->left = p->left;
        add_child(p,temp);
        //cout<<"Making "<<p->name<<" child of "<<temp->name<<" with parent having degree: "<<temp->degree<<endl;
        numNodes--;
        flag = true;
      }
      else if(temp->frequency <= p->frequency && temp->name != p->name){
        temp->right->left = temp->left;
        temp->left->right = temp->right;
        add_child(temp,p);
        //cout<<"**Making "<<temp->name<<" child of "<<p->name<<" with parent having degree: "<<p->degree<<endl;
        flag = true;
        //cout << "p is "<< p->name <<endl;
        head = p;
        p = nullptr;
      }
  }
    if(flag){
      temp = head;
    }
    else{
      temp = temp->right;
    }
    flag = false;
  }
  //cout<<"combining done"<<endl;
}

void FibonacciHeap :: find_max(){
  Node *temp = head;
  Node *temp_max = new Node("dummy",-1)  ;
  int max = temp_max->frequency;
  while(temp->right != head){
    if(temp->frequency > max){
      max = temp->frequency;
      temp_max = temp;
    }
    temp = temp->right;
  }
  if(temp->frequency > max){
    temp_max = temp;
  }
  if(temp_max == nullptr){
    return;
  }
  head = temp_max;
}

void FibonacciHeap :: increase_key(string hashtag,int value){
  Node *heap_node = umap.at(hashtag);
  heap_node->frequency += value;
  Node *parent_node = heap_node->parent;
  if(parent_node != nullptr){
    if(heap_node->frequency > parent_node->frequency){
      remove_child(heap_node,parent_node);
      cascade_cut(parent_node);
    }
  }
  if(heap_node->frequency > head->frequency){
    head = heap_node;
  }
}

void FibonacciHeap :: remove_child(Node *heap_node,Node *parent_node){
  heap_node->left->right = heap_node->right;
  heap_node->right->left = heap_node->left;
  parent_node->degree -= 1;
  if(parent_node->children == heap_node){
    parent_node->children = heap_node->right;
  }
  if(parent_node->degree == 0){
    parent_node->children = nullptr;
  }

  heap_node->left = head;
  heap_node->right = head->right;
  head->right->left = heap_node;
  head->right = heap_node;
  heap_node->childCut = false;
  heap_node->parent = nullptr;
}

void FibonacciHeap :: cascade_cut(Node *parent_node){
  //cout<<"cascading"<<endl;
  Node *parent_ptr = parent_node->parent;
  if(parent_ptr != nullptr){
    if(parent_node->childCut == false){
      parent_node->childCut = true;
    }
    else{
      remove_child(parent_node,parent_ptr);
      cascade_cut(parent_ptr);
    }
  }
}

int main(int argc, char const *argv[]) {
  FibonacciHeap *object = new FibonacciHeap();
  ifstream file;
  file.open("input.txt");
  ofstream myfile;
  myfile.open ("output.txt");
  string line,name;
  while(getline(file,line)){
    if(line[0] == '#'){
      name = line.substr(1,string::npos);
      string hashtag = name.substr(0,name.find(' '));
      int freq = stoi(name.substr(name.find(' '),string::npos),nullptr,10);
      object->reinsert(hashtag,freq);
    }
    else if((line[0]>'0') && (line[0]<='9')){
      int top_K = stoi(line,nullptr,10);
      vector<string> output = object->top_k_hashtags(top_K);
      string text="";
      for (int i = 0; i < output.size(); i++) {
        text = text + output[i] + ",";
      }
      myfile << text.substr(0,text.size()-1) << endl;
    }
    else if (line == "stop"){
      break;
    }
  }
  myfile.close();
  return 0;
}
