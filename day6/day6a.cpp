#include <fstream>
using namespace std;

int main()
{
	fstream f("input.txt");

	char c, prev;
	
	int yes=0, total=0;
	
	while ((c = f.get()) != EOF)
	{
		if (c=='\n')
		{
			if (prev=='\n') { total += __builtin_popcount(yes); yes=0; }
		} else yes |= 1<<(c-'a');
		
		prev = c;
	}
	
	total += __builtin_popcount(yes);
	
	printf("%i\n", total);
	return 0;
}
