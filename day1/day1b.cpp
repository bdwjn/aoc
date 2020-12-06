#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	int nums[200], mults[2020] = {}, n = 0;	
	
	fstream f("input.txt");
	
	while (!f.eof()) f >> nums[n++];
	n--;
	
	f.close();
	
	for (int i=0; i<n; ++i)
	{	for (int j=0; j<i; ++j)
		{
			if (nums[i] + nums[j] < 2020)
				mults[nums[i] + nums[j]] = nums[i] * nums[j];
		}
	}
	
	for (int i=0; i<n; ++i)
	{
		if (mults[2020-nums[i]])
		{
			cout << mults[2020 - nums[i]] * nums[i] << endl;
			return 0;
		}
	}
	
	return EXIT_FAILURE;
}
