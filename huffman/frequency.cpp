#include<unordered_map>
#include<iostream>
#include<fstream>
#include<sstream>
#include "huffman.cpp"
using namespace std;
void frequencyCount(){

 unordered_map<char,int> freq;
 ifstream input("encode.txt");
 if(!input) {
    cout<<"error openening the file"<<endl;
    return;
 }
 char ch;
 while(input.get(ch)){
    freq[ch]++;
 }
input.close();
 heap(freq);

}
