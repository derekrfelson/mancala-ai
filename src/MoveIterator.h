/*
 * MoveIterator.h
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#ifndef SRC_MOVEITERATOR_H_
#define SRC_MOVEITERATOR_H_

#include "Move.h"
#include "State.h"
#include <memory>
#include <queue>

const Move& NO_MORE_MOVES();

class MoveIterator
{
public:
	explicit MoveIterator(const State& state);
	bool hasNext() const;
	bool isValid() const;
	void next();
	std::queue<Move> operator*();
private:
	State state;
	Move move;
	std::unique_ptr<MoveIterator> bonusMove;
};

#endif /* SRC_MOVEITERATOR_H_ */
