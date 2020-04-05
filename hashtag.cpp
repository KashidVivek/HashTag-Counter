#include<iostream>
#include<algorithm>
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

//class definition of Fibonacci heaps and All heap functions
class FibonacciHeap{
public:
  Node *head;                                                 //max pointer in the FHeap
  unordered_map<string,Node *> umap;                          //Hashmap(Key:Hashtag,Value:Frequency of hashtag)
  Node *max;        
  int numNodes=0;                                             //Number of nodes in FHeap
  void insert(string name,int frequency);                     //Insert into FHeap
  Node* extract_max();                                        //Extract node with max frequency
  void remove_max();                                          //Removing Max Node
  void pairwise_combine();                                    //Pairwise Combine
  void add_child(Node *Child,Node *Parent);                   //Adding a child node to Parent Specified
  void find_max();                                            //Finding max after Pairwise Combine
  void set_parent(Node *start);                               //Setting Parent of a child Node
  void reinsert(string name,int frequency);                   //Reinsert into hashmap
  void remove_child(Node *heap_node,Node *parent_node);       //Remove Child
  void cascade_cut(Node *parent_node);                        //Cascading Cut
  void increase_key(string hashtag, int value);               //Increase Key
  void print_heap();                                          //printing Heap
  vector<string> top_k_hashtags(long k);                      //Getting top K Hashtags
};

//Fibonacci Heap Node Structure
Node :: Node(string name, int frequency){ 
  this->frequency = frequency;                 //Frequency Count
  this->name = name;                           //Name of Hashtag
  this->degree = 0;                            //Degree
  this->childCut = false;                      //Boolean value of ChildCut
  this->parent = nullptr;                      //Parent Node
  this->children = nullptr;                    //Ptr to Children
  this->left = nullptr;                        //Ptr to left Node
  this->right = nullptr;                       //Ptr to right Node
}

/**
 * Getting Top K hashtags.
 *
 * @param values Long input of K.
 * @return Top K hashtags names in a vector.
 */
vector<string> FibonacciHeap :: top_k_hashtags(long k){
  vector<string> k_freq;
  vector<Node*> k_freq_nodes;
  for (int i = 0; i < k; i++) {
    Node *max_node = extract_max();
    k_freq.push_back(max_node->name);
    k_freq_nodes.push_back(max_node);
  }
  for(auto a:k_freq_nodes){
    reinsert(a->name,a->frequency);
  }
  return k_freq;
}

/**
 * Inserting in Fibonacci Heap.
 *
 * @param values Name of Hashtag and Integer Frequency.
 * @return void.
 */
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

/**
 * Adding a child to a parent node in Fibonacci Heap.
 *
 * @param values Child Node, Parent Node.
 * @return void.
 */
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

/**
 * Reinserting in Fibonacci Heap.
 *
 * @param values Name of Hashtag and Integer Frequency.
 * @return void.
 */
void FibonacciHeap :: reinsert(string name, int frequency){
  if(umap.count(name)>0){
    increase_key(name,frequency);
  }else{
    insert(name,frequency);
  }
}

/**
 * Inserting in Fibonacci Heap.
 *
 * @param values NULL.
 * @return void.
 */
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


/**
 * Extracting max node in Fibonacci Heap.
 *
 * @param values NULL.
 * @return void.
 */
Node* FibonacciHeap :: extract_max(){
  if(head == nullptr){
    cout<<"extract max failed"<<endl;
  }
  Node *max = head;
  umap.erase(max->name);
  remove_max();
  numNodes--;
  return max;
}


/**
 * Removing the max node in Fibonacci Heap.
 *
 * @param values NULL.
 * @return void.
 */
void FibonacciHeap :: remove_max(){
  Node *temp = head;
  if(head == nullptr){
    return;
  }
  if(head->children == nullptr){              //Case 1: Max node does not have children.
    Node *temp = head->right;
    temp->left = head->left;
    temp->left->right = temp;
    head = temp;
  }
  else{                                       //Case 2: Max node does have children.
    Node *temp = head;
    Node *head_children = head->children;
    set_parent(head_children);                //Set parent of the New Head as NULL. 
    head_children->left->right = temp->right;
    temp->right->left = head_children->left;
    head_children->left = temp->left;
    temp->left->right = head_children;
    head = head->children;
  }
   pairwise_combine();                       //Pairwisse Combine after RemoveMax Operation.
   find_max();                               //Finding Next max Node in FHeap and making it head.
}


/**
 * Setting Parent Fields in Fibonacci Heap.
 *
 * @param values NULL.
 * @return void.
 */
void FibonacciHeap :: set_parent(Node *start){
  Node *temp = start;
  while(temp->right != start){
    temp->parent = nullptr;
    temp = temp->right;
  }
  temp->parent = nullptr;
}


/**
 * Pairwise Combine in Fibonacci Heap.
 *
 * @param values NULL.
 * @return void.
 */
void FibonacciHeap :: pairwise_combine(){
  if(head == nullptr){
    return;
  }
  int nodes = numNodes;
  bool flag = false;
  Node *temp = head;
  Node *p;
  while(temp->right != head){
    p = temp->right;
    while(p->degree != temp->degree && p != head){
      p = p->right;
    }
    if(temp->degree == p->degree){
      if(temp->frequency >= p->frequency && temp->name != p->name){
        p->left->right = p->right;
        p->right->left = p->left;
        add_child(p,temp);
        numNodes--;
        flag = true;
      }
      else if(temp->frequency <= p->frequency && temp->name != p->name){
        temp->right->left = temp->left;
        temp->left->right = temp->right;
        add_child(temp,p);
        flag = true;
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
}

/**
 * Find the max node in Fibonacci Heap.
 *
 * @param values NULL.
 * @return void.
 */
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

/**
 * Incresing the value of a node in Fibonacci Heap.
 *
 * @param values String Name of Hashtag,Int amount to increase.
 * @return void.
 */
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


/**
 * Removing the child node in Fibonacci Heap.
 *
 * @param values Child Node, Parent Node.
 * @return void.
 */
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


/**
 * Cascading Cut.
 *
 * @param values Parent Node.
 * @return void.
 */
void FibonacciHeap :: cascade_cut(Node *parent_node){
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
  cout<<argc<<endl;
  //cout<<argv[1]<<" "<<argv[2]<<endl;
  FibonacciHeap *object = new FibonacciHeap();                                       //Object Of Class FibonacciHeap
  ifstream file;
  string input_file = argv[1];
  ofstream myfile;
  if(argc>2){
    string output_file = argv[2];
    myfile.open (output_file);
  }
  file.open(input_file);
  // ofstream myfile;
  // myfile.open (output_file);
  string line,name;
  while(getline(file,line)){
    if(line[0] == '#'){                                                             //Recognizing Hashtags
      name = line.substr(1,string::npos);
      string hashtag = name.substr(0,name.find(' '));
      int freq = stoi(name.substr(name.find(' '),string::npos),nullptr,10);
      object->reinsert(hashtag,freq);
    }
    else if((line[0]>'0') && (line[0]<='9')){                                       //Printing to Output file Top K hashtags
      int top_K = stoi(line,nullptr,10);
      vector<string> output = object->top_k_hashtags(top_K);
      string text="";
      for (int i = 0; i < output.size(); i++) {
        text = text + output[i] + ",";
      }
      if(argc == 2){
        cout<<text.substr(0,text.size()-1) << endl;
      }
      else{
        myfile << text.substr(0,text.size()-1) << endl;
      }
    }
    else if (line == "stop"){                                                       //Stopping Condition
      break;
    }
  }
  myfile.close();
  return 0;
}
