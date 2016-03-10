/*
 * Move.h
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#ifndef SRC_MOVE_H_
#define SRC_MOVE_H_

#include <cstdint>
class State;

struct Move
{
public:
	Move(uint8_t holeNumber, bool clockwise);
	uint8_t holeNumber;
	bool clockwise;
};

void applyMove(State& state, const Move& move);

#endif /* SRC_MOVE_H_ */
