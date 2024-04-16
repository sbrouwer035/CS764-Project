#ifndef Ncache_H
#define Ncache_H
#include "defs.h"
#include "NRecord.h"
#include "Constant.h"
#include "NTournamentTreeNode.h"
#include "NTournamentTree.h"
#include <vector>
#include <queue>
#include <math.h>

class Ncache
{
private:
    uint64_t cache_available;
    int numberOfRecord;
    int numberofBin;
    bool graceful_degradation;
    std::string lastSpilled;
    std::vector< std::queue<NRecord> > binList;
    std::vector<NTournamentTreeNode> *leafNodes; //list of leafNode that points to each bin
    NTournamentTreeNode *root;
    
  
public:
    Ncache(uint64_t cachesize);
    ~Ncache();

    //getter
    uint64_t get_cache_available(){
        return cache_available;
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
    void set_cache_available(uint64_t val){
        cache_available = val;
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
    void set_lastspilled(std::string spilled){
        lastSpilled=spilled;
    }


    //method
    void addRecord(NRecord record){
       
        if(get_cache_available() < (unsigned)record.get_size()){
            if (get_GD()){
                //evict as many records to make room for a new record
                // use while evict (record.get_size())


            }else {
                //get the number of bins, construct turnament tree 
                std::cout <<"I was here again\n\n\n";
                NTournamentTree *cacheTree = new NTournamentTree(get_numberOfBin());
                int x = 10;
                std::cout <<"I was here  \n\n\n"<< ceil(log2(x));
                cacheTree->buildTree();
                
                
                cacheTree->print(cacheTree->root);
                //evict as many records to make room for new size evict (record.get_size())
                set_GD(true); 
            }
        }
        if (get_GD() && record.get_value() >= lastSpilled){
            set_cache_available(get_cache_available() - record.get_size());
            //binList.front().push(record);
            //TODO: push new record to the tree
        }else{
            set_cache_available(get_cache_available() - record.get_size());
            std::queue<NRecord> newbin;
            newbin.push(record);
            binList.push_back(newbin);
            set_numberOfBin(get_numberOfBin() + 1);
            set_numberOfRecord(get_numberOfRecord() + 1);
        }
    }
    //void reset_cache
};

#endif