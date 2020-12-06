#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	fstream f("input.txt");

	char c, prev;
	
	int answers=0, total=0;
	
	while ((c = f.get()) != EOF)
	{
		if (c=='\n')
		{
			if (prev=='\n')
			{
				total += __builtin_popcount(answers);
				answers = 0;
			}
		} else answers |= 1<<(c-'a');
		
		prev = c;
	}
	
	total += __builtin_popcount(answers);
	cout << total << endl;

	return 0;
}
