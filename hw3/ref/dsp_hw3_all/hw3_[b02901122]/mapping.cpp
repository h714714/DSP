#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<string>
#include<vector>
using namespace std;
vector<string>first;
vector<string>sec;
vector<string>* output;
void push(string&, vector<int>&, size_t&);
int Zhu_to_num(string&);
bool Zhu[37];
int main(int argc, char** argv)
{
    output = new vector<string>[37];
    for(int i=0;i<37;i++)Zhu[i]=false;
    ifstream fin;
    fin.open(argv[1]);
    string line;
    while (getline(fin, line))
    {
        int pos = line.find_first_of(" ");
        first.push_back(line.substr(0, pos));
        sec.push_back(line.substr(pos + 1));
    }

    string tempshort;
    vector<int>repeats;
    for (size_t i = 0; i < sec.size(); ++i)
    {
        repeats.clear();
        string templong = sec[i];
        int start = 0;
        int pos = 0;
        while (1)
        {
            pos = templong.find_first_of("/", start);
            tempshort = templong.substr(start, 2);
            push(tempshort, repeats, i);
            if (pos == int(string::npos))
                break;
            start = pos + 1;
        }
    }
    ofstream fout;
    fout.open(argv[2]);
    for(int i=0;i<37;++i)
    {
        if(!output[i].empty())
        {
            fout<<output[i][0]<<" ";
            for(size_t j=1;j<output[i].size();++j)
            {
                fout<<" "<<output[i][j];
            }
            fout<<endl;
            for(size_t k=1;k<output[i].size();++k)
            {
                fout<<output[i][k]<<" "<<output[i][k]<<endl;
            }
        }
    }



}
void push(string& s,vector<int>& v,size_t& row)
{
    int index=0;
    index=Zhu_to_num(s);
    if(Zhu[index]==false&&output[index].size()==0)
    {
        output[index].push_back(s);
        Zhu[index]=true;
    }

    bool re=false;
    if(v.empty())v.push_back(index);
    else 
    {
        for(size_t i=0;i<v.size();++i){
            if(v[i]==index){re=true;break;}
        }
    }
    if(!re)
    {
        output[index].push_back(first[row]);
    }
}
int Zhu_to_num(string& s)
{
    int n1 = int(s[0]);
    int n2 = int(s[1]);
    if (n1 != -93)return -1;
    if (n2 < 0) n2 += 256;
    switch (n2){
        case 0x74:return 0;
        case 0x75:return 1;
        case 0x76:return 2;
        case 0x77:return 3;
        case 0x78:return 4;
        case 0x79:return 5;
        case 0x7a:return 6;
        case 0x7b:return 7;
        case 0x7c:return 8;
        case 0x7d:return 9;
        case 0x7e:return 10;
        case 0xa1:return 11;
        case 0xa2:return 12;
        case 0xa3:return 13;
        case 0xa4:return 14;
        case 0xa5:return 15;
        case 0xa6:return 16;
        case 0xa7:return 17;
        case 0xa8:return 18;
        case 0xa9:return 19;
        case 0xaa:return 20;
        case 0xab:return 21;
        case 0xac:return 22;
        case 0xad:return 23;
        case 0xae:return 24;
        case 0xaf:return 25;
        case 0xb0:return 26;
        case 0xb1:return 27;
        case 0xb2:return 28;
        case 0xb3:return 29;
        case 0xb4:return 30;
        case 0xb5:return 31;
        case 0xb6:return 32;
        case 0xb7:return 33;
        case 0xb8:return 34;
        case 0xb9:return 35;
        case 0xba:return 36;
        default: return 37;
    }
}


