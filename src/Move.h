/*
 * Move.h
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#ifndef SRC_MOVE_H_
#define SRC_MOVE_H_

#include <cstdint>
#include <iosfwd>
class State;

struct Move
{
public:
	Move(uint8_t holeNumber, bool clockwise);
	uint8_t holeNumber;
	bool clockwise;
	std::ostream& print(std::ostream& stream) const;
};

void applyMove(State& state, const Move& move);
std::ostream& operator<<(std::ostream& stream, const Move& move);

#endif /* SRC_MOVE_H_ */
