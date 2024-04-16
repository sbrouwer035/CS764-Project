#include "Ncache.h"
#include "Constant.h"
#include "NRecord.h"
#include <vector>

Ncache::Ncache(uint64_t cachesize){
    cache_available = cachesize;
    numberOfRecord = 0;
    numberofBin = 1;
    graceful_degradation = false;
    lastSpilled = "";
    std::queue<NRecord> buffer_bin;
    binList.push_back(buffer_bin);
}
Ncache::~Ncache(){}


