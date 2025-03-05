#include<iostream>
#include<fstream>
#include<unordered_map>
using namespace std;
void decode(unordered_map<string,char> &decode){
    ifstream decFile("huffman.txt");
    ofstream decodedfile("decode.txt");
    char ch;
    string st="";
    while(decFile.get(ch)){
        st += ch;
        if(decode.find(st)!=decode.end()){
            decodedfile<<decode[st];
             st = "";
        }

    }
    return;
}