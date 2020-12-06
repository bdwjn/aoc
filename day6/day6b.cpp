#include <fstream>
#include <iostream>
using namespace std;

int main()
{
	fstream f("input.txt");

	char c, prev;
	
	int person=0, group=(1<<27)-1, total=0;
	
	while ((c = f.get()) != EOF)
	{
		if (c=='\n')
		{			
			if (prev=='\n')
			{
				total += __builtin_popcount(group);				
				group = (1<<27) - 1;
			} else 
			{
				group &= person;
				person = 0;
			}
		} else
		{
			person |= 1<<(c-'a');
		}
		
		prev = c;
	}
	
	total += __builtin_popcount(group);
	
	cout << total << endl;
	return 0;
}
