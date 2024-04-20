
#include "NTournamentTree.h"
#include "NTournamentTreeNode.h"
#include <cmath>
#include <math.h>

NTournamentTree::NTournamentTree (int binNumbers, std::string type){
    root = new NTournamentTreeNode ("",0);
    numberOfBins=binNumbers;
    lastSpilledKey ="";
    typeOfStorage = type;
    numberOfspilled=0;
    
    
    //layers = ceil(log2(binNumbers));
}
