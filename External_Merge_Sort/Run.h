#pragma once
#include <string>
#include <vector>

class Run
{
public:
	Run();
	std::string nextValue();
    void addValue(std::string val);
private:
    std::vector<std::string> * recordList;
	int recordCount;
	int index;
};