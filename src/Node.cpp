/*
 * Node.cpp
 *
 *  Created on: Mar 9, 2016
 *      Author: derek
 */

#include "Node.h"
#include "State.h"
#include "Move.h"
#include "MoveIterator.h"
#include <ostream>
#include <iostream>
#include <cassert>

Node::Node(const State& state, Node* const parent,
			std::unique_ptr<std::queue<Move> > action, uint8_t depth,
			int alpha, int beta, bool maximizer)
: state{state},
  parent{parent},
  action{std::move(action)},
  depth{depth},
  alpha{alpha},
  beta{beta},
  maximizer{maximizer},
  iter{state},
  bestMove{nullptr}
{
}

// For a root node
Node::Node(const State& state, uint8_t depth, bool maximizer)
: Node(state, nullptr, nullptr, depth, -99999, 99999, maximizer)
{
}

// We will expand child nodes until we have explored all possible moves,
// or beta > alpha. Note that beta is our strongest lower bound and alpha
// is our strongest upper bound, so beta > alpha indicates a game state that
// any sane opponent will never let happen.
//
// We also stop expanding nodes when depth reaches 0 or when the game is over.
bool Node::hasNextNode() const
{
	return (depth > 0) && (!isTerminalState())
			&& (iter.hasNext()) && (beta > alpha);
}

void Node::expandNextNode(std::stack<Node>& fringe)
{
	assert(iter.hasNext());
	iter.next();
	auto newState = state;
	auto newMove = std::make_unique<std::queue<Move> >(*iter);
	applyMoves(newState, *newMove);
	fringe.emplace(Node{newState, this,
						std::move(newMove), depth > 0 ? depth - 1 : 0,
						alpha, beta, !maximizer});
}

int Node::getValue() const
{
	if (parent && ((depth == 0) || isTerminalState()))
	{
		std::cout << "getValue() using heuristic" << std::endl;
		return calculateHeuristic1(state, false);
	}

	if (maximizer)
	{
		std::cout << "getValue() returning alpha" << std::endl;
		return alpha;
	}
	else
	{
		std::cout << "getValue() returning beta" << std::endl;
		return beta;
	}
	return maximizer ? alpha : beta;
}

void Node::updateParent()
{
	if (parent)
	{
		parent->update(*this);
	}
}

void Node::update(const Node& child)
{
	std::cout << "Node::update" << std::endl;
	if (maximizer)
	{
		std::cout << "Node is maximizer" << std::endl;
		std::cout << "Child value: " << child.getValue() << std::endl;
		std::cout << "alpha: " << alpha << ", beta: " << beta << std::endl;
		// the value of a maximizer node is the highest
		// value of any of its children. It stores this
		// value as its alpha.
		if (child.getValue() > alpha)
		{
			alpha = child.getValue();
			if (bestMove)
			{
				*bestMove = *child.action;
			}
			else
			{
				bestMove = std::make_unique<std::queue<Move> >(*child.action);
			}
			std::cout << "New alpha: " << alpha << std::endl;
			std::cout << "New best move: ";
			printMoves(*bestMove);
			std::cout << std::endl;
		}
	}
	else
	{
		std::cout << "Node is minimizer" << std::endl;
		std::cout << "Child value: " << child.getValue() << std::endl;
		std::cout << "alpha: " << alpha << ", beta: " << beta << std::endl;
		// The value of a minimizer node is the lowest
		// value of any of its children. It stores this
		// value as its beta.
		if (child.getValue() < beta)
		{
			beta = child.getValue();
			if (bestMove)
			{
				*bestMove = *child.action;
			}
			else
			{
				bestMove = std::make_unique<std::queue<Move> >(*child.action);
			}
			std::cout << "New beta: " << beta << std::endl;
			std::cout << "New best move: ";
			printMoves(*bestMove);
			std::cout << std::endl;
		}
	}
}

bool Node::isTerminalState() const
{
	return state.isEndState();
}

std::queue<Move> Node::getBestMove() const
{
	assert(bestMove != nullptr);
	return *bestMove;
}

std::ostream& Node::print(std::ostream& stream) const
{
	stream << "Node{ depth=" << static_cast<int>(depth) << ", "
			<< "State{ " << state << " }, Action=";
	if (action)
	{
		printMoves(*action);
	}
	else
	{
		stream << "null";
	}
	stream << ", BestMove=";
	if (bestMove)
	{
		printMoves(*bestMove);
	}
	else
	{
		stream << "null";
	}
	stream << ", value=" << calculateHeuristic1(state, false);
	return stream << " }";
}

std::ostream& operator<<(std::ostream& stream, const Node& node)
{
	return node.print(stream);
}

/**
 * First heuristic is simple: the number of stones the current player
 * has captured, minus the number of stones the opponent has captured.
 */
int calculateHeuristic1(const State& state, bool p1IsMaximizer)
{
	if (p1IsMaximizer)
	{
		return state.p1Captures - state.p2Captures;
	}
	else
	{
		return state.p2Captures - state.p1Captures;
	}
}
