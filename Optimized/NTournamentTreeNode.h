#ifndef NTournamentTreeNode_H
#define NTournamentTreeNode_H
#include <string>


class NTournamentTreeNode
{
    
private:
    

public:
    std::string key;   //first 8 bytes
    int index; //point to individual run/bin
    std::string winnerKey;
    int winnerIndex;
    int offsetValueCode;
    NTournamentTreeNode *left;
    NTournamentTreeNode *right;
    NTournamentTreeNode *parent;
    NTournamentTreeNode(std::string keyin, int indexin);
    ~NTournamentTreeNode();

};

#endif