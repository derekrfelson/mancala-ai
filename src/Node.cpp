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
#include "Settings.h"
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
	if (depth > 0 && !isTerminalState() && iter.isValid())
	{
		if (globalState().prune)
		{
			globalState().prunedNodes += 1;
			return beta > alpha;
		}
		else
		{
			return true;
		}
	}

	return false;
}

void Node::expandNextNode(std::stack<Node>& fringe)
{
	assert(iter.isValid());
	auto newState = state;
	auto newMove = std::make_unique<std::queue<Move> >(*iter);
	applyMoves(newState, *newMove);
	fringe.emplace(Node{newState, this,
						std::move(newMove), depth > 0 ? depth - 1 : 0,
						alpha, beta, !maximizer});
	iter.next();
}

int Node::getValue() const
{
	if (parent && ((depth == 0) || isTerminalState()))
	{
		return globalState().currentHeuristic(state);
	}

	if (maximizer)
	{
		return alpha;
	}
	else
	{
		return beta;
	}
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
	if (maximizer)
	{
		//std::cout << "Maximizer (a=" << alpha << ",b=" << beta << ")"
		//		<< " updating on " << child.getValue() << std::endl;
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
			//std::cout << "New alpha: " << alpha << " (";
			//printMoves(*bestMove);
			//std::cout << ")" << std::endl;
		}
	}
	else
	{
		//std::cout << "Minimizer (a=" << alpha << ",b=" << beta << ")"
		//				<< " updating on " << child.getValue() << std::endl;
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
			//std::cout << "New beta: " << beta << " (";
			//printMoves(*bestMove);
			//std::cout << ")" << std::endl;
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
	stream << ", value=" << globalState().currentHeuristic(state);
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
	auto diff = 0;
	if (p1IsMaximizer)
	{
		diff = static_cast<int>(state.p1Captures) - state.p2Captures;
	}
	else
	{
		diff = static_cast<int>(state.p2Captures) - state.p1Captures;
	}

	// Place a high value on actually winning the game
	if (state.isEndState())
	{
		if (diff > 0)
		{
			diff += 9999;
		}
		else
		{
			diff -= 9999;
		}
	}

	return diff;
}

int calculateHeuristic2(const State& state, bool p1IsMaximizer)
{
	auto h = 130*calculateHeuristic1(state, p1IsMaximizer);
	auto maximizerStones = 0;
	auto minimizerStones = 0;
	for (auto val : state.p1Holes)
	{
		if (p1IsMaximizer)
		{
			maximizerStones += val;
		}
		else
		{
			minimizerStones += val;
		}
	}
	for (auto val : state.p2Holes)
	{
		if (!p1IsMaximizer)
		{
			maximizerStones += val;
		}
		else
		{
			minimizerStones += val;
		}
	}
	return h + maximizerStones - minimizerStones;
}
