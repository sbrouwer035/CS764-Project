#ifndef NTournamentTree_H
#define NTournamentTree_H
#include "NTournamentTreeNode.h"
#include <iostream>


class NTournamentTree
{
private: 
    
public:    
    NTournamentTreeNode *root;
    int numberOfBins;
    int layers;

    NTournamentTree(int binNumbers);

    NTournamentTreeNode * buildTree(){
        NTournamentTreeNode *subTreeRoot;

        if (layers == 1){
            subTreeRoot = root;
        } else{
            subTreeRoot = new NTournamentTreeNode("",0);
        }
        
        if (numberOfBins > 0){
             subTreeRoot -> left = buildTree();
            (subTreeRoot -> left) -> parent = subTreeRoot;
            subTreeRoot -> right= buildTree();
            (subTreeRoot -> right) -> parent = subTreeRoot;
        }
        return subTreeRoot;
    };

    void print(NTournamentTreeNode *root){
        if(root == NULL){
            return;
        }
        std::cout << root->offsetValueCode << "\n";
        print(root->left);
        print(root-> right);

    }
};

#endif