#include <iostream>
#include <queue>
#include <unordered_map>
#include "MinheapNode.h"
#include "encode.cpp"
#include "decode.cpp"
using namespace std;
unordered_map<string, char> decodes;
unordered_map<char, string> encodes;

void display(MinHeapNode *root, string str) {
    if (!root) return;

    if (root->data != '$') {
        cout << root->data << ":" << str << endl;
        encodes[root->data] = str;  
        decodes[str] = root->data;  
    }

    display(root->left, str + "0");
    display(root->right, str + "1");
}

void heap(unordered_map<char, int>& del) {
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minheap;
    MinHeapNode *left, *right, *top;
    for (auto it = del.begin(); it != del.end(); ++it) {
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
    display(minheap.top(), "");
    encode(encodes);
    decode(decodes);
    

}

