#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <cstddef>
#include <vector>
#include <queue>
#include <stack>
#include "Settings.h"
#include "State.h"
#include "Move.h"
#include "HoleIterator.h"
#include "MoveIterator.h"
#include "Node.h"
using namespace std;

void usage();
State nextHumanMove(const State& currentState);
State nextAiMove(const State& currentState);

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

	auto isHumanTurn = true;
	while (!startState.isEndState())
	{
		cout << endl;
		cout << endl;
		if (isHumanTurn)
		{
			startState = nextHumanMove(startState);
		}
		else
		{
			startState = nextAiMove(startState);
		}
		isHumanTurn = !isHumanTurn;
	}

	return 0;
}

State nextAiMove(const State& currentState)
{
	// Show the current state so the user knows what's going on
	cout << "AI's turn" << endl;
	if (currentState.getIsP1Turn())
	{
		cout << "AI is Player 1 (top)" << endl;
	}
	else
	{
		cout << "AI is Player 2 (bottom)" << endl;
	}
	currentState.prettyPrint(cout);

	auto fringe = stack<Node>{};
	// Limiting the search depth to 1 seems to lead to optimal play.
	// Because the AI assumes the human plays perfectly, allowing a deeper
	// search causes it to dismiss options that could lead to victory
	// when the human plays poorly.
	fringe.emplace(Node{currentState, 1, true});

	while (!fringe.empty())
	{
		if (fringe.top().hasNextNode())
		{
			// Expand the next node, and make that the top of the stack
			fringe.top().expandNextNode(fringe);
			// Following ordinary stack rules, we can only ever operate on the
			// top element, so go back to the start of the loop.
		}
		else
		{
			if (fringe.size() > 1)
			{
				fringe.top().updateParent();
				fringe.pop();
			}
			else
			{
				break;
			}
		}
	}

	auto newState = currentState;
	auto moves = fringe.top().getBestMove();
	cout << "AI chose moves: ";
	printMoves(moves);
	cout << endl;
	applyMoves(newState, moves);
	return newState;
}

// Note: this uses the currentState to determine whose move it is
State nextHumanMove(const State& currentState)
{
	// Show the current state so the user knows what's going on
	if (currentState.getIsP1Turn())
	{
		cout << "You are Player 1 (top)" << endl;
	}
	else
	{
		cout << "You are Player 2 (bottom)" << endl;
	}
	currentState.prettyPrint(cout);

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
