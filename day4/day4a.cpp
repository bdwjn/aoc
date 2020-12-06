#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;

int main()
{
	fstream f("input.txt");
	
	string line;
	
	int correct = 0;
	
	map<string, string> passport;
	
	do
	{
		getline(f, line);
		
		int pos = 0, i;
		for (i=0; line[i]; ++i)
		{
			switch (line[i])
			{
			case ':': pos = i; break;
			case ' ':
				line[i] = line[pos] = 0;
				passport[&line[pos-3]] = &line[pos+1];
				break;
			}
		}
		
		if (pos) {
			line[i] = line[pos] = 0;
			passport[&line[pos-3]] = &line[pos+1];		
		} else
		{	
			correct += (passport.size() == 8 || (passport.size()==7 && passport.find("cid")==passport.end() ));	
			passport.clear();
		}
	} while (!f.eof());
	
	cout << correct << endl;
	
	return 0;
}
