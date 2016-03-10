/*
 * Node.h
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#ifndef SRC_NODE_H_
#define SRC_NODE_H_

#include "State.h"
#include <cstdint>

class Node
{
public:
	explicit Node(const State& state, uint8_t depth, bool maximizer);
	Node(const Node&) = delete;
	Node& operator=(Node&) = delete;
	Node& operator=(Node&&) = default;
	Node(Node&&) = default;
private:
	State state;
	uint8_t depth;
	Node* parent;
};

#endif /* SRC_NODE_H_ */
