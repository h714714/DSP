#include<stdio.h>
#include<unordered_map>
#include<vector>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include "Ngram.h"
#include "VocabMap.h"
//bigram
double getBigramProb(Ngram& lm, const char *w1, const char *w2)
{
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return lm.wordProb( wid2, context);
}
void readMap(fstream& test, fstream& myMap, unordered_map<string, vector<string> >& table){
  string str;
  while(getline(myMap, str)){
    string word = str.substr(0, 2); 
    size_t index = str.find_first_not_of(" ", 3);
    vector<string> wordVec;
    while(index != string::npos){
      string tmp = str.substr(index, 2);
      wordVec.push_back(tmp);
      index = str.find_first_not_of(" ", index+2);
    }
    table[word] = wordVec;
    test<<word<<" : ";
    for(int i = 0; i< wordVec; i++)
      test<<wordVec[i]<<"  ";
    test<<endl;
  }
}


void viterbi(string str, unordered_map<string, vector<string> > MAP,Ngram& lm, fstream& result); //including trace back
{





}

int main(int argc, char *argv[])
{
  int ngram_order;
  Vocab voc;
  Ngram lm( voc, ngram_order );
  File lmFile( argv[7], "r" );
  lm.read(lmFile);
  lmFile.close();

  //read map and make table
  unordered_map<string, vector<string> > table;
  fstream myMap;
  myMap.open(argv[5], ios::in);
  fstream test;
  test.open("pythonTest", ios::out);
  readMap(myMap, &table);


  //open output file
  fstream result;
  result.open(argv[],"w");
  
  //read testdata
  string str;
  while(getline(test, str)){
    viterbi(str, MAP, lm, result); //including trace back
  }

}
