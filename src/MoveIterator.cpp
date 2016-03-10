/*
 * MoveIterator.cpp
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#include "MoveIterator.h"
#include "Move.h"
#include "Settings.h"
#include "State.h"
#include <queue>

MoveIterator::MoveIterator(State& state)
: state{state},
  move{1, false},
  bonusMove{nullptr}
{
}

bool MoveIterator::hasNext()
{
	return move.clockwise != NO_MORE_MOVES().clockwise
			|| move.holeNumber != NO_MORE_MOVES().holeNumber;
}

void MoveIterator::next()
{
	// If we're on a bonus move, defer to the bonus move iterator
	if (bonusMove && bonusMove->hasNext())
	{
		bonusMove->next();
	}
	else
	{
		if (!move.clockwise)
		{
			// If counterclockwise, increment by making it clockwise
			move.clockwise = true;
		}
		else
		{
			// If clockwise, increment by making it counterclockwise
			// and incrementing the move number
			move.clockwise = false;
			move.holeNumber += 1;

			// If we've run out of holes, invalidate the iterator
			if (move.holeNumber > globalState().numHoles)
			{
				move = NO_MORE_MOVES();
			}
		}

		// Try out the move and see if it leads to bonus moves
		auto projectedState = state;
		applyMove(projectedState, move);
		if (projectedState.getIsP1Turn() == state.getIsP1Turn())
		{
			bonusMove = std::make_unique<MoveIterator>(projectedState);
		}
	}
}

std::queue<Move> MoveIterator::operator*()
{
	auto ret = std::queue<Move>{};
	ret.push(move);
	if (bonusMove)
	{
		auto bms = bonusMove->operator *();
		while (!bms.empty())
		{
			ret.push(bms.front());
			bms.pop();
		}
	}
	return ret;
}

const Move& NO_MORE_MOVES()
{
	static Move nmm{99, false};
	return nmm;
}
