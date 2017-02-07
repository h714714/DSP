#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<string>

using namespace std;

int main(int argc, char* argv[]){

   fstream input_file(argv[1], ios::in);
   fstream output_file(argv[2], ios::out);

   map<string, vector<string> > Zu2Big;

   string line;
   
   while(getline(input_file, line)){
      string word = line.substr(0,2);
      string new_line = line.substr(3);

      int index = 0;
      size_t end;
      for(size_t i=0; i<new_line.length(); ++i){
         end = new_line.find("/", index);
         string key = new_line.substr(index, 2);
         if(Zu2Big[key].empty() || word != Zu2Big[key].back()) Zu2Big[key].push_back(word);
         index = end+1;
         if(end == string::npos) break;
      }
   }

   for(map<string, vector<string> >::iterator it = Zu2Big.begin() ; it != Zu2Big.end() ; ++it){
      string Zu = it->first;
      output_file << Zu;
      size_t size = Zu2Big[Zu].size();
      for(size_t i=0 ;i<size ; ++i) output_file << " " << Zu2Big[Zu][i];
      output_file << endl;
      for(size_t i=0 ;i<size ; ++i) output_file << Zu2Big[Zu][i] << " " << Zu2Big[Zu][i] <<endl;

   }


   return 0;
}
