#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <cstddef>
#include <vector>
#include "Settings.h"
#include "State.h"
#include "Move.h"
#include "HoleIterator.h"
using namespace std;

void usage();
State nextHumanMove(const State& currentState);
void applyMove(State& state, const Move& move);

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

	// Create starting state
	auto startState = State{p1Holes, p2Holes, 0, true};

	cout << startState << endl;
	cout << endl;
	startState.prettyPrint(cout);

	nextHumanMove(nextHumanMove(startState)).prettyPrint(cout);

	return 0;
}

// Note: this uses the currentState to determine whose move it is
State nextHumanMove(const State& currentState)
{
	// Show the current state so the user knows what's going on
	currentState.prettyPrint(cout);
	if (currentState.getIsP1Turn())
	{
		cout << "You are Player 1 (top)" << endl;
	}
	else
	{
		cout << "You are Player 2 (bottom)" << endl;
	}

	// We will successively modify the return state until
	// it is the next player's turn
	State returnState{currentState};

	bool bonusMove = false;
	do
	{
		if (bonusMove)
		{
			cout << "Bonus move! You get to go again!" << endl;
			returnState.prettyPrint(cout);
		}

		// Have the human enter the next move number and direction
		auto move = -1;
		auto clockwise = true;
		auto validDir = false;
		while (move < 1 || move > globalState().numHoles || !validDir)
		{
			// Reset the loop conditions
			move = -1;
			validDir = false;

			// Get the move number
			cout << "Select one of your holes (range is 1 - "
					<< globalState().numHoles << ")" << endl;
			auto input = string{};
			getline(cin, input);
			stringstream{input} >> move;

			// Get the move direction
			cout << "Move clockwise (cw) or counterclockwise (ccw)?" << endl;
			getline(cin, input);
			if (input.compare("ccw") == 0)
			{
				validDir = true;
				clockwise = false;
			}
			else if (input.compare("cw") == 0)
			{
				validDir = true;
				clockwise = true;
			}
			else
			{
				cout << "Invalid direction. Must be one of \"cw\" or \"ccw\""
						<< endl;
			}
		}

		// Apply the move
		applyMove(returnState, Move{move, clockwise});

		// Repeat the loop if we got a bonus move
		bonusMove = returnState.getIsP1Turn() == currentState.getIsP1Turn();
	} while (bonusMove);

	return returnState;
}

void applyMove(State& state, const Move& move)
{
	// Take all the stones in the chosen hole and drop them in successive
	// holes one by one.
	auto iter = HoleIterator{move, state};
	auto stonesInHand = *iter;
	*iter = 0;
	while (stonesInHand > 0)
	{
		iter.next();
		*iter += 1;
		stonesInHand -= 1;
	}

	// If the final stone ends up in an empty hole of yours, you get to
	// add all the stones in your opponent's corresponding hole into
	// your own mancala.
	if (*iter == 1 && iter.isOwnHole())
	{
		if (state.getIsP1Turn())
		{
			state.p1Captures += iter.opposite();
		}
		else
		{
			state.p2Captures += iter.opposite();
		}
		iter.opposite() = 0;
	}

	// If the final stone ends up in your mancala, you get another turn.
	if (!iter.isOwnMancala())
	{
		state.nextTurn();
	}
}
