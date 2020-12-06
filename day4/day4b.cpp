#include <cstdio>
#include <cstdlib>
#include <cstdint>

using namespace std;

int main()
{
	FILE *f = fopen("input.txt", "r");
	if (!f) return EXIT_FAILURE;
	
	char *input = new char[20000];
	int size = fread(input, 1, 20000, f);
	fclose(f);
	
	int validPassports = 0, validFields = 0;
	
	for (char *pos = input; pos != input+size; ++pos)
	{
		if (pos[0]=='\n' && pos[1]=='\n')
		{
			validPassports += (validFields==0x7F);
			validFields = 0;
		}
		
		if (*pos != ':') continue;
				
		int t = -1;
		
		switch (*(uint32_t*)(pos-3))
		{
		case 0x3A727962: t++; // byr
		case 0x3A727969: t++; // iyr
		case 0x3A727965: t++; // eyr
		{
			int yr = 1000*(pos[1]-'0') + 100*(pos[2]-'0') + 10*(pos[3]-'0') + (pos[4]-'0');
			
			validFields |= (yr>=(t==2?1920:t==1?2010:2020) && yr<=(t==2?2002:t==1?2020:2030)) << t;

			break;
		}
		case 0x3A6C6365: // ecl
		{
			uint32_t ecl = *(uint32_t*)pos;
			switch (ecl) {
			case 0x7972673A: case 0x6E72623A:
			case 0x68746f3A: case 0x6E72673A:
			case 0x756c623A: case 0x6C7A683A:
			case 0x626D613A:
				validFields |= 1 << 3;
			default: break;
			}
			break;
		}
		case 0x3A6C6368: // hcl
		{
			const char *p;

			for (p = pos+((pos[1]=='#')<<2); p-pos<8 && ((*p>='0' && *p<='9') || (*p>='a' && *p<='f')); ++p);
			validFields |= (p-pos==8) << 4;
			
			break;
		}
		case 0x3A746768: // hgt
		{	int num = 0; const char *p = pos+1;

			for (p=pos+1; *p>='0' && *p <= '9'; ++p) num = 10*num+(*p-'0');

			validFields |= ((p[0]=='c' && p[1]=='m') ? (num>=150 && num<=193) : (p[0]=='i' && p[1]=='n') && (num>=59 && num<=76)) << 5;

			break;
		}
		case 0x3A646970: // pid
			const char *p = pos+1;
			while (*p>='0' && *p<='9') p++;
			
			validFields |= (p-pos == 10) << 6;
			break;
		}
	}
	
	printf("%i\n", validPassports);
	
	return 0;
}
