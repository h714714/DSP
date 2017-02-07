#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
   fstream ifile(argv[1], ios::in),
           ofile(argv[2], ios::out);
   map<string, vector<string> > z_b;

   string B, Z, line, key;
   while(getline(ifile, line))
   {
      B = line.substr(0, 2);
      line = line.substr(3);
      stringstream ss(line);
      while(getline(ss, Z, '/')) 
      {
         key = Z.substr(0, 2);
	 if(z_b[key].empty() || z_b[key].back() != B) z_b[key].push_back(B);
      }
   }

   for(map<string, vector<string> >::iterator i = z_b.begin(); i != z_b.end(); ++i)
   {
      string lead = i->first;
      ofile << lead;
      vector<string>& content = z_b[lead];
      size_t z = content.size();
      for(size_t j = 0; j < z; ++j) ofile << " " << content[j];
      ofile << endl;
      for(size_t j = 0; j < z; ++j) ofile << content[j] << " " << content[j] << endl; 
   }

   return 0;
}
