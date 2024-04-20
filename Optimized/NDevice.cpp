#include "NDevice.h"
#include "Constant.h"
#include "NRecord.h"
#include <vector>
#include <fstream>

NDevice::NDevice(uint64_t memorySize, std::string type)
{   
    this -> memory_available = memorySize;
    this -> numberOfRecord = 0;
    this -> numberofBin = 1;
    this -> graceful_degradation = false;
    deviceType = type;
    std::queue<NRecord> buffer_bin;
    this -> binList.push_back(buffer_bin); 
    this -> cacheTree = NULL;
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
    
    if (deviceType=="HDD"){
        writeToFile(record);
        std::cout << record.get_key() << "\n\n";
        return;
    }
    // remove consective duplicate values
    if (record.get_value() == lastAddedValue)
    {
        return;
    }

    if (get_memory_available() < (unsigned)record.get_size())
    {
        if (get_GD())
        {

            // evict as many records to make room for a new record
            int spaceReleased = cacheTree->spilltoFreeSpace(record.get_size(), binList, *nextLevelDevice);

            // can't release more space from existing tree
            if (spaceReleased == 0)
            {
                cleanup_Device();
                addRecord(record);
                return;
            }

            set_memory_available(get_memory_available() + (unsigned)(spaceReleased));
        }
        else
        {

            // get the number of bins, construct turnament tree
            cacheTree = new NTournamentTree(get_numberOfBin(), "cache"); // std::cout <<get_numberOfBin()<<"  I am in addRecord and about to start the GD logic\n\n\n";

            int counter = 0;
            cacheTree->buildTree(counter, cacheTree->numberOfBins); cacheTree-> print("",cacheTree->root, false);
            cacheTree->fillTree(binList, get_numberOfBin());        cacheTree-> print("",cacheTree->root, false);

            // std::cout << get_memory_available() << "before \n";
            set_memory_available(get_memory_available() + (unsigned)(cacheTree->spilltoFreeSpace(record.get_size(), binList, *nextLevelDevice))); // std::cout << get_cache_available() << "after \n";

            set_GD(true);
        }
    }

    if (get_GD())
    {
        set_memory_available(get_memory_available() - record.get_size());
        // std::cout<< "\n\n I am here in addRecord about to add a record under GD" << cacheTree -> lastSpilledKey << record.get_key()<<binList.front().empty() << "\n";

        if (record.get_key() <= cacheTree->lastSpilledKey ||
            ((!binList.front().empty()) && (binList.front().back().get_value() > record.get_value())))
        {

            if (side_bin.empty())
            {
                side_bin.push(record);
                // std::cout << "\n\n the side bin after adding record" << side_bin.front().get_value() << "\n\n\n";
            }
            else
            {

                cleanup_Device();

                // std::cout << "\n\n after cleanup and add record retry\n\n";
                set_memory_available(get_memory_available() + record.get_size());
                addRecord(record);
                return;
            }
        }
        else if (!binList[0].empty() && binList[0].back().get_value() <= record.get_value())
        {
            if (binList[0].back().get_value() == record.get_value())
            {
                set_memory_available(get_memory_available() + record.get_size());
                return;
            }
            binList[0].push(record);
        }
        else
        {
            binList[0].push(record);
            cacheTree->pushBufferBinFirstRecord(binList);
            // std::cout << "I am adding record to buffer bin \n";
        }
    }
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
        std::cout << "device type is : " << deviceType << "CACHE Tree NULL" << (cacheTree==NULL);
        return;
    }
    

    // std::cout<< "I was here in cleanup_Cache() \n\n" ;
    set_memory_available(get_memory_available() + cacheTree->spillAll(binList, *nextLevelDevice));
    // std::cout << get_cache_available()<< "\n";

    numberOfRecord -= cacheTree->numberOfspilled;

    numberofBin = 2;
    graceful_degradation = false;
    std::queue<NRecord> tempbuffer_bin;
    std::queue<NRecord> tempside_bin;
    // std::queue<NRecord> newside_bin;
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
    //std::cout << "\n\n cleanup_Cache print the binList"; //<< binList[1].front().get_key() << binList[0].empty()<<"\n\n\n";

    //std::cout << "\n\n cleanup_Cache print the sidebin"; //<< side_bin.front().get_value() <<"\n\n\n";
}
void NDevice::end_Device()
{   std::cout << "device type is : " << deviceType;
    cleanup_Device();
    
    if (binList.size() > 0)
    {
        int counter = 0;
        cacheTree = new NTournamentTree(get_numberOfBin(), "cache");
        /*
        std::cout << get_numberOfBin() << "  I was here  \n\n\n";
        for (int i = 0; i < numberofBin; i++)
        {
            std::cout << binList[i].front().get_key() << "\n";
        }
        std::cout << binList[0].front().get_key() << "\n\n";
        */
        
        cacheTree->buildTree(counter, cacheTree->numberOfBins);
        cacheTree->fillTree(binList, get_numberOfBin());
        
    }
    cleanup_Device();
   /*
    std::cout << "device type is : " << deviceType << "@ " << (deviceType !="SSD");
    if(deviceType !="SSD"){
        nextLevelDevice->end_Device();
    }
    */


    std::cout << "\n\n"
              << get_numberOfRecord() << "\n\n";    
}

