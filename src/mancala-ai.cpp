#include <iostream>
#include <cstring>
#include <sstream>
using namespace std;

void usage()
{
	cerr << "Usage: mancala [stones] [holes]" << endl;
	cerr << "   where stones in range [2, 6] "
			"and holes in range [stones-1, 2*(stones-1)]"
		 << endl;
}

int main(int argc, char** argv)
{
	// Check parameters

	if (argc != 3)
	{
		usage();
		return 1;
	}

	auto stones = -1;
	stringstream{argv[1]} >> stones;
	if (stones < 2 || stones > 6)
	{
		usage();
		return 1;
	}

	auto holes = -1;
	stringstream{argv[2]} >> holes;
	if (holes < stones - 1 || holes > 2* (stones - 1))
	{
		usage();
		return 1;
	}

	cout << "Holes: " << holes << endl;
	cout << "Stones: " << stones << endl;

	return 0;
}
