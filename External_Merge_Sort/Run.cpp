#include <iostream>
#include "Run.h"
#include "Constant.h"

Run::Run () {
    recordList = new std::vector<std::string>();
    recordCount = 0;
    index = 0;
}

std::string Run::nextValue() {
    if (index < recordCount) {
        std::string value = recordList->at(index);
        index++;
        return value;
    }
    return MAX_VALUE;
}

void Run::addValue (std::string val) {
    recordList->push_back(val);
    recordCount++;
    std::cout << "Added record (" << recordCount << "): " << val << "\n";
}