#ifndef NTournamentTree_H
#define NTournamentTree_H
#include "NTournamentTreeNode.h"
#include "Constant.h"
#include "NRecord.h"
#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <queue>
#include <string>

class NDevice;


class NTournamentTree
{
private: 
    
public:    
    NTournamentTreeNode *root;
    int numberOfBins;
    std::vector <NTournamentTreeNode *> leafNodes;
    std::string lastSpilledKey;
    std::string typeOfStorage;
    int numberOfspilled;

    //int layers;

    NTournamentTree(int binNumbers, std::string type);
    ~NTournamentTree();

    NTournamentTreeNode * buildTree(int& counter, int numberOfLeaves);
    void print(const std::string& prefix, NTournamentTreeNode * node, bool isLeft);
    void fillTree(std::vector< std::queue<NRecord> >& binList, int numberofBin );
    NTournamentTreeNode * mergeTree(NTournamentTreeNode *subtreeRoot, std::vector< std::queue<NRecord> >& binList);
    int computeOffSetValueCode (std::string smallerKey, std::string key2);
    uint64_t spillAll (std::vector< std::queue<NRecord> >& binList, NDevice& thisDevice, NDevice& nextLevelDevice);
    int spilltoFreeSpace (int inRecordSize, std::vector< std::queue<NRecord> >& binList, NDevice& thisDevice, NDevice& nextLevelDevice);
    void pushBufferBinFirstRecord(std::vector< std::queue<NRecord> >& binList);
    void bubbleUp (NTournamentTreeNode * currentNode, int tempOffsetValueCode, int index, std::vector< std::queue<NRecord> >& binList);
    void bufferReplaceBubbleUp (NTournamentTreeNode * currentNode, int index, std::vector< std::queue<NRecord> >& binList);

};

#endif