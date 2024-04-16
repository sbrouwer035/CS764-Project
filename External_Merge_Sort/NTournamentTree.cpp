
#include "NTournamentTree.h"
#include "NTournamentTreeNode.h"
#include <cmath>
#include <math.h>

NTournamentTree::NTournamentTree (int binNumbers){
    root = new NTournamentTreeNode ("",0);
    layers = log2 (binNumbers);
}
