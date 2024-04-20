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
#include <limits>

class Ncache
{
private:

public:
    uint64_t cache_available;
    int numberOfRecord;
    int numberofBin;
    bool graceful_degradation;
    std::string lastSpilled;
    std::vector< std::queue<NRecord> > binList;
    std::queue<NRecord> side_bin;
    NTournamentTree *cacheTree;

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
    void addRecord(NRecord record);
    void cleanup_Cache (){
        
        if (cacheTree==NULL) {return;}
        std::cout<< "I was here in cleanup_Cache() \n\n" ;
        set_cache_available( get_cache_available() + cacheTree -> spillAll(binList));
        std::cout << get_cache_available()<< "\n";
        numberOfRecord -= cacheTree->numberOfspilled;
        
        numberofBin = 2;
        graceful_degradation = false;
        lastSpilled = "";
        std::queue<NRecord> tempbuffer_bin;
        std::queue<NRecord> tempside_bin;
        //std::queue<NRecord> newside_bin;
        while (!side_bin.empty()){
            tempside_bin.push(side_bin.front());
            side_bin.pop();
        }
        //clear(side_bin);
        
        binList.clear();
             
        binList.push_back(tempbuffer_bin);
        binList.push_back(tempside_bin);
        
        cacheTree = NULL;
        std::cout <<"\n\n cleanup_Cache print the binList"; //<< binList[1].front().get_key() << binList[0].empty()<<"\n\n\n";

        std::cout <<"\n\n cleanup_Cache print the sidebin"; //<< side_bin.front().get_value() <<"\n\n\n";         
    }
    void end_Cache(){
        cleanup_Cache();

        if(binList.size() > 0){
            int counter=0;
            cacheTree = new NTournamentTree(get_numberOfBin(),"cache");
            std::cout <<get_numberOfBin()<<"  I was here  \n\n\n";
            for (int i = 0; i < numberofBin ; i++){
                std::cout << binList[i].front().get_key() << "\n";
            }
            std::cout << binList[0].front().get_key() <<"\n\n";
            cacheTree-> buildTree(counter, cacheTree->numberOfBins);
            std::cout << binList[0].front().get_key() <<"\n\n";
            cacheTree -> fillTree(binList, get_numberOfBin());   
            //cacheTree ->print("",cacheTree->root,false);
        }
        cleanup_Cache();

        std::cout << "\n\n"<< get_numberOfRecord()<< "\n\n";
       
        //todo:  send end DRAM 
    }
    //void reset_cache
};

#endif