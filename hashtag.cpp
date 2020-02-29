#include<iostream>
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
  Node *max;
  void insert(string name,int frequency);
  // Node* remove_max(Node *head);
  // void meld(Node* head1, Node* head2);
  // void increase_key(Node *node, int value);
  void print_heap();
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
  if(heap_node->frequency > head->frequency){
    head = heap_node;
  }
}

void FibonacciHeap :: print_heap(){
  Node *temp = head;
  if(temp == nullptr){
    cout<<"Empty"<<endl;
    return;
  }
  while(temp->right != head){
    cout<<"("<<temp->name<<" : "<<temp->frequency<<") -> ";
    temp = temp->right;
  }
  cout<<"("<<temp->name<<" : "<<temp->frequency<<")";
  // cout<<"last element is: "<<temp->name<<" and its right pointer is: "<<temp->right->name<<endl;
  // cout<<"first element is: "<<head->name<<" and its left pointer is: "<<head->left->name<<endl;
}

void FibonacciHeap :: remove_max(Node *head){

}

int main(int argc, char const *argv[]) {
  FibonacciHeap *object = new FibonacciHeap();
  object->insert("sunday",5);
  object->insert("saturday",3);
  object->insert("monday",7);
  object->insert("reading",2);
  object->insert("play",9);
  object->print_heap();
  return 0;
}
