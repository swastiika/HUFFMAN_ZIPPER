#include <iostream>
#include <queue>
#include "MinheapNode.h"
using namespace std;

void heap(char data[], int freq[], int size) {
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minheap;
    MinHeapNode *left, *right, *top;
    for (int i = 0; i < size; ++i) {
        minheap.push(new MinHeapNode(data[i], freq[i]));
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
   
}
