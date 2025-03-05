#include<unordered_map>
#include<iostream>
#include<fstream>
using namespace std;
void encode(unordered_map<char,string> &encodes){
       ofstream outputFile("huffman.txt");
       ifstream inputfile("encode.txt");
       char ch;

       if(!outputFile){
        cout<<"error opening the huffman file"<<endl;
        return;
       }
       while(inputfile.get(ch)){
        outputFile<<encodes[ch];
       }



}