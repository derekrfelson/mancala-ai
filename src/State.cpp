/*
 * State.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: derek
 */

#include <ostream>
#include <iomanip>
#include "State.h"
#include "Settings.h"

State::State(std::vector<uint8_t> p1Holes,
			std::vector<uint8_t> p2Holes,
			uint8_t p1Captures,
			bool isP1Turn)
: p1Holes{p1Holes},
  p2Holes{p2Holes},
  p1Captures{p1Captures},
  isP1Turn{isP1Turn}
{
}

uint8_t State::getP1Captures() const
{
	return p1Captures;
}

uint8_t State::getP2Captures() const
{
	return globalState().totalStones() - getUncaptured();
}

uint8_t State::getUncaptured() const
{
	uint8_t sum = 0;
	for (auto c : p1Holes)
	{
		sum += c;
	}
	for (auto c : p2Holes)
	{
		sum += c;
	}
	return sum;
}

bool State::getIsP1Turn() const
{
	return isP1Turn;
}

std::ostream& State::print(std::ostream& stream) const
{
	stream << static_cast<int>(p1Captures) << "/";
	for (auto i = 0; i < p1Holes.size()-1; ++i)
	{
		stream << static_cast<int>(p1Holes[i]) << ",";
	}
	stream << static_cast<int>(p1Holes[p1Holes.size()-1]) << "/";
	for (auto i = 0; i < p2Holes.size()-1; ++i)
	{
		stream << static_cast<int>(p2Holes[i]) << ",";
	}
	stream << static_cast<int>(p2Holes[p2Holes.size()-1]);
	return stream;
}

std::ostream& State::prettyPrint(std::ostream& stream) const
{
	stream << "P1's mancala contains " << static_cast<int>(getP1Captures())
			<< " stones" << std::endl;
	stream << "P2's mancala contains " << static_cast<int>(getP2Captures())
				<< " stones" << std::endl;
	stream << "Uncaptured stones remaining: "
			<< static_cast<int>(getUncaptured())
			<< std::endl;

	// Column labels for usability
	stream << "  # ";
	for (auto i = 1; i <= globalState().numHoles; ++i)
	{
		stream << std::setfill(' ') << std::setw(3) << i << " ";
	}
	stream << "   " << std::endl;

	stream << " * |"; // Top left mancala
	for (auto h : p1Holes)
	{
		stream << std::setfill(' ') << std::setw(3)
		<< static_cast<int>(h) << "|";
	}
	stream << " * "; // Top right mancala
	stream << std::endl;
	stream << " * |"; // Bottom left mancala
	for (auto h : p2Holes)
	{
		stream << std::setfill(' ') << std::setw(3)
		<< static_cast<int>(h) << "|";
	}
	stream << " * "; // Bottom right mancala
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const State& state)
{
	return state.print(stream);
}
