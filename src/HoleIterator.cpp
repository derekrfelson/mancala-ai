/*
 * HoleIterator.cpp
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#include "HoleIterator.h"
#include "State.h"
#include "Settings.h"
#include <cassert>

// MoveNumber is from 1 - #holes
HoleIterator::HoleIterator(uint8_t moveNumber, State& state, bool clockwise)
: currentHole{moveNumber},
  state{state},
  clockwise{clockwise},
  otherMancala1{0},
  otherMancala2{0},
  myMancala1{0},
  myMancala2{0}
{
	assert(moveNumber > 0 && moveNumber <= globalState().numHoles);
	if (!state.getIsP1Turn())
	{
		// Suppose there are 4 holes. Then clockwise hole order goes:
		//
		// Move #s     1  2  3  4
		// Hole #s  0  1  2  3  4  5
		// Hole #s  11 10 9  8  7  6
		//
		// Where the bottom row is P2, the middle row is P1, and the
		// top row is just a legend with the move numbers.
		// So for P2, 1->10, 2->9, 3->8, and 4->7.
		//
		// P1's mancalas are 0, 5, and P2's mancalas are 11, 6.
		//
		// The purpose of the HoleIterator class is to hide this ugliness from
		// the rest of the code while allowing easy iteration through the holes
		// and mancalas in either clockwise or counterclockwise order.
		//
		// When you do the math, the transformation from move number
		// to bottom hole is:
		currentHole += 3 + 2 * (globalState().numHoles - moveNumber);
	}

	if (state.getIsP1Turn())
	{
		otherMancala1 = globalState().numHoles + 2;
		otherMancala2 = mod() - 1;
		myMancala1 = 0;
		myMancala2 = globalState().numHoles + 1;
	}
	else
	{
		otherMancala1 = 0;
		otherMancala2 = globalState().numHoles + 1;
		myMancala1 = globalState().numHoles + 2;
		myMancala2 = mod() - 1;
	}
}

void HoleIterator::next()
{
	// Check if the next is the other person's mancala


	if (clockwise)
	{
		if (currentHole + 1 == otherMancala1
				|| currentHole + 1 == otherMancala2)
		{
			currentHole = (currentHole + 2) % mod();
		}
		else
		{
			currentHole = (currentHole + 1) % mod();
		}
	}
	else
	{
		if (currentHole - 1 == otherMancala1
				|| currentHole - 1 == otherMancala2)
		{
			currentHole = (mod() + currentHole - 2) % mod();
		}
		else
		{
			currentHole = (mod() + currentHole - 1) % mod();
		}
	}
}

uint8_t& HoleIterator::operator*()
{
	assert(currentHole != otherMancala1 && currentHole != otherMancala2);
	if (currentHole == myMancala1 || currentHole == myMancala2)
	{
		if (state.getIsP1Turn())
		{
			return state.p1Captures;
		}
		else
		{
			return state.p2Captures;
		}
	}
	else
	{
		if (currentHole >= 1 && currentHole <= globalState().numHoles)
		{
			// We are in P1 hole range
			return state.p1Holes[currentHole-1];
		}
		else
		{
			// We are in P2 hole range
			// Assuming 4 holes, the hole iterator order looks like this:
			//
			// Hole #s  0  1  2  3  4  5
			// Hole #s  11 10 9  8  7  6
			// Array#      0  1  2  3
			//
			// Need to transform hole iterator #s to array indices:
			//
			//    7->p2Holes[3]
			//    8->p2Holes[2]
			//    9->p2Holes[1]
			//   10->p2Holes[0]
			//
			// When you do the math, the formula is like so:
			auto holeIndex = mod()-2-currentHole;
			assert(holeIndex >= 0 && holeIndex < state.p2Holes.size());
			return state.p2Holes[holeIndex];
		}
	}
}

uint8_t HoleIterator::mod() const
{
	return globalState().numHoles * 2 + 4;
}
