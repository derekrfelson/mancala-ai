/*
 * Settings.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: derek
 */

#include "Settings.h"

uint8_t GlobalState::totalStones() const
{
	return numHoles * 2 * numStones;
}

GlobalState& globalState()
{
	static GlobalState gs;
	return gs;
}
