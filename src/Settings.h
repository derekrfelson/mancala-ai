/*
 * Settings.h
 *
 *  Created on: Mar 8, 2016
 *      Author: derek
 */

#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include <cstdint>
#include <functional>
class State;

using HeuristicFn = std::function<int(const State&)>;
using NextMoveFn = std::function<State(const State& currentState)>;

struct GlobalState
{
public: /* Member functions */
	uint8_t totalStones() const;

public: /* Data members */
	int numStones;
	int numHoles;
	int searchDepth;
	bool prune;
	int prunedNodes;
	HeuristicFn p1Heuristic;
	HeuristicFn p2Heuristic;
	NextMoveFn p1NextMoveFn;
	NextMoveFn p2NextMoveFn;
	HeuristicFn currentHeuristic;
};

GlobalState& globalState();
void initializeGlobals();

#endif /* SRC_SETTINGS_H_ */
