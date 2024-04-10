#include <string>
#include "Iterator.h"

class SortPlan : public Plan
{
	friend class SortIterator;
public:
	SortPlan (Plan * const input);
	~SortPlan ();
	Iterator * init () const;
private:
	Plan * const _input;
}; // class SortPlan

class SortIterator : public Iterator
{
public:
	SortIterator (SortPlan const * const plan);
	~SortIterator ();
	bool next ();
private:
	SortPlan const * const _plan;
	Iterator * const _input;
	RowCount _consumed, _produced;
}; // class SortIterator

class Run
{
public:
	Run();
	std::string nextValue();
private:
	int recordCount;
	int index;
};

class TreeNode
{
public:
	TreeNode(Run runList[], int runCount, int depth, int * runsAssigned);
	std::string getNextVal();

private:
	std::string leftVal;
	std::string rightVal;
	TreeNode * leftChild;
	TreeNode * rightChild;
	bool isLeaf;
	Run * leftSource;
	Run * rightSource;

	void setLeftVal();
	void setRightVal();
	std::string getNextInput(Run * source);
};

class SortTree
{
public:
	SortTree(Run runList[], int runCount);
private:
	TreeNode * root;
};
