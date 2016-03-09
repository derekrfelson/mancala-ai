/*
 * Move.h
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#ifndef SRC_MOVE_H_
#define SRC_MOVE_H_

#include <cstdint>

struct Move
{
public:
	Move(uint8_t holeNumber, bool clockwise);
	uint8_t holeNumber;
	bool clockwise;
};

#endif /* SRC_MOVE_H_ */
