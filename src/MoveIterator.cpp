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
#include <cassert>

MoveIterator::MoveIterator(const State& state)
: state{state},
  move{1, false},
  bonusMove{nullptr}
{
}

bool MoveIterator::hasNext() const
{
	if (move.holeNumber < globalState().numHoles)
	{
		return true;
	}
	else if (move.holeNumber == globalState().numHoles)
	{
		// Last move is {numHoles, clockwise=true}
		return !move.clockwise;
	}
	else
	{
		// Hole number exceeded actual number of holes
		return false;
	}
}

bool MoveIterator::isValid() const
{
	return move.holeNumber <= globalState().numHoles
			&& move.holeNumber >= 1;
}

void MoveIterator::next()
{
	// If we're on a bonus move, defer to the bonus move iterator
	if (bonusMove)
	{
		bonusMove->next();
		if (bonusMove->isValid())
		{
			return;
		}
		else
		{
			bonusMove = nullptr;
		}
	}

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
}

std::queue<Move> MoveIterator::operator*()
{
	assert(isValid());

	// Add the current move to what we're returning
	auto ret = std::queue<Move>{};
	ret.push(move);

	// Even if we're not in the middle of iterating through a bonus move,
	// still check to see if we should be, before returning anything.
	if (!bonusMove)
	{
		auto projectedState = state;
		applyMove(projectedState, move);
		if (projectedState.getIsP1Turn() == state.getIsP1Turn())
		{
			bonusMove = std::make_unique<MoveIterator>(projectedState);
		}
	}

	// If we've either just started a bonus move, or are in the
	// middle of iterating through one, we need to add those to what we return
	if (bonusMove)
	{
		// We are already iterating through
		assert(bonusMove->isValid());
		auto bms = bonusMove->operator*();
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
