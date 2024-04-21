#ifndef NDevice_H
#define NDevice_H
#include "defs.h"
#include "NRecord.h"
#include "Constant.h"
#include "NTournamentTreeNode.h"
#include "NTournamentTree.h"
#include <vector>
#include <queue>
#include <math.h>
#include <limits>

class NDevice
{
private:

public:
    uint64_t memory_available;
    int numberOfRecord;
    int numberofBin;
    bool graceful_degradation;
    std::string lastAddedValue;   //last added record value
    std::string lastAddedKey;     //last added record key
    std::string deviceType;       //CACHE; DRAM; SSD; HDD
    std::vector< std::queue<NRecord> > binList; //List of Bins for Tree
    std::queue<NRecord> side_bin; //Host new entries that can't fit into the Tree. Only one additional bin is allowed.
    NDevice *nextLevelDevice;
    NTournamentTree *cacheTree;   //pointer to the current tree
    

    NDevice(uint64_t memorySize, std::string type);
    
    //getter
    uint64_t get_memory_available(){
        return memory_available;
    }
    int get_numberOfRecord(){
        return numberOfRecord;
    }
    int get_numberOfBin(){
        return numberofBin;
    }
    bool get_GD(){
        return graceful_degradation;
    }
    std::vector< std::queue<NRecord> > get_binList(){
        return binList;
    }

    //setter
    void set_memory_available(uint64_t val){
        memory_available = val;
    }
    void set_numberOfRecord(int val){
        numberOfRecord = val;
    }
    void set_numberOfBin(int val){
        numberofBin = val;
    }
    void set_GD(bool GD_enabled){
        graceful_degradation=GD_enabled;
    }


    //method
    void writeToFile(NRecord record);
    void addRecord(NRecord record);
    void cleanup_Device ();
    void end_Device();

};

#endif