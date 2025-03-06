#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include "MinheapNode.h"
#include <bitset>
using namespace std;

unordered_map<char, string> codes;

// Function to display the Huffman codes and store them in the codes map
void display(MinHeapNode *root, string str) {
    if (!root) return;

    if (root->data != '$') {
        cout << root->data << ":" << str << endl;
        codes[root->data] = str;  
    }

    display(root->left, str + "0");
    display(root->right, str + "1");
}

// Count frequency of each character in the input file
unordered_map<char, int> frequencyCount(const string& inputfile) {
    unordered_map<char, int> freq;
    ifstream input(inputfile);
    
    if (!input) {
        cout << "Error opening the file" << endl;
        return freq;
    }

    char ch;
    while (input.get(ch)) {
        freq[ch]++;
    }

    input.close();
    return freq;
}

// Write the frequency table header to the output file
void writeHeader(ofstream& outfile, unordered_map<char, int>& freq) {
    // Write the number of unique characters first
    int numChars = freq.size();
    outfile.write(reinterpret_cast<const char*>(&numChars), sizeof(numChars));
    
    // Write each character and its frequency
    for (const auto& pair : freq) {
        outfile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
        outfile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }
}

// Create the Huffman tree
MinHeapNode* encodingTree(unordered_map<char, int>& freq) {
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minheap;
    MinHeapNode *left, *right, *top;
    
    for (auto it = freq.begin(); it != freq.end(); ++it) {
        minheap.push(new MinHeapNode(it->first, it->second));
    }
    
    // Handle the case of only one unique character
    if (minheap.size() == 1) {
        MinHeapNode* singleNode = minheap.top();
        top = new MinHeapNode('$', singleNode->freq);
        top->left = singleNode;
        return top;
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
    return minheap.top();
}

// Compress the input file
void compressFile(string inputfile, string outputfile) {
    // Get frequency of each character
    unordered_map<char, int> freq = frequencyCount(inputfile);
    if (freq.empty()) {
        cout << "Input file is empty or couldn't be read!" << endl;
        return;
    }
    
    // Build Huffman tree and generate codes
    MinHeapNode* root = encodingTree(freq);
    codes.clear();
    display(root, "");
    
    // Open output file
    ofstream outfile(outputfile, ios::binary);
    if (!outfile) {
        cout << "Error opening the output file!" << endl;
        return;
    }
    
    // Write header (frequency table)
    writeHeader(outfile, freq);
    
    // Read input file and convert to bit stream
    ifstream infile(inputfile);
    if (!infile) {
        cout << "Error opening the input file!" << endl;
        outfile.close();
        return;
    }
    
    string bitStream = "";
    char ch;
    while (infile.get(ch)) {
        bitStream += codes[ch];
    }
    infile.close();
    
    // Calculate padding
    int padding = 8 - (bitStream.size() % 8);
    if (padding == 8) padding = 0;
    
    // Write padding info
    outfile.write(reinterpret_cast<const char*>(&padding), sizeof(padding));
    
    // Add padding bits
    for (int i = 0; i < padding; i++) {
        bitStream += "0";
    }
    
    // Write compressed data
    for (size_t i = 0; i < bitStream.size(); i += 8) {
        bitset<8> byte(bitStream.substr(i, 8));
        unsigned char byteVal = static_cast<unsigned char>(byte.to_ulong());
        outfile.write(reinterpret_cast<const char*>(&byteVal), sizeof(byteVal));
    }
    
    outfile.close();
    cout << "Compression complete. File saved as " << outputfile << endl;
}

// Read the frequency table from the encoded file
unordered_map<char, int> readHeader(ifstream& infile) {
    unordered_map<char, int> freq;
    
    // Read the number of unique characters
    int numChars;
    infile.read(reinterpret_cast<char*>(&numChars), sizeof(numChars));
    
    // Read each character and its frequency
    for (int i = 0; i < numChars; i++) {
        char ch;
        int frequency;
        infile.read(reinterpret_cast<char*>(&ch), sizeof(ch));
        infile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));
        freq[ch] = frequency;
    }
    
    return freq;
}

// Extract Huffman codes from the tree
void extractCodes(MinHeapNode* root, string code, unordered_map<string, char>& reverseCodes) {
    if (!root) return;
    
    if (root->data != '$') {
        reverseCodes[code] = root->data;
    }
    
    extractCodes(root->left, code + "0", reverseCodes);
    extractCodes(root->right, code + "1", reverseCodes);
}

// Decompress the encoded file
void decompress(string encodeFile, string decodeFile) {
    // Open the encoded file
    ifstream infile(encodeFile, ios::binary);
    if (!infile) {
        cout << "Error opening the encoded file!" << endl;
        return;
    }
    
    // Read the frequency table
    unordered_map<char, int> freq = readHeader(infile);
    if (freq.empty()) {
        cout << "Error reading the header information!" << endl;
        infile.close();
        return;
    }
    
    // Reconstruct the Huffman tree
    MinHeapNode* root = encodingTree(freq);
    
    // Create a reverse mapping of codes to characters for faster decoding
    unordered_map<string, char> reverseCodes;
    extractCodes(root, "", reverseCodes);
    
    // Read the padding information
    int padding;
    infile.read(reinterpret_cast<char*>(&padding), sizeof(padding));
    
    // Read the remaining bytes from the file
    string bitStream = "";
    char byte;
    while (infile.read(&byte, sizeof(byte))) {
        bitStream += bitset<8>(static_cast<unsigned char>(byte)).to_string();
    }
    infile.close();
    
    // Remove padding bits
    if (padding > 0 && bitStream.size() >= padding) {
        bitStream = bitStream.substr(0, bitStream.size() - padding);
    }
    
    // Decode the bit stream
    ofstream outfile(decodeFile);
    if (!outfile) {
        cout << "Error opening the output file!" << endl;
        return;
    }
    
    string currentCode = "";
    for (char bit : bitStream) {
        currentCode += bit;
        if (reverseCodes.find(currentCode) != reverseCodes.end()) {
            outfile << reverseCodes[currentCode];
            currentCode = "";
        }
    }
    
    outfile.close();
    cout << "Decompression complete. File saved as " << decodeFile << endl;
}