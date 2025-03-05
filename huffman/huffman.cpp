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

    // Display the Huffman codes for each character
    display(minheap.top(), "");

    // Open the file to write the encoded data
    ofstream outputFileStream("huffman.txt"); // Opening in binary mode

    // Open the input file and read the content
    ifstream inputFileStream("encode.txt");
    char ch;
    string encodedString = "";

    while (inputFileStream.get(ch)) {
        encodedString += encodes[ch];  // Append the Huffman code for each character
    }

    // Write the encoded string to the output file
    // Convert the encoded string to binary and write it in 8-bit chunks
    int padding = 8 - (encodedString.size() % 8);  // Calculate the padding required to make it a multiple of 8 bits

    // Add padding bits at the end if necessary
    encodedString.append(padding, '0');

    // Write the padding value at the start of the encoded file for decoding purposes
    outputFileStream.put(padding);  // First byte stores the padding

    // Write the encoded data in 8-bit chunks
    for (size_t i = 0; i < encodedString.size(); i += 8) {
        bitset<8> bits(encodedString.substr(i, 8));  // Get 8 bits
        outputFileStream.put(static_cast<char>(bits.to_ulong()));  // Write the byte to the file
    }

    inputFileStream.close();
    outputFileStream.close();
}


void decodeFile() {
    // Open the file containing the encoded data
    ifstream inputFileStream("huffman.txt", ios::binary); // Opening in binary mode
    ofstream outputFileStream("decode.txt");

    // Read the padding value from the first byte
    int padding;
    inputFileStream.get(reinterpret_cast<char&>(padding));  // Read the padding value

    // Read the rest of the file to get the encoded binary data
    string encodedString = "";
    char byte;
    while (inputFileStream.get(byte)) {
        bitset<8> bits(byte);  // Convert the byte into a bitset
        encodedString += bits.to_string();  // Convert bitset to a string and append
    }

    // Remove the padding bits from the end of the string
    encodedString = encodedString.substr(0, encodedString.size() - padding);

    // Use the decodes map to decode the binary string back to characters
    string currentCode = "";
    for (size_t i = 0; i < encodedString.size(); ++i) {
        currentCode += encodedString[i];
        if (decodes.find(currentCode) != decodes.end()) {  // If we found a matching Huffman code
            outputFileStream.put(decodes[currentCode]);  // Write the corresponding character to the output file
            currentCode = "";  // Reset the current code
        }
    }

    inputFileStream.close();
    outputFileStream.close();
}
