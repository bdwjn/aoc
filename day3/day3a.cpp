#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	fstream f("input.txt");
	
	string row;
	
	int pos=0, trees = 0;
	
	while (!f.eof())
	{
		f >> row;
		trees += row[pos%row.size()]=='#';
		pos+=3;
	}
	
	cout << trees << endl;
	
	return 0;
}
