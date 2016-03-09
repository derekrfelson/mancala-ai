/*
 * HoleIterator.h
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#ifndef SRC_HOLEITERATOR_H_
#define SRC_HOLEITERATOR_H_

#include <cstdint>
class State;
class Move;

class HoleIterator
{
public:
	HoleIterator(const Move& move, State& state);
	void next();
	uint8_t& operator*();
private:
	uint8_t currentHole;
	State& state;
	bool clockwise;
	uint8_t mod() const;
	uint8_t otherMancala1;
	uint8_t otherMancala2;
	uint8_t myMancala1;
	uint8_t myMancala2;
};

#endif /* SRC_HOLEITERATOR_H_ */
