#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<utility>
#include<map>
#include<cfloat>

#include "Ngram.h"

using namespace std;

void readMap(ifstream&, std::map<string, vector<string> >&);
double monogram(const string&, Ngram&, Vocab&);
double bigram(const string&, const string&, Ngram&, Vocab&);
void parseSen(string&, vector<string>&);
void viterbi(vector<string>&, Ngram&, Vocab&, map<string, vector<string> >&);

class Word{
   public:
      Word():_prob(-DBL_MAX),_preword(0),_word(" "){}
      Word(double p, size_t pre, string w) :_prob(p),_preword(pre),_word(w){}
      double getProb(){return _prob;}
      string getWord(){return _word;}
      size_t getPre(){return _preword;}

   private:
      string _word;
      double _prob;
      size_t _preword;
};

int main(int argc, char* argv[]){
   if(argc!=9){cerr<<"Check the input again!!!"; exit(1);}
   //std::map<string, vector<string> > ttt;
   int ngram_order = atoi(argv[8]);

   ifstream test(argv[2], ios::in);
   ifstream map(argv[4], ios::in);

   if(!test.is_open()){
      cerr<<"input test file cannnot be opened!"<<endl;
      return 0;
   }

   if(!map.is_open()){
      cerr<<"input map cannot be openned!"<<endl;
      return 0;
   }

   Vocab voc;
   Ngram lm(voc, ngram_order);
   File lmfile(argv[6], "r");
   lm.read(lmfile);
   lmfile.close();
     
   //create dictionary
   std::map<string, vector<string> > dictionary;
   readMap(map, dictionary);
      
   string test_s;
   while(getline(test, test_s)){
      vector<string> words;
      parseSen(test_s, words);
      viterbi(words, lm, voc, dictionary);
   }

   return 0;
}


void readMap(ifstream& m, std::map<string, vector<string> >& dict){
   string line;
   string key;
   vector<string> words;
   while(getline(m, line)){
      words.clear();
      key = line.substr(0,2);
      int pos = line.find_first_not_of(" ",2);
      string subline = line.substr(pos);

      size_t ll = subline.length();
      if(ll != 2){
         int index=0;
         size_t end;
         parseSen(subline, words);
      }
      else words.push_back(subline);
      dict[key] = words;
   }  
}

double monogram(const string& word, Ngram& lm, Vocab& voc){
   VocabIndex word_id = voc.getIndex(word.c_str());
   if(word_id == Vocab_None) word_id = voc.getIndex(Vocab_Unknown);
   VocabIndex context[] = {Vocab_None};
   return (lm.wordProb(word_id, context));
}

double bigram(const string& preword, const string& word, Ngram& lm, Vocab& voc){
   VocabIndex preword_id = voc.getIndex(preword.c_str());
   VocabIndex word_id = voc.getIndex(word.c_str());
   if(preword_id == Vocab_None) preword_id = voc.getIndex(Vocab_Unknown);
   if(word_id == Vocab_None) word_id = voc.getIndex(Vocab_Unknown);
   VocabIndex context[] = {preword_id, Vocab_None};
   return (lm.wordProb(word_id, context));
}


void parseSen(string& s, vector<string>& w){ 
   int index=0;
   size_t end;
   while(1){
      end = s.find(" ", index);
      if(end == string::npos) break;
      else{
         int d = end - index;
         while(d/2) {
            string word = s.substr(index, 2);
            w.push_back(word);
            d = d/2;
            if(d > 1) index = index+2;
         }
      }
      index = end+1;
   } 
}

void viterbi(vector<string>& words, Ngram& lm, Vocab& voc, map<string, vector<string> >& dict ){
   vector<vector<Word> > sentense;
   vector<Word> temp;
   
   size_t size = words.size();

   for(size_t i=0 ; i<size ; ++i){
      size_t sss = dict[words[i]].size();
      if(!dict[words[i]].empty()) dict[words[i]].push_back(words[i]);
      if(i == 0){
         temp.clear();
         for(size_t j=0 ; j<sss ; ++j){
            temp.push_back(Word(monogram(dict[words[i]][j], lm, voc), j, dict[words[i]][j]));         
         }   
      }

      else{
         temp.clear();
         vector<Word> pre = sentense.back();
         size_t ss = pre.size();

         for(size_t j=0 ; j<sss ; ++j){
            double max_prob = -DBL_MAX;
            size_t index;
            for(size_t k=0 ; k<ss ; ++k){
               double prob = pre[k].getProb() + bigram(pre[k].getWord(), dict[words[i]][j], lm, voc);
               if(prob > max_prob){max_prob = prob, index = k;}
            }
            temp.push_back(Word(max_prob, index, dict[words[i]][j]));
         }
      }
      sentense.push_back(temp);
   }

   //backtracking
   vector<Word> answer;
   vector<Word> end = sentense.back();
   size_t s = end.size();
   Word hiword = end[0];
   for(size_t i=1 ; i<s ; ++i)
      if(end[i].getProb() > hiword.getProb()) hiword = end[i];
    
   answer.push_back(hiword);
   size_t ssss = sentense.size();
   for(size_t i=1 ; i<ssss ; ++i){
      hiword = sentense[ssss-1-i][hiword.getPre()];
      answer.push_back(hiword);
   }

   cout<<"<s>";
   for(size_t i=0 ; i<ssss ; ++i) cout<<" "<<answer[ssss-1-i].getWord();
   cout<<" </s>"<<endl;
}



