#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cfloat>

#include "Ngram.h"

class vword
{
   public:
   vword(const string& w = "", const double p = -DBL_MAX, size_t pre = -1) : _word(w), _preword(pre), _acprob(p) {}
   bool operator >(const vword& i) {return _acprob > i._acprob;}
   string _word;
   size_t _preword;
   double _acprob;
};

double getprob(const string& preword, const string& word, Ngram& lm, Vocab& voc)
{
   VocabIndex pwid = voc.getIndex(preword.c_str()),
              wid = voc.getIndex(word.c_str());
   if(pwid == Vocab_None) pwid = voc.getIndex(Vocab_Unknown);
   if(wid == Vocab_None) wid = voc.getIndex(Vocab_Unknown);
   VocabIndex context[] = {pwid, Vocab_None};
   return (lm.wordProb(wid, context));
}


int main(int argc, char* argv[])
{
   if(argc != 9) {cerr << "Illegal input!\n"; exit(1);}
   int gram_order = atoi(argv[8]);
   Vocab voc;
   Ngram lm(voc, gram_order);
   
   File lmfile(argv[6], "r");
   lm.read(lmfile);
   lmfile.close();

   fstream mapping(argv[4], ios::in),
           test(argv[2], ios::in);

   map<string, vector<string> > dict;
   
   string line, buf, index, word;
   while(getline(mapping , line))
   {  
      bool first = true;
      stringstream ss(line);
      while(getline(ss, buf, ' '))
      {
         if(first) {index = buf; first = false; continue;}
         dict[index].push_back(buf);
      }
   }

   while(getline(test, line))
   {
      stringstream ss(line);
      vector<string> sentence;
      vector<vector<vword> > viterbi;
      bool first = true;
      while(getline(ss, word, ' '))
      { 
         if(word == "") continue;
         vector<vword> temp;
         sentence.push_back(word);

         if(dict[word].empty()) dict[word].push_back(word);

	 size_t z = dict[word].size();
	 if(first)
	 {
	    for(size_t i = 0; i < z; ++i)
               temp.push_back(vword(dict[word][i], getprob("<s>", dict[word][i], lm, voc)));
	    first = false;
	 }
	 else
	 {
	    vector<vword>& prev = viterbi.back();
	    size_t zz = prev.size();
	    for(size_t i = 0; i < z; ++i)
	    {
	       double maxp = -DBL_MAX;
	       size_t idx = 0;
	       for(size_t j = 0; j < zz; ++j)
	       {
	          double p = prev[j]._acprob + getprob(prev[j]._word, dict[word][i], lm, voc); 
	          if(p > maxp) {maxp = p; idx = j;}
	       }
	       temp.push_back(vword(dict[word][i], maxp, idx));
	    }     
         }
	 viterbi.push_back(temp);
      }
      
      vector<vword>& last = viterbi.back();
      vword res(last[0]);
      size_t zzz= last.size();
      for(size_t i = 0; i < zzz; ++i)
         if(last[i] > res) res = last[i];
     
      size_t zzzz = viterbi.size();
      size_t s_max = size_t(-1);   
      for(size_t i = 0; i < zzzz; ++i)
      {         
         sentence[zzzz-1-i] = res._word;
	 if(res._preword != s_max) res = viterbi[zzzz-2-i][res._preword];
      }
      
      cout << "<s>";
      for(size_t i = 0; i < zzzz; ++i)
      cout << " " << sentence[i];
      cout << " </s>" << endl;
   }
   return 0;
}

