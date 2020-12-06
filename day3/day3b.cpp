#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	fstream f("input.txt");
	
	string row;
	
	int pos[5] = {0}, slope[5] = {1,3,5,7,1}, trees[5] = {0};
	
	long mul;
	
	while (!f.eof())
	{
		f >> row;
		
		mul=1;
		for (int i=0; i<5; ++i)
		{
			trees[i] += (slope[i] && row[pos[i]] == '#');
			pos[i] = (pos[i] + slope[i]) % row.length();
			mul *= trees[i];
		}
		
		slope[4] ^= 1;
	}
	
	cout << mul << endl;
	
	return 0;
}
