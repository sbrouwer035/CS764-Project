#include <string>
#include "Iterator.h"
#include "Run.h"

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

class TreeNode
{
public:
	TreeNode(std::vector<Run*> &runList, int runCount, int depth, int * runsAssigned);
	std::string getNextVal(int depth);

private:
	std::string leftVal;
	std::string rightVal;
	TreeNode * leftChild;
	TreeNode * rightChild;
	bool isLeaf;
	Run * leftSource;
	Run * rightSource;

	void setLeftVal(int depth);
	void setRightVal(int depth);
	std::string getNextInput(Run * source);
};

class SortTree
{
public:
	SortTree(std::vector<Run*> &runList, int runCount);
	std::string nextValue();
private:
	TreeNode * root;
};
