#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	fstream f("input.txt");
	
	bool found[2020] = { };
	
	while (!f.eof())
	{
		int num;
		f >> num;
		
		if (found[2020-num])
		{
			cout << num * (2020-num) << endl;
			return 0;
		} else
		{
			found[num] = true;
		}
	}
	
	return EXIT_FAILURE;
}
