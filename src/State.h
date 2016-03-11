/*
 * State.h
 *
 *  Created on: Mar 8, 2016
 *      Author: derek
 */

#ifndef SRC_STATE_H_
#define SRC_STATE_H_

#include <cstdint>
#include <vector>
#include <iosfwd>

struct State
{
public: /* Member functions */
	State(std::vector<uint8_t> p1Holes,
			std::vector<uint8_t> p2Holes,
			uint8_t p1Captures,
			bool isP1Turn);
	uint8_t getP1Captures() const;
	uint8_t getP2Captures() const;
	uint8_t getUncaptured() const;
	bool getIsP1Turn() const;
	void nextTurn();
	bool isEndState() const;
	std::ostream& prettyPrint(std::ostream& stream) const;
	std::ostream& print(std::ostream& stream) const;
public: /* Data members */
	std::vector<uint8_t> p1Holes;
	std::vector<uint8_t> p2Holes;
	uint8_t p1Captures;
	uint8_t p2Captures;
private:
	bool isP1Turn;
};

std::ostream& operator<<(std::ostream& stream, const State& state);

#endif /* SRC_STATE_H_ */
