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
  int numNodes=0;
  void insert(string name,int frequency);
  Node* extract_max();
  void remove_max();
  void pairwise_combine();
  void add_child(Node *Child,Node *Parent);
  void find_max();
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
  numNodes++;
}

void FibonacciHeap :: add_child(Node *m, Node *n){
  //cout<<"Adding child"<<endl;
  m->right->left = m->left;
  m->left->right = m->right;
  m->parent = n;
  if(n->children == nullptr){
    //cout<<"NULL"<<endl;
    n->children = m;
    m->left = m;
    m->right = m;
  }
  else{
    //cout<<"Here"<<endl;
    m->left = n->children;
    m->right = n->children->right;
    n->children->right->left = m;
    n->children->right = m;
  }
  m->childCut = false;
  n->degree = n->degree + 1;
}

void FibonacciHeap :: print_heap(){
  Node *temp = head;
  if(temp == nullptr){
    //cout<<"Empty"<<endl;
    return;
  }
  while(temp->right != head){
    cout<<"("<<temp->name<<" : "<<temp->frequency<<" : "<<temp->degree<<") -> ";
    temp = temp->right;
  }
  cout<<"("<<temp->name<<" : "<<temp->frequency<<" : "<<temp->degree<<")";
}

Node* FibonacciHeap :: extract_max(){
  //get head Node
  //remove head node and insert its children in doubly circular list
  //do pairwise combine
  Node *max = head;
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
    Node *temp = head->right;
    temp->left = head->left;
    temp->left->right = temp;
    head = temp;
  }
  else{
    Node *temp = head;
    Node *head_children = head->children;
    // temp->right->left = head_children->left;
    // temp->left->right = head->children;
    // head->children->left->right = temp->right;
    head_children->left->right = temp->right;
    temp->right->left = head_children->left;
    head_children->left = temp->left;
    // if(temp->right->right == head){
    //   temp->right->right = head_children;
    // }
    temp->left->right = head_children;
    head = head->children;
    //cout<<"printing:";
    //print_heap();
  }
   pairwise_combine();
   find_max();
}

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
    //cout<<"Checking "<<temp->name<<" "<<p->name<<endl;
    //cout<<"Matched degree of:("<<temp->name<<":"<<temp->degree<<") with ";
    while(p->degree != temp->degree && p != head){
      //cout<<"temp->name "<<temp->name<<" p->name : "<<p->name<<endl;
      p = p->right;
    }
    if(temp->degree == p->degree){
      //cout<<"----"<<endl;
      if(temp->frequency > p->frequency && temp->name != p->name){
        //temp->right = p->right;
        // cout<<"temp->name "<<temp->name<<" p->name : "<<p->name<<endl;
        // cout<<"temp->left->left "<<temp->left->left->name<<endl;
        // cout<<"temp->right->left "<<temp->right->left->name<<endl;
        // cout<<"temp->left "<<temp->left->name<<endl;
        // cout<<"temp->right "<<temp->right->name<<endl;
        // cout<<"temp->left->right "<<temp->left->right->name<<endl;
        p->left->right = p->right;
        p->right->left = p->left;
        add_child(p,temp);
        //cout<<"Making "<<p->name<<" child of "<<temp->name<<" with parent having degree: "<<temp->degree<<endl;
        numNodes--;
        flag = true;
        //temp = head;
      }
      else if(temp->frequency < p->frequency && temp->name != p->name){
        //cout<<"temp->name "<<temp->name<<" p->name : "<<p->name<<endl;
        //p->left = temp->left;
        //temp->left->right = p;
        //print_heap();
        // cout<<"temp->left->left "<<temp->left->left->name<<endl;
        // cout<<"temp->right->left "<<temp->right->left->name<<endl;
        // cout<<"temp->left "<<temp->left->name<<endl;
        // cout<<"temp->right "<<temp->right->name<<endl;
        // cout<<"temp->left->right "<<temp->left->right->name<<endl;
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
      //cout << "head after combine " << head->name <<endl;
      //cout << "temp is" << temp->name <<endl;
      temp = head;
      //cout << "new head " << temp->name <<endl;
    }
    else{
      temp = temp->right;
    }
    flag = false;
  }

}

void FibonacciHeap :: find_max(){
  Node *temp = head;
  //cout<<"Current max: "<<temp->name<<endl;
  Node *temp_max = new Node("dummy",-1)  ;
  //cout<<"temp_max: "<<temp_max->frequency<<endl;
  int max = temp_max->frequency;
  while(temp->right != head){
    //cout<<"checking "<<temp->name<<endl;
    if(temp->frequency > max){
      //cout<<"changing max to: "<<temp->name<<endl;
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
  //cout<<"Max node: "<<temp_max->name<<endl;
  head = temp_max;
}

int main(int argc, char const *argv[]) {
  FibonacciHeap *object = new FibonacciHeap();
  Node *head;
  object->insert("sunday",3);
  object->insert("saturday",9);
  object->insert("monday",4);
  object->insert("reading",8);
  object->insert("play",1);
  object->insert("sleep",11);
  object->insert("vivek",6);
  object->print_heap();
  cout<<endl;
  cout<<"---------------------------------"<<endl;

  Node *maximum = object->extract_max();
  cout<<"Max removed:"<<maximum->name<<endl;
  object->print_heap();
  cout<<endl;
  cout<<"---------------------------------"<<endl;

  Node *maximum1 = object->extract_max();
  cout<<"Max removed:"<<maximum1->name<<endl;
  object->print_heap();
  cout<<endl;
  cout<<"---------------------------------"<<endl;

  Node *maximum2 = object->extract_max();
  cout<<"Max removed:"<<maximum2->name<<endl;
  object->print_heap();
  cout<<endl;
  cout<<"---------------------------------"<<endl;

  Node *maximum3 = object->extract_max();
  cout<<"Max removed:"<<maximum3->name<<endl;
  object->print_heap();
  cout<<endl;
  cout<<"---------------------------------"<<endl;

  Node *maximum4 = object->extract_max();
  cout<<"Max removed:"<<maximum4->name<<endl;
  object->print_heap();
  cout<<endl;

  return 0;
}
