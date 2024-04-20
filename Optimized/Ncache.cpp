#include "Ncache.h"
#include "Constant.h"
#include "NRecord.h"
#include <vector>

Ncache::Ncache(uint64_t cachesize){
    cache_available = cachesize;
    numberOfRecord = 0;
    numberofBin = 1;
    graceful_degradation = false;
    lastSpilled ="";
    std::queue<NRecord> buffer_bin;
    std::queue<NRecord> side_bin;
    binList.push_back(buffer_bin);
    cacheTree = NULL;
}
Ncache::~Ncache(){}

void Ncache::addRecord(NRecord record){
       
        if(get_cache_available() < (unsigned)record.get_size()){
            if (get_GD()){
                
                //evict as many records to make room for a new record
                int spaceReleased = cacheTree -> spilltoFreeSpace(record.get_size(), binList);
                
                //can't release more space from existing tree
                if (spaceReleased==0){
                    cleanup_Cache();
                    addRecord(record);
                    return;
                }
                
                set_cache_available( get_cache_available() + (unsigned)(spaceReleased));

            }else {
                
                //get the number of bins, construct turnament tree 
                cacheTree = new NTournamentTree(get_numberOfBin(),"cache");
                std::cout <<get_numberOfBin()<<"  I am in addRecord and about to start the GD logic\n\n\n";

                int counter=0;
                cacheTree-> buildTree(counter, cacheTree->numberOfBins);
                //cacheTree-> print("",cacheTree->root, false);
                cacheTree -> fillTree(binList, get_numberOfBin());
                //cacheTree-> print("",cacheTree->root, false);

                std::cout << get_cache_available() << "before \n";
                set_cache_available( get_cache_available() + (unsigned)(cacheTree -> spilltoFreeSpace(record.get_size(), binList)));
                std::cout << get_cache_available() << "after \n";
                
                set_GD(true); 
            }
        }


        if (get_GD()){
            set_cache_available(get_cache_available() - record.get_size());
            std::cout<< "\n\n I am here in addRecord about to add a record under GD" << cacheTree -> lastSpilledKey << record.get_key()<<binList.front().empty() << "\n";
            if (record.get_key() <= cacheTree -> lastSpilledKey ||
                    (!binList.front().empty() && binList.front().back().get_value() > record.get_value()) ){
                
                if (side_bin.empty()){
                    side_bin.push(record);
                    std::cout << "\n\n the side bin after adding record" << side_bin.front().get_value() << "\n\n\n";
                } else {
                    cleanup_Cache();

                    std::cout << "\n\n after cleanup and add record retry\n\n";
                    set_cache_available(get_cache_available() + record.get_size());
                    addRecord(record);
                    return;
                }
            }else if (!binList.front().empty() && binList.front().back().get_value() <= record.get_value()){
                if (binList.front().back().get_value() == record.get_value()){
                    set_cache_available(get_cache_available() + record.get_size());
                    return;
                }
                binList.front().push(record);
            }else {
                binList.front().push(record);
                cacheTree->pushBufferBinFirstRecord(binList);
                std::cout << "I am adding record to buffer bin \n";
            }
            //set_cache_available(get_cache_available() - record.get_size());
            //binList.front().push(record);
            //TODO: push new record to the tree
        }else{
            set_cache_available(get_cache_available() - record.get_size());
            std::queue<NRecord> newbin;
            newbin.push(record);
            binList.push_back(newbin);
            set_numberOfBin(get_numberOfBin() + 1);
            
        }
        set_numberOfRecord(get_numberOfRecord() + 1);
    }

