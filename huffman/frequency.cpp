#include<unordered_map>
#include<iostream>
#include "huffman.cpp"
using namespace std;
void frequencyCount(string str){
 unordered_map<char,int> freq;
  for(int i=0; i<str.length(); i++){
    freq[str[i]]++;
  }
 heap(freq);

}
