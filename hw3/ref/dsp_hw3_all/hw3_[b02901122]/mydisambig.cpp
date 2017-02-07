#include<vector>
#include<iostream>
#include "Ngram.h"
#include<fstream>
#include<algorithm>
#include<cmath>
#include<string>
using namespace std;

void Viterbi(string&,Vocab&, Ngram&,vector< pair<string,string> >&);
void Readmap(ifstream& ,vector<pair<string,string> >&);
struct CompareFirst
{
    CompareFirst(string val):_val(val){}
    bool operator()(const pair<string,string> element)const{
        return element.first==_val;
    }

  private:
    string _val;
};

int main(int argc, char** argv)
{
    if(argc!=9){cerr<<"NO!"<<endl;exit(-1);}
    ifstream text,map;
    text.open(argv[2]);
    if(!(text.is_open()))
    {
        cerr<<"input file cannot open!"<<endl;
        return 0;
    }
    map.open(argv[4]);
    if(!(map.is_open()))
    {
        cerr<<"map file cannot open!"<<endl;
        return 0;
    }
    int ngram_order=atoi(argv[8]);
    Vocab voc;
    Ngram lm( voc , ngram_order );
    File lmfile(argv[6] , "r" );
    lm.read(lmfile);
    lmfile.close();
    /********************************/
    vector< pair<string,string> >map_pair;
    
    Readmap(map,map_pair); 
    
    string text_s;//test data
    while(getline(text,text_s))
    {
        text_s=text_s.substr(1);
        Viterbi(text_s,voc,lm,map_pair);
    }

}

void Readmap(ifstream& map,vector< pair<string,string> >& map_pair)
{
    string templine;
    pair<string,string>p;
    while(getline(map,templine))
    {
        p.first=templine.substr(0,2);
        int pos=templine.find_first_not_of(" ",2);
        p.second=templine.substr(pos);
        map_pair.push_back(p);
    }
}
double get_bigram(string& s1,string& s2, Vocab& voc, Ngram& lm)
{
    VocabIndex wid1=voc.getIndex(s1.c_str());
    VocabIndex wid2=voc.getIndex(s2.c_str());
    if(wid1==Vocab_None)wid1=voc.getIndex(Vocab_Unknown);
    if(wid2==Vocab_None)wid2=voc.getIndex(Vocab_Unknown);
    VocabIndex context[]={wid1,Vocab_None};
    return lm.wordProb(wid2,context);
}
double get_monogram(string& s1, Vocab& voc, Ngram& lm)
{
    VocabIndex wid1=voc.getIndex(s1.c_str());
    if(wid1==Vocab_None)wid1=voc.getIndex(Vocab_Unknown);
    VocabIndex context[]={Vocab_None};
    return lm.wordProb(wid1,context);
}


void Viterbi(string& text_s,Vocab& voc, Ngram& lm, vector< pair<string,string> >&map_pair)
{
    vector<string> word;
    string temp; //for parse
    int start=0;
    while(1)
    {
        temp=text_s.substr(start,2);
        word.push_back(temp);
        start=text_s.find_first_not_of(" ",start+2);
        if(start==string::npos)
            break;
    }
    /************************/
    size_t size=word.size();
    vector<string>* t_names=new vector<string>[size];
    vector<int>*    t_prev =new vector<int>   [size];
    vector<double>* t_prob =new vector<double>[size]; 
    vector< pair<string,string> >::iterator it;
    string _second;
    
    //for t=0
    it = find_if(map_pair.begin(), map_pair.end(), CompareFirst(word[0]) );
    if (it==map_pair.end() ){ cerr<< "no such word in mapping!" <<endl;exit(-1); }
    _second = it->second;
    if( _second.length() >=3)
    {
        string shortstring; // for parse
        int pos=0;
        while(1)
        {
            shortstring=_second.substr(pos,2);
            t_names[0].push_back(shortstring);
            t_prob[0].push_back(get_monogram(shortstring,voc,lm));
            
            pos=_second.find_first_not_of(" ",pos+2);
            if(pos== string::npos)break;
        }
    }
    else
    {
        t_names[0].push_back(_second);
        t_prob[0].push_back(get_monogram(_second,voc,lm));
        t_prev[0].push_back(0);
    }
    //end t=0
    for(size_t t=1; t<size; ++t)
    {
        it= find_if ( map_pair.begin(), map_pair.end(), CompareFirst(word[t]) );
        if(it==map_pair.end())exit(-1);
        _second=it->second;
        if(_second.length()>=3)
        {
            string shortstring; // for parse
            int pos=0;
            double prob=0;
            while(1)
            {
                shortstring=_second.substr(pos,2);  //切第一個字
                t_names[t].push_back(shortstring);// push 到 time t
                double temp_prob=0;
                int temp_prev=0;
                for(size_t i=0 ; i< t_prob[t-1].size(); ++i)  //probability of time t-1
                {
                    temp_prob=t_prob[t-1][i]+get_bigram(t_names[t-1][i],shortstring,voc,lm);
                    if(i==0){prob=temp_prob;temp_prev=i;}
                    if(temp_prob>prob)
                    {
                        prob=temp_prob;
                        temp_prev=i; //record who goes to me
                    }
                }
                t_prob[t].push_back(prob);
                t_prev[t].push_back(temp_prev);
                pos=_second.find_first_not_of(" ",pos+2);
                if(pos== string::npos)break;
            }
        }
        else
        {
            t_names[t].push_back(_second);
            double prob=0;
            double temp_prob=0;
            int temp_prev=0;
            for(size_t i=0; i<t_prob[t-1].size(); ++i)
            {
                temp_prob=t_prob[t-1][i]+get_bigram(t_names[t-1][i], _second,voc,lm);
                if(i==0) {prob=temp_prob;temp_prev=i;}
                if(temp_prob>prob)
                {
                    prob=temp_prob;
                    temp_prev=i;
                }

            }
            t_prob[t].push_back(prob);
            t_prev[t].push_back(temp_prev);
        }
    }
    /************back track****************/
    vector<string>output;
    int index=0;
    double t_p;

    for(size_t i=0; i<t_prob[size-1].size() ;++i)
    {
        if(i==0){ t_p=t_prob[size-1][i]; index=i;}
        if(t_prob[size-1][i]>t_p){ t_p=t_prob[size-1][i]; index=i;}
    }
    for(size_t t= size-1;t>=0 ;--t)
    {
        output.push_back(t_names[t][index]);
        if(t!=0){index=t_prev[t][index];}
        else {break;}
    }
    
    cout<<"<s> ";
    for(size_t i=0 ;i<output.size();++i)
    {
        cout<<output[output.size()-1-i]<<" ";
    }
    cout<<"</s>"<<endl;
}



