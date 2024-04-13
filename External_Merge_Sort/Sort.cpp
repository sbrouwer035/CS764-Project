#include <iostream>
#include "Sort.h"
#include "Constant.h"

SortPlan::SortPlan (Plan * const input) : _input (input)
{
	TRACE (true);
} // SortPlan::SortPlan

SortPlan::~SortPlan ()
{
	TRACE (true);
	delete _input;
} // SortPlan::~SortPlan

Iterator * SortPlan::init () const
{
	TRACE (true);
	return new SortIterator (this);
} // SortPlan::init

SortIterator::SortIterator (SortPlan const * const plan) :
	_plan (plan), _input (plan->_input->init ()),
	_consumed (0), _produced (0)
{
	TRACE (true);

	while (_input->next ())  ++ _consumed;
	delete _input;

	traceprintf ("consumed %lu rows\n",
			(unsigned long) (_consumed));
} // SortIterator::SortIterator

SortIterator::~SortIterator ()
{
	TRACE (true);

	traceprintf ("produced %lu of %lu rows\n",
			(unsigned long) (_produced),
			(unsigned long) (_consumed));
} // SortIterator::~SortIterator

bool SortIterator::next ()
{
	TRACE (true);

	if (_produced >= _consumed)  return false;

	++ _produced;
	return true;
} // SortIterator::next

SortTree::SortTree (std::vector<Run*> &runList, int runCount)
{
	//need tree with runCount/2 leaf nodes	
	int runsAssigned = 0;
	root = new TreeNode(runList, runCount, 0, &runsAssigned);

}

std::string SortTree::nextValue()
{
	return root->getNextVal(0);
}

TreeNode::TreeNode(std::vector<Run*> &runList, int runCount, int depth, int * runsAssignedPtr)
{
	leftVal = "";
	rightVal = "";
	isLeaf = false;

	// leaf nodes point to runs
	if ((1 << depth) >= (runCount/2)) {					// 1 << depth == 2^depth
		isLeaf = true;
		if (*runsAssignedPtr <= runCount) {
			leftSource = runList.at(*runsAssignedPtr);
			(*runsAssignedPtr)++;
		}
		if (*runsAssignedPtr <= runCount) {
			rightSource = runList.at(*runsAssignedPtr);
			(*runsAssignedPtr)++;
		}
		return;
	}

	// intermediate nodes
	leftChild = new TreeNode(runList, runCount, depth+1, runsAssignedPtr);
	rightChild = new TreeNode(runList, runCount, depth+1, runsAssignedPtr);
}

std::string TreeNode::getNextVal(int depth)
{
	std::string lowestValue;

	// bring up contenders
	if (leftVal == "") {
		setLeftVal(depth);
	}
	if (rightVal == "") {
		setRightVal(depth);
	}

	// determine winning contender (lowest value for ascending sort)
	if (leftVal.compare(rightVal) <  0) {
		lowestValue = leftVal;
		leftVal = "";
	}
	else {
		lowestValue = rightVal;
		rightVal = "";
	}
	return lowestValue;
}

void TreeNode::setLeftVal(int depth) {
	if (isLeaf) {
		leftVal = getNextInput(leftSource);
	}
	else {
		leftVal = leftChild->getNextVal(depth+1);
	}
}

void TreeNode::setRightVal(int depth) {
	if (isLeaf) {
		rightVal = getNextInput(rightSource);
	}
	else {
		rightVal = rightChild->getNextVal(depth+1);
	}
}

std::string TreeNode::getNextInput(Run * source) {
	if (source == nullptr) {
		return MAX_VALUE;
	}
	return source->nextValue();
}
