#include <queue>
#include <iostream>

#define F(a,b,c)    for(register int a=b; a<=c; a++)

using namespace std;

class Trie {
public: 
    Trie *  child[26]; 
    int end; 

    Trie   () {
        F(i, 0, 25) {
            child[i] = 0;
        }
    }
};


class Node {
public:
    int val; 
    Node* mParent; 
    Node* mChildroot; 
    Node* mSibNext; 

    
};

void addChild(Node* parent, Node* child) {
    child->mSibNext = parent->mChildroot; 
    child->mParent = parent; 
    parent->mChildroot = child; 
}


struct comp {
    bool operator() (Node a, Node b) {
        return a.val < b.val;  
    }
  };

priority_queue <Node , vector<Node>, comp > q1; 

void Init() {
    while (!q1.empty())
    {
        q1.pop(); 
    }
}


int main()
{

    Node a = { 7,0,0,0 }; 
    Node b = { 8,0,0,0 };
    Node c = { 9,0,0,0 };

    q1.push(a);
    q1.push(b);
    q1.push(c);

    while (!q1.empty()) {
        Node t = q1.top(); 
        printf("val = %d ", t.val); 
        q1.pop(); 
    }

}