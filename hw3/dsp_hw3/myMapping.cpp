#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<algorithm>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
using namespace std;
int main(int argc, char** argv)
{
  fstream file;
  file.open(argv[1], ios::in);
  string Str;
  int lineSize = 50;
  map<string, vector<string> > yintable;
  map<string, vector<string> >::iterator it;
  vector<string> wordtable;
  fstream output;
  output.open(argv[2], ios::out);

  while(getline(file, Str)){
    string word = Str.substr(0, 2);
    vector<string> yin;
    size_t index = Str.find_first_not_of(" ", 2);
    //push first yin
//if(strtol(Str.substr(index,2), NULL, 16) == A374) cout<<"fuck"<<endl;
    yin.push_back(Str.substr(index, 2));
    index = Str.find_first_of("/", index);
   
    while(index != string::npos){
      index ++;  //the word after '/', size of '/' is one
      string tmp_yin = Str.substr(index, 2);
      vector<string>::iterator it;
      it = find (yin.begin(), yin.end(), tmp_yin);
      if (it == yin.end()){
        yin.push_back(tmp_yin);
      }
      index = Str.find_first_of("/", index);
    }


    wordtable.push_back(word);
    for(int i= 0; i< yin.size(); i++){
      it = yintable.find(yin[i]);
      if (it != yintable.end())
        it->second.push_back(word);
      else{
        vector<string> tmp;
        tmp.push_back(word);
        yintable[yin[i]] = tmp;
      }
    }
  }
  for(int i =0; i< wordtable.size(); i++){
    output<<wordtable[i]<<"  "<<wordtable[i]<<endl; 
  }
  for(it=yintable.begin(); it!=yintable.end(); it++){
    output<<it->first<<"   ";
    for(int i =0 ; i< it->second.size(); i++){
      output<<it->second[i]<<" ";
    }
    output<<endl;
  }
  return 0;
}
