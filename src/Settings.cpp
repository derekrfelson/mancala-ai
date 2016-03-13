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

void initializeGlobals()
{
	globalState().numHoles = 4;
	globalState().numStones = 4;
	globalState().searchDepth = 1;
	globalState().prune = true;
	globalState().prunedNodes = 0;
	globalState().p1Heuristic = nullptr;
	globalState().p2Heuristic = nullptr;
	globalState().p1NextMoveFn = nullptr;
	globalState().p2NextMoveFn = nullptr;
	globalState().currentHeuristic = nullptr;
}
