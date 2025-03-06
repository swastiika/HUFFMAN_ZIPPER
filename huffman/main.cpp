#include <iostream>
#include <fstream>
#include <chrono>
#include "huffman.cpp" 


long getFileSize(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return -1;
    }
    file.seekg(0, ios::end);
    long fileSize = file.tellg();
    file.close();

    return fileSize;
}

using namespace std;

int main() {
    cout << "Welcome to Huffman Zipper" << endl;
   
 
    string checker;
    cout << "Enter the mode: compress or decompress? : ";
    cin >> checker;

    if (checker == "compress") {
        auto startCompression = chrono::high_resolution_clock::now();
         compressFile("encode.txt","huffman.txt");
        auto endCompression = chrono::high_resolution_clock::now();
        chrono::duration<double> compressionTime = endCompression - startCompression;
        cout << "Compression Time: " << compressionTime.count() << " seconds" << endl;
        cout << "The size of the original file: " << getFileSize("encode.txt") << " bytes" << endl;
        cout << "The size of the compressed file: " << getFileSize("huffman.txt") << " bytes" << endl;
        long originalSize = getFileSize("encode.txt");
        long compressedSize = getFileSize("huffman.txt");
        if (originalSize != -1 && compressedSize != -1) {
            double compressionRatio = (double)originalSize / compressedSize;
            cout << "Compression Ratio: " << compressionRatio << endl;
        }
    }
    else if (checker == "decompress") {

        auto startDecompression = chrono::high_resolution_clock::now();
           decompress("huffman.txt","decode.txt");
        auto endDecompression = chrono::high_resolution_clock::now();
        chrono::duration<double> decompressionTime = endDecompression - startDecompression;
        cout << "Decompression Time: " << decompressionTime.count() << " seconds" << endl;
    }

    return 0;
}
