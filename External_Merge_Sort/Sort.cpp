#include "Sort.h"

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

SortTree::SortTree (Run runList[], int runCount)
{
	//need tree with runCount/2 leaf nodes	
	int runsAssigned = 0;
	root = new TreeNode(runList, runCount, 0, &runsAssigned);

}

TreeNode::TreeNode(Run runList[], int runCount, int depth, int * runsAssignedPtr)
{
	// initialize values to null, they are retrieved during comparisons
	leftVal = "";
	rightVal = "";

	// leaf nodes point to runs
	if ((1 << depth) >= (runCount/2)) {					// 1 << depth == 2^depth
		isLeaf = true;
		if (*runsAssignedPtr <= runCount) {
			leftSource = &runList[*runsAssignedPtr];
			(*runsAssignedPtr)++;
		}
		if (*runsAssignedPtr <= runCount) {
			rightSource = &runList[*runsAssignedPtr];
			(*runsAssignedPtr)++;
		}
		return;
	}

	// intermediate nodes
	leftChild = new TreeNode(runList, runCount, depth+1, runsAssignedPtr);
	rightChild = new TreeNode(runList, runCount, depth+1, runsAssignedPtr);
}

std::string TreeNode::getNextVal()
{
	std::string lowestValue;

	// bring up contenders
	if (leftVal == "") {
		setLeftVal();
	}
	if (rightVal == "") {
		setRightVal();
	}

	// determine winning contender (lowest value for ascending sort)
	if (leftVal < rightVal) {
		lowestValue = leftVal;
		leftVal = "";
	}
	else {
		lowestValue = rightVal;
		rightVal = "";
	}
	return lowestValue;
}

void TreeNode::setLeftVal() {
	if (isLeaf) {
		leftVal = getNextInput(leftSource);
	}
	else {
		leftVal = leftChild->getNextVal();
	}
}

void TreeNode::setRightVal() {
	if (isLeaf) {
		rightVal = getNextInput(rightSource);
	}
	else {
		rightVal = rightChild->getNextVal();
	}
}

std::string TreeNode::getNextInput(Run * source) {
	if (source == nullptr) {
		return "The max string value";
	}
	return source->nextValue();
}

Run::Run ()
{

}

std::string Run::nextValue ()
{
	return "next value from set of keys in run";
}
