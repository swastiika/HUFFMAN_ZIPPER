#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include "MinheapNode.h"
#include<bitset>
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
    ofstream outputFileStream("huffman.txt"); 
    ifstream inputFileStream("encode.txt");
    char ch;
    string encodedString = "";

    while (inputFileStream.get(ch)) {
        encodedString += encodes[ch]; 
    }
    int padding = 8 - (encodedString.size() % 8); 
    encodedString.append(padding, '0');
    outputFileStream.put(padding); 
    for (size_t i = 0; i < encodedString.size(); i += 8) {
        bitset<8> bits(encodedString.substr(i, 8));  
        outputFileStream.put(static_cast<char>(bits.to_ulong()));  
    }

    inputFileStream.close();
    outputFileStream.close();
}


void decodeFile() {
    ifstream inputFileStream("huffman.txt", ios::binary); 
    ofstream outputFileStream("decode.txt");
    int padding;
    inputFileStream.get(reinterpret_cast<char&>(padding));  
    string encodedString = "";
    char byte;
    while (inputFileStream.get(byte)) {
        bitset<8> bits(byte);  
        encodedString += bits.to_string();  
    }
    encodedString = encodedString.substr(0, encodedString.size() - padding);
    string currentCode = "";
    for (size_t i = 0; i < encodedString.size(); ++i) {
        currentCode += encodedString[i];
        if (decodes.find(currentCode) != decodes.end()) { 
            outputFileStream.put(decodes[currentCode]);  
            currentCode = "";  
        }
    }

    inputFileStream.close();
    outputFileStream.close();
}
