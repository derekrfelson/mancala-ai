#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <cstddef>
#include <vector>
#include "Settings.h"
#include "State.h"
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
	globalState().numStones = stones;

	auto holes = -1;
	stringstream{argv[2]} >> holes;
	if (holes < stones - 1 || holes > 2* (stones - 1))
	{
		usage();
		return 1;
	}
	globalState().numHoles = holes;

	cout << "Holes: " << holes << endl;
	cout << "Stones: " << stones << endl;

	auto p1Holes = vector<uint8_t>{};
	auto p2Holes = vector<uint8_t>{};
	for (auto i = 0; i < globalState().numHoles; ++i)
	{
		p1Holes.push_back(globalState().numStones);
		p2Holes.push_back(globalState().numStones);
	}

	auto startState = State{p1Holes, p2Holes, 0};

	cout << startState << endl;
	cout << endl;
	startState.prettyPrint(cout);
	cout << endl;

	return 0;
}
