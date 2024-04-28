#include "NDevice.h"
#include "Constant.h"
#include "NRecord.h"
#include <vector>
#include <fstream>
#include <sstream>

NDevice::NDevice(uint64_t memorySize, std::string type, char* traceFN, double lat, double bw)
{   
    this -> memory_available = memorySize;
    this -> numberOfRecord = 0;
    this -> numberofBin = 1;
    this -> graceful_degradation = false;
    deviceType = type;
    std::queue<NRecord> buffer_bin;
    this -> binList.push_back(buffer_bin); 
    this -> cacheTree = NULL;
    this -> traceFile = traceFN;
    this -> latency = lat;
    this -> bandwidth = bw;
}

void NDevice::writeToFile(NRecord record){
    std::fstream createdOutputFile;
	createdOutputFile.open("output_table", std::ios_base::app);
	if (!createdOutputFile.is_open()){
        std::cout<<"Error: fail to open the file. \n";
        return;
    }
    createdOutputFile << record.get_value();
	createdOutputFile <<std::endl;

    createdOutputFile.close();
}

void NDevice::addRecord(NRecord record)
{   
    
    if (deviceType=="output_table"){
        writeToFile(record);
        return;
    }
    // remove consective duplicate values
    if (record.get_value() == lastAddedValue)
    {
        return;
    }

    // No more room, let's make some
    if (get_memory_available() < (unsigned)record.get_size())
    {
        // Evict records from our merge tree
        if (get_GD())
        {
            int spaceReleased = cacheTree->spilltoFreeSpace(record.get_size(), binList, *this, *nextLevelDevice);
            set_memory_available(get_memory_available() + (unsigned)(spaceReleased));

            // can't release more space from existing tree, flush device and start fresh
            if (spaceReleased == 0)
            {
                cleanup_Device();
                addRecord(record);
                return;
            }
        }

        // Build merge tree and free up space
        else
        {
            int counter = 0;
            cacheTree = new NTournamentTree(get_numberOfBin(), "cache");
            cacheTree->buildTree(counter, cacheTree->numberOfBins);
            cacheTree->fillTree(binList, get_numberOfBin());

            int spaceReleased = cacheTree->spilltoFreeSpace(record.get_size(), binList, *this, *nextLevelDevice);
            set_memory_available(get_memory_available() + (unsigned)(spaceReleased));

            set_GD(true);
        }
    }

    // memory is tight, determine if we should stash record record or flush device
    if (get_GD())
    {
        // Record sorts too low
        if (record.get_key() <= cacheTree->lastSpilledKey ||
            ((!binList.front().empty()) && (binList.front().back().get_value() > record.get_value())))
        {

            // put one record off to the side
            if (side_bin.empty())
            {
                set_memory_available(get_memory_available() - record.get_size());
                side_bin.push(record);
            }

            // we've saved off one record already, at this point there's probably more coming so flush device
            else
            {
                cleanup_Device();
                addRecord(record);
                return;
            }
        }

        // Record sorts after last value in first bin -> add it to the bin
        else if (!binList[0].empty() && binList[0].back().get_value() <= record.get_value())
        {
            // Skip if it's a duplicate
            if (binList[0].back().get_value() == record.get_value())
            {
                set_memory_available(get_memory_available() + record.get_size());
                return;
            }

            binList[0].push(record);
        }

        // First bin is empty -> add record and push it into the tree
        else
        {
            binList[0].push(record);
            cacheTree->pushBufferBinFirstRecord(binList);
            // std::cout << "I am adding record to buffer bin \n";
        }
    }

    // Standard case: plenty of memory available, put record into appropriate bin
    else
    {
        set_memory_available(get_memory_available() - record.get_size());
        if (!binList.back().empty() && binList.back().back().get_key() < record.get_key())
        {
            binList.back().push(record);
        }
        else
        {
            std::queue<NRecord> newbin;
            newbin.push(record);
            binList.push_back(newbin);
            set_numberOfBin(get_numberOfBin() + 1);
        }
    }

    set_numberOfRecord(get_numberOfRecord() + 1);
    lastAddedKey = record.get_key();
    lastAddedValue = record.get_value();
}

void NDevice::cleanup_Device()
{
    
    if (cacheTree == NULL || cacheTree->root ==NULL)
    {
        return;
    }
    
    set_memory_available(get_memory_available() + cacheTree->spillAll(binList, *this, *nextLevelDevice));

    numberOfRecord -= cacheTree->numberOfspilled;

    numberofBin = 2;
    graceful_degradation = false;
    std::queue<NRecord> tempbuffer_bin;
    std::queue<NRecord> tempside_bin;
    while (!side_bin.empty())
    {
        tempside_bin.push(side_bin.front());
        side_bin.pop();
    }
    // clear(side_bin);

    binList.clear();

    binList.push_back(tempbuffer_bin);
    binList.push_back(tempside_bin);

    cacheTree = NULL;
}
void NDevice::end_Device(NDevice& outputDevice)
{   
    if (numberOfRecord == 0) {
        return;
    }
    if (cacheTree == NULL)
    {
        int counter = 0;
        cacheTree = new NTournamentTree(get_numberOfBin(), "cache");
        cacheTree->buildTree(counter, cacheTree->numberOfBins);
        cacheTree->fillTree(binList, get_numberOfBin());
    }
    if (cacheTree->root ==NULL)
    {
        return;
    }
    
    // No records at the next level -> write the output
    if (nextLevelDevice == NULL || nextLevelDevice->numberOfRecord == 0) {
        cacheTree->spillAll(binList, *this, outputDevice);
        return;
    }
    
    // Merge remaining records into next device
    cleanup_Device();
}

std::string NDevice::latencyString(uint64_t dataSize){
    if (deviceType != "HDD" && deviceType != "SSD"){
        return "";
    }
    int time = (2*latency) + (dataSize/bandwidth);
    std::stringstream str;
    str << " with latency " << time << "us";
    return str.str();
}

