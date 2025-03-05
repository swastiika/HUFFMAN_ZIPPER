#include <iostream>
#include <queue>
#include "MinheapNode.h"
#include<unordered_map>
using namespace std;
void display(MinHeapNode *root,string str){
    if(!root) return;
    if(root->data!='$') cout<<root->data<<":"<<str;
    display(root->left,str+"0");
    display(root->right,str+"1");
    return;

}
void heap(unordered_map<char,int> &del) {
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minheap;
    MinHeapNode *left, *right, *top;
    for(auto it=del.begin(); it!=del.end(); ++it ) {
        minheap.push(new MinHeapNode(it->first, it->second));
    }
    while (minheap.size() != 1) {
        left = minheap.top();
        minheap.pop();
        right = minheap.top();
        minheap.pop();
        top = new MinHeapNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minheap.push(top);
    }
   display(minheap.top(),"");
}
