/*
 * Move.cpp
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#include "Move.h"
#include "State.h"
#include "HoleIterator.h"
#include <ostream>
#include <iostream>

Move::Move(uint8_t holeNumber, bool clockwise)
: holeNumber{holeNumber},
  clockwise{clockwise}
{
}

std::ostream& Move::print(std::ostream& stream) const
{
	return stream << "Move{" << static_cast<int>(holeNumber) << ","
			<< (clockwise ? "cw" : "ccw") << "}";
}

std::ostream& operator<<(std::ostream& stream, const Move& move)
{
	return move.print(stream);
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

	// If one player has no more stones on his side, we let the other
	// player capture all the stones on his own side, and the game ends.
	if (state.isEndState())
	{
		// See if it's P1 or P2 who has no more stones
		bool p1Done = true; // Assume it's P1 to start
		for (auto val : state.p1Holes)
		{
			if (val != 0)
			{
				// P1 still has stones, so P2 is the one who finished
				p1Done = false;
				break;
			}
		}

		// Let the other player capture all remaining pieces
		if (p1Done)
		{
			for (auto& val : state.p2Holes)
			{
				state.p2Captures += val;
				val = 0;
			}
		}
		else
		{
			for (auto& val : state.p1Holes)
			{
				state.p1Captures += val;
				val = 0;
			}
		}
	}
}

void applyMoves(State& state, std::queue<Move> moves)
{
	while (!moves.empty())
	{
		applyMove(state, moves.front());
		moves.pop();
	}
}

void printMoves(std::queue<Move> moves)
{
	while (!moves.empty())
	{
		std::cout << moves.front() << ",";
		moves.pop();
	}
}
