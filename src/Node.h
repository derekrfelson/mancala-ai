/*
 * Node.h
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#ifndef SRC_NODE_H_
#define SRC_NODE_H_

#include "State.h"
#include "Move.h"
#include "MoveIterator.h"
#include <cstdint>
#include <memory>
#include <stack>
#include <queue>
#include <iosfwd>

class Node
{
public:
	explicit Node(const State& state, uint8_t depth, bool maximizer);
	Node(const Node&) = delete;
	Node& operator=(Node&) = delete;
	Node& operator=(Node&&) = default;
	Node(Node&&) = default;
	void expandNextNode(std::stack<Node>& fringe);
	bool hasNextNode() const;
	void updateParent();
	std::queue<Move> getBestMove() const;
	std::ostream& print(std::ostream& stream) const;
private:
	State state;
	Node* parent;
	std::unique_ptr<std::queue<Move> > action;
	uint8_t depth;
	int alpha;
	int beta;
	bool maximizer;
	MoveIterator iter;
	std::unique_ptr<std::queue<Move> > bestMove;
private: // Member functions
	explicit Node(const State& state, Node* const parent,
			std::unique_ptr<std::queue<Move> > action, uint8_t depth,
			int alpha, int beta, bool maximizer);
	void update(const Node& child);
	int getValue() const;
	bool isTerminalState() const;
};

std::ostream& operator<<(std::ostream& stream, const Node& node);
int calculateHeuristic1(const State& currentState);

#endif /* SRC_NODE_H_ */
