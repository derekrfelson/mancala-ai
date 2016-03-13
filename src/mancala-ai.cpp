#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <cstddef>
#include <vector>
#include <queue>
#include <stack>
#include <cassert>
#include "Settings.h"
#include "State.h"
#include "Move.h"
#include "HoleIterator.h"
#include "MoveIterator.h"
#include "Node.h"
using namespace std;

void tests();
void usage();
State nextHumanMove(const State& currentState);
State nextAiMove(const State& currentState);

void usage()
{
	cerr << "Usage: mancala [stones] [holes] [depth]" << endl
		 << "   where stones in range [2, 6] " << endl
	     <<	"         and holes in range [stones-1, 2*(stones-1)]" << endl
		 << "         and depth in range [1,5]" << endl;
}

int main(int argc, char** argv)
{
	// Run some sanity tests
	tests();

	// Check number of parameters
	if (argc != 4)
	{
		usage();
		return 1;
	}

	// Get number of stones
	auto stones = -1;
	stringstream{argv[1]} >> stones;
	if (stones < 2 || stones > 6)
	{
		usage();
		return 1;
	}
	globalState().numStones = stones;

	// Get number of holes
	auto holes = -1;
	stringstream{argv[2]} >> holes;
	if (holes < stones - 1 || holes > 2 * (stones - 1))
	{
		usage();
		return 1;
	}
	globalState().numHoles = holes;

	// Get search depth
	auto depth = -1;
	stringstream{argv[3]} >> depth;
	if (depth < 1 || depth > 5)
	{
		usage();
		return 1;
	}
	globalState().searchDepth = depth;

	// Create starting state
	auto startState = State{};

	// Main game loop
	auto isHumanTurn = true;
	while (!startState.isEndState())
	{
		cout << endl;
		cout << endl;
		if (isHumanTurn)
		{
			assert(startState.getIsP1Turn());
			startState = nextHumanMove(startState);
			assert(!startState.getIsP1Turn());
		}
		else
		{
			assert(!startState.getIsP1Turn());
			startState = nextAiMove(startState);
			assert(startState.getIsP1Turn());
		}
		isHumanTurn = !isHumanTurn;
	}

	// Print who won
	cout << endl;
	cout << endl;
	cout << "GAME OVER" << endl;
	if (startState.p1Captures > startState.p2Captures)
	{
		cout << "Player 1 wins" << endl;
	}
	else if (startState.p1Captures < startState.p2Captures)
	{
		cout << "Player 2 wins" << endl;
	}
	else
	{
		cout << "Game ended in a tie" << endl;
	}

	return 0;
}

State nextAiMove(const State& currentState)
{
	// For now, AI is only player 2
	assert(!currentState.getIsP1Turn());

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
	fringe.emplace(Node{currentState, globalState().searchDepth, true});

	while (!fringe.empty())
	{
		cout << "AI is evaluating move " << fringe.top() << endl;

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
				// We're at the root now, so end the loop
				// If you pop the root, you won't be able to retrieve its data
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

	// Note that because newState indicates it's the other player's turn now,
	// you have to tell it to maximize for the opposite player.
	newState.nextTurn();
	cout << "Heuristic rates this state as "
			<< calculateHeuristic1(newState,
					currentState.getIsP1Turn()) << endl;
	newState.nextTurn();

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

void tests()
{
	globalState().numHoles = 4;
	globalState().numStones = 4;
	globalState().searchDepth = 1;
	auto p1Holes = vector<uint8_t>{0, 0, 6, 6};
	auto p2Holes = vector<uint8_t>{4, 4, 5, 5};
	auto startState = State{p1Holes, p2Holes, 2, false};
	auto s1 = stringstream{};
	s1 << startState;
	assert(s1.str() == "2/0,0,6,6/4,4,5,5/0*");
	auto m1 = Move{1, false};
	s1 = stringstream{};
	s1 << m1;
	assert(s1.str() == "Move{1,ccw}");
	auto m2 = Move{1, true};
	s1 = stringstream{};
	s1 << m2;
	assert(s1.str() == "Move{1,cw}");

	auto s1AfterM1 = startState;
	applyMove(s1AfterM1, m1);
	s1 = stringstream{};
	s1 << s1AfterM1;
	cout << "2/0,0,6,6/4,4,5,5/0* + Move{1,ccw} -> " << s1AfterM1 << endl;
	assert(s1.str() == "2/0,0,6,6/0,5,6,6/1*");

	auto s1AfterM2 = startState;
	applyMove(s1AfterM2, m2);
	s1 = stringstream{};
	s1 << s1AfterM2;
	cout << "2/0,0,6,6/4,4,5,5/0* + Move{1,cw} -> " << s1AfterM2 << endl;
	assert(s1.str() == "*2/1,1,7,6/0,4,5,5/1");
}
