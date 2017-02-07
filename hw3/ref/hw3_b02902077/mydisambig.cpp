#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "Ngram.h"
#include "VocabMap.h"


#define minf -1.7E+308
using namespace std;

//global variables

unordered_map<string,vector<string> > ZhuYin_map;
Vocab voc;

//reading arg inputs
void read_arg(int argc, char* argv[], char* test, char* Yin, char* lm, int& order)
{
	for(int i=1; i< argc; ++i){
		if(strcmp("-text",argv[i]) == 0){
			strcpy(test,argv[i+1]);
			++i;
		}
		else if(strcmp("-map",argv[i]) == 0){
			strcpy(Yin,argv[i+1]);
			++i;
		}
		else if(strcmp("-lm",argv[i]) == 0){
			strcpy(lm,argv[i+1]);
			++i;
		}
		else if(strcmp("-order",argv[i]) == 0){
			order = atoi(argv[i+1]);
			++i;
		}
	}	
	return;
}

void read_ZhuYin_map(char *path)
{
	char buf[50001];
	char buf_w[5];
	string buf_w1;
	string buf_s;
	vector<string> buf_w2;

	FILE* pfile = fopen(path,"r");

	if(pfile==NULL) perror("Error opening map file");
	while(fgets(buf,50000,pfile) != NULL){
		//init buf_w2
		buf_w2.clear();

		sscanf(buf,"%s",buf_w);//read vocab1 in map
		buf_w1 = string(buf_w);

		//read vocab2 in map
		for(int i = 2; i<strlen(buf); ++i){
			if(buf[i]!=' '){
				sscanf(buf+i,"%s",buf_w);
				buf_s = string(buf_w);
				buf_w2.push_back(buf_s);
				i += 2;
			}

		}
		ZhuYin_map[buf_w1] = buf_w2;
	}
	return;
}

double getBigramProb(Ngram& lm,const char *w1, const char *w2)
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

double getTrigramProb(Ngram& lm,const char *w1, const char *w2, const char *w3) 
{
	VocabIndex wid1 = voc.getIndex(w1);
	VocabIndex wid2 = voc.getIndex(w2);
	VocabIndex wid3 = voc.getIndex(w3);

	if(wid1 == Vocab_None)  //OOV
		wid1 = voc.getIndex(Vocab_Unknown);
	if(wid2 == Vocab_None)  //OOV
		wid2 = voc.getIndex(Vocab_Unknown);
	if(wid3 == Vocab_None)  //OOV
		wid3 = voc.getIndex(Vocab_Unknown);

	VocabIndex context[] = { wid2, wid1, Vocab_None };
	return lm.wordProb( wid3, context );
}


void viterbi_bigram(Ngram& lm,int& count, string sen[])
{	
	double max_p = minf;
	double tmp=0;
	double path[100][800];
	double V[100][800];


	//init
	for(int i=0; i<100; ++i){
		for(int j =0; j<800; ++j){
			path[i][j] = 0;
			V[i][j]=minf;
		}
	}

	for(int i=0; i<ZhuYin_map[sen[0]].size(); ++i){
		V[0][i] = getBigramProb(lm,"<s>",ZhuYin_map[sen[0]][i].c_str());
	}	

	for(int k =1; k<count; ++k){
		for(int j =0 ; j<ZhuYin_map[sen[k]].size(); ++j){
			max_p = minf;
			for(int i =0; i< ZhuYin_map[sen[k-1]].size(); ++i){
				tmp = getBigramProb(lm,ZhuYin_map[sen[k-1]][i].c_str(),ZhuYin_map[sen[k]][j].c_str()) + V[k-1][i];
				if(tmp >= max_p){
					path[k][j] = i;
					V[k][j] = tmp;
					max_p = tmp;}}}			
		if(k == count-1){
			for(int i =0; i< ZhuYin_map[sen[k-1]].size(); ++i){
				tmp = getBigramProb(lm,ZhuYin_map[sen[k-1]][i].c_str(),sen[k].c_str()) + V[k-1][i];
				if(tmp >= max_p){
					path[k][0] = i;
					V[k][0] = tmp;
					max_p = tmp;}}}
	}

	//find path
	int end=0;
	int seq[1000] = {0};
	max_p = minf;
	seq[count-1] = end;
	for(int k=count-2; k>=0; --k){
		seq[k] = path[k+1][seq[k+1]];
	}

	//print
	cout<<"<s> ";
	for(int i=0; i<count-1; ++i){
		cout<<ZhuYin_map[sen[i]][seq[i]]<<" ";
	}
	cout<<"</s>\n";



	return;
}

void viterbi(FILE* pfile, Ngram& lm, int order)
{

	char buf[1001]={0};
	string sen[1000];//store each words in one line from input file

	char buf_w[4]={0};
	string buf_s[3];

	int count=0;

	//eats each lines of input file
	while(fgets(buf,1000,pfile)!=NULL){
		count=0;
		//convert input sentence to string
		for(int i = 0; i<strlen(buf);++i){
			if(buf[i]!=' '){
				if(sscanf(buf+i,"%s",buf_w) != EOF){
					sen[count] = string(buf_w);
					i += 2;
					++count;
				}
			}
		}
		sen[count] = string("</s>");
		++count;

		//run viterbi
		viterbi_bigram(lm,count,sen);

	}
	return;
}

int main(int argc, char* argv[])
{
	char testdata_path[256]={0};
	char ZhuYin_map_path[256]={0};
	char lm_path[256]={0};
	int order;

	char buffer[5001]={0};

	//read arg
	if(argc != 9){
		fprintf(stderr,"Error input arguments\n");
		exit(1);
	}

	read_arg(argc,argv, testdata_path,ZhuYin_map_path,lm_path,order);

	//open & read lm
	Ngram lm(voc, order);

	File lmFile(lm_path,"r");
	lm.read(lmFile);
	lmFile.close();

	//open & reading mapping
	read_ZhuYin_map(ZhuYin_map_path);

	//open input txt
	FILE* ptest = fopen(testdata_path,"r");
	if(ptest==NULL) perror("Error open testdata");

	//run viterbi
	viterbi(ptest,lm,order);

	exit(0);
}
