/*
 * Settings.h
 *
 *  Created on: Mar 8, 2016
 *      Author: derek
 */

#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include <cstdint>

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
};

GlobalState& globalState();

#endif /* SRC_SETTINGS_H_ */
