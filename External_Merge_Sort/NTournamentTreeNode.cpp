#include "NTournamentTreeNode.h"

NTournamentTreeNode::NTournamentTreeNode(std::string keyin, int indexin)
{
    key=keyin;
    index = indexin;
    winnerKey = "" ;
    winnerIndex = 0 ;
    offsetValueCode = -1;
	left = NULL;
	right = NULL;
    parent = NULL;

}
NTournamentTreeNode::~NTournamentTreeNode(){}