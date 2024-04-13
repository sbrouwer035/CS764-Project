#pragma once
#include <string>
#include <vector>

class Run
{
public:
	Run();
	std::string nextValue();
    void addValue(std::string val);
	void mergeSubRuns(std::vector<Run*> &runList, int runCount);
private:
    std::vector<std::string> * recordList;
	int recordCount;
	int index;
};