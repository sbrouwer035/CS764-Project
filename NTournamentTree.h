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

    //Final !!! build Tree
    NTournamentTreeNode * buildTree(int& counter, int numberOfLeaves){
        NTournamentTreeNode *subTreeRoot;
        
        if (numberOfBins == 1){
            return root;
        } else if (numberOfBins == numberOfLeaves){
            subTreeRoot = root;
        }else{
            subTreeRoot = new NTournamentTreeNode("", counter);
        }
        
        if (numberOfLeaves > 1){
            int NextLeftLeaves = pow(2,ceil(log2(numberOfLeaves)))/2;
            subTreeRoot -> left = buildTree(counter , NextLeftLeaves); // pow(2,ceil(log2(numberOfLeaves)))/2);
            //std::cout << " please delete this left calls: " << pow(2,ceil(log2(numberOfLeaves)))/2 <<"\n";
            if (subTreeRoot->left != NULL){
                (subTreeRoot -> left) -> parent = subTreeRoot;
            } 
            subTreeRoot -> right = buildTree(counter , pow(2,ceil(log2(numberOfLeaves-NextLeftLeaves))));//pow(2,floor(log2(numberOfLeaves)))/2);
            //std::cout << " please delete this right calls: " << pow(2,floor(log2(numberOfLeaves)))/2 <<"\n";
            if (subTreeRoot->right != NULL){
                (subTreeRoot -> right) -> parent = subTreeRoot;
            }
        } else{
            if (numberOfLeaves == 1){
                leafNodes.push_back(subTreeRoot);
            }
            counter++;
        }
        return subTreeRoot;
    };

    //Final!!!! print the tree 
    void print(const std::string& prefix, NTournamentTreeNode * node, bool isLeft){
        if(node == NULL){
            return;
        }
        std::cout << prefix;
        if (prefix== ""){
            std::cout << "winner key: " << node -> winnerKey << "@" << node -> winnerIndex << "\n";
        }
        std::cout << (isLeft ? "|____" : "'----");
        if (node -> left == NULL || node->right == NULL){
            std::cout << "*" <<node->index << "@" << node->key << " with "<< node->offsetValueCode << std::endl;
        } else {
            std::cout << "O"<< node->index << "@" << node->key << " with "<< node->offsetValueCode << std::endl;
        }
        print(prefix + (isLeft ? "|     ": "      "),node->left, true);
        print(prefix + (isLeft ? "|     ": "      "),node-> right, false);
    }

    void fillTree(std::vector< std::queue<NRecord> >& binList, int numberofBin ){
        for (int i = 0; i < numberofBin; i++){
            if(binList[i].empty()){
                leafNodes[i] -> key = maxKey;
                leafNodes[i] -> winnerKey = maxKey;
                leafNodes[i] -> winnerIndex= i;
                leafNodes[i] -> index = i;
            } else{
                leafNodes[i] -> key = binList[i].front().get_key();
                leafNodes[i] -> winnerKey = binList[i].front().get_key();
                leafNodes[i] -> index = i;
                leafNodes[i] -> winnerIndex = i;
            }            
        }
        std::cout << "I am in fillTree: check leafnode key" << leafNodes[0]-> key<<"\n";
        if (!binList[0].empty()){
            std::cout << binList[0].front().get_key();
        }
        mergeTree (root, binList);
    }

    NTournamentTreeNode * mergeTree(NTournamentTreeNode *subtreeRoot, std::vector< std::queue<NRecord> >& binList){
        
        if(subtreeRoot->left != NULL){
            subtreeRoot->left = mergeTree( subtreeRoot->left, binList);
            subtreeRoot->right = mergeTree( subtreeRoot->right, binList);

            while (subtreeRoot->left->winnerKey == subtreeRoot->right->winnerKey && (subtreeRoot->left->winnerKey != maxKey) && binList[subtreeRoot->left->winnerIndex].front().get_value() == binList[subtreeRoot->right->winnerIndex].front().get_value()){
                //std::cout << "I am trying to remove duplicate" << subtreeRoot -> right -> winnerKey;
                binList[subtreeRoot->right->winnerIndex].pop();
                numberOfspilled++;
                if (binList[subtreeRoot->right->winnerIndex].empty()){
                    leafNodes[subtreeRoot->right->winnerIndex] -> key = maxKey;
                    leafNodes[subtreeRoot->right->winnerIndex] -> winnerKey = maxKey;
                }else{
                    leafNodes[subtreeRoot->right->winnerIndex] -> key = binList[subtreeRoot->right->winnerIndex].front().get_key();
                    leafNodes[subtreeRoot->right->winnerIndex] -> winnerKey = binList[subtreeRoot->right->winnerIndex].front().get_key();
                }
                subtreeRoot->right = mergeTree( subtreeRoot->right, binList);   
            } 

            if (subtreeRoot->left->winnerKey > subtreeRoot->right -> winnerKey || subtreeRoot->left->winnerKey == maxKey){
                subtreeRoot->key = subtreeRoot->left -> winnerKey;
                subtreeRoot->index = subtreeRoot->left ->winnerIndex;
                subtreeRoot->winnerKey = subtreeRoot -> right -> winnerKey;
                subtreeRoot->winnerIndex = subtreeRoot -> right -> winnerIndex;
                                
            } else if (subtreeRoot->left->winnerKey < subtreeRoot->right -> winnerKey ) {
                subtreeRoot->key = subtreeRoot->right -> winnerKey;
                subtreeRoot->index = subtreeRoot->right ->winnerIndex;
                subtreeRoot->winnerKey = subtreeRoot -> left -> winnerKey;
                subtreeRoot->winnerIndex = subtreeRoot -> left -> winnerIndex;

            } else {
                if (binList[subtreeRoot->left->winnerIndex].front().get_value() > binList[subtreeRoot->right->winnerIndex].front().get_value()){
                    subtreeRoot->key = subtreeRoot->left -> winnerKey;
                    subtreeRoot->index = subtreeRoot->left ->winnerIndex;
                    subtreeRoot->winnerKey = subtreeRoot -> right -> winnerKey;
                    subtreeRoot->winnerIndex = subtreeRoot -> right -> winnerIndex;
                } else {
                    subtreeRoot->key = subtreeRoot->right -> winnerKey;
                    subtreeRoot->index = subtreeRoot->right ->winnerIndex;
                    subtreeRoot->winnerKey = subtreeRoot -> left -> winnerKey;
                    subtreeRoot->winnerIndex = subtreeRoot -> left -> winnerIndex;
                }
            }

            if (subtreeRoot->winnerKey == maxKey){
                subtreeRoot->offsetValueCode = 900;
            }else{
                subtreeRoot->offsetValueCode=computeOffSetValueCode(subtreeRoot->winnerKey, subtreeRoot->key);
            }
            

        } else {
            if (subtreeRoot-> key == ""){
                subtreeRoot-> key = maxKey;
                subtreeRoot-> winnerKey = maxKey;
            }
        }
        return subtreeRoot;
    }

    int computeOffSetValueCode (std::string smallerKey, std::string key2){
        int offset = 0;

        while (offset < 8 && smallerKey[offset] == key2[offset]){
            offset++; 
        }
        
        int offsetValueCode = (8-offset)*100 + ((key2[offset] - 0) - 48); //ASCII 0 is 48
        if (key2==maxKey){
            offsetValueCode=900;
        }
        //std::cout << "Node level offsetvalue code " << (key2[offset] - 48) << " dl " << offsetValueCode << "\n";

        return offsetValueCode;
    }

    uint64_t spillAll (std::vector< std::queue<NRecord> >& binList) {
        int freedspace = 0;
        std::string lastPopedKey = "";
        
        std::cout << "\n\n\n I was here in spillAll " << root->winnerKey << "\n";
        print("",root,false);
        
        
        while (root->winnerKey != maxKey){
            
            lastSpilledKey = root->winnerKey;
            
            /*
            while (!binList[root->winnerIndex].empty() && binList[root->winnerIndex].front().get_key() < root->key){                        
                lastPopedKey = binList[root->winnerIndex].front().get_key();
                //TODO: add record to next layer
                std::cout << "\n\n The next poped value: "<<binList[root->winnerIndex].front().get_value() << "\n";
                freedspace += binList[root->winnerIndex].front().get_size();
                binList[root->winnerIndex].pop();
            } */
            lastPopedKey = binList[root->winnerIndex].front().get_key();
                //TODO: add record to next layer
            std::cout << "\n\n The next poped value: "<<binList[root->winnerIndex].front().get_value() << "\n";
            freedspace += binList[root->winnerIndex].front().get_size();
            binList[root->winnerIndex].pop();
            numberOfspilled++;
            if (binList[root->winnerIndex].empty()){
                leafNodes[root->winnerIndex]->key = maxKey;
                leafNodes[root->winnerIndex]->winnerKey = maxKey;
            } else {
                leafNodes[root->winnerIndex]->key = binList[root->winnerIndex].front().get_key();
                leafNodes[root->winnerIndex]->winnerKey = binList[root->winnerIndex].front().get_key();
            }
    
            int tempOffsetValueCode = computeOffSetValueCode(lastSpilledKey, leafNodes[root->winnerIndex]->winnerKey);
            bubbleUp(leafNodes[root->winnerIndex], tempOffsetValueCode, root-> winnerIndex, binList);
            print("", root, false);
            //push new key through tree and get a new winner. 

        }
        
        if (lastSpilledKey != lastPopedKey && lastPopedKey != ""){
            lastSpilledKey = lastPopedKey;
        }
        std::cout << freedspace<< "freed space \n";
        return freedspace;
        
    }


    int spilltoFreeSpace (int inRecordSize, std::vector< std::queue<NRecord> >& binList) {
        int freedspace = 0;
        int spaceNeeded = inRecordSize;
        std::string lastPopedKey = "";
        while (spaceNeeded > 0){
            if (root->winnerKey == maxKey){
                break;
            }
            lastSpilledKey = root->winnerKey;
            std::cout << root->winnerKey << "new round ";
            
            //while (!binList[root->winnerIndex].empty() && binList[root->winnerIndex].front().get_key() < root->key){
            spaceNeeded -= binList[root->winnerIndex].front().get_size();
                        
            lastPopedKey = binList[root->winnerIndex].front().get_key();
                //TODO: add record to next layer
            std::cout << "\n The next poped value: "<<binList[root->winnerIndex].front().get_value() << "\n";
            freedspace += binList[root->winnerIndex].front().get_size();
            binList[root->winnerIndex].pop();
            numberOfspilled++;
            
            
            if (binList[root->winnerIndex].empty()){
                leafNodes[root->winnerIndex]->key = maxKey;
                leafNodes[root->winnerIndex]->winnerKey = maxKey;
            } else {
                leafNodes[root->winnerIndex]->key = binList[root->winnerIndex].front().get_key();
                leafNodes[root->winnerIndex]->winnerKey = binList[root->winnerIndex].front().get_key();
            }
    
            int tempOffsetValueCode = computeOffSetValueCode(lastSpilledKey, leafNodes[root->winnerIndex]->winnerKey);
            bubbleUp(leafNodes[root->winnerIndex], tempOffsetValueCode, root-> winnerIndex, binList);
            print("", root, false);
            //push new key through tree and get a new winner. 

        }
        
        if (lastSpilledKey != lastPopedKey){
            lastSpilledKey = lastPopedKey;
        }
        std::cout << freedspace<< "freed space \n";
        //print("", root, false);
        return freedspace;
        
    }

    void pushBufferBinFirstRecord(std::vector< std::queue<NRecord> >& binList){
        leafNodes[0] -> key = binList[0].front().get_key();
        leafNodes[0] -> winnerKey = binList[0].front().get_key() ;
        leafNodes[0] -> winnerIndex = 0;
        //int tempOffsetValueCode = computeOffSetValueCode(leafNodes[0] -> parent -> winnerKey, leafNodes[0] ->key);
        bufferReplaceBubbleUp(leafNodes[0], 0 , binList);
    }
        

    void bubbleUp (NTournamentTreeNode * currentNode, int tempOffsetValueCode, int index, std::vector< std::queue<NRecord> >& binList){
        if (currentNode == root){return;}

        if (tempOffsetValueCode < currentNode->parent->offsetValueCode){
            currentNode->parent-> winnerIndex = index;
            currentNode->parent-> winnerKey = currentNode->winnerKey;
            currentNode->parent-> offsetValueCode = computeOffSetValueCode (currentNode->parent-> winnerKey, currentNode->parent-> key);
            bubbleUp(currentNode->parent, tempOffsetValueCode, index, binList);
        } else if (tempOffsetValueCode > currentNode->parent->offsetValueCode){
            currentNode->parent -> winnerIndex = currentNode ->parent-> index;
            currentNode->parent -> winnerKey = currentNode -> parent -> key;
            currentNode->parent -> key = currentNode -> winnerKey;
            currentNode ->parent -> index = currentNode -> winnerIndex;
            tempOffsetValueCode = currentNode->parent-> offsetValueCode;
            currentNode->parent-> offsetValueCode = computeOffSetValueCode (currentNode->parent-> winnerKey, currentNode->parent-> key);
            bubbleUp(currentNode->parent, tempOffsetValueCode, currentNode->parent -> winnerIndex, binList);
        } else if (!binList[currentNode->winnerIndex].empty() && (!binList[currentNode->parent->index].empty()) 
                    && binList[currentNode->winnerIndex].front().get_value() < binList[currentNode->parent->index].front().get_value()){
            currentNode->parent-> winnerIndex = index;
            currentNode->parent-> winnerKey = currentNode->winnerKey;
            currentNode->parent-> offsetValueCode = computeOffSetValueCode (currentNode->parent-> winnerKey, currentNode->parent-> key);
            bubbleUp(currentNode->parent, tempOffsetValueCode, index, binList);
        } else {
            currentNode->parent -> winnerIndex = currentNode ->parent-> index;
            currentNode->parent -> winnerKey = currentNode -> parent -> key;
            currentNode->parent -> key = currentNode -> winnerKey;
            currentNode ->parent -> index = currentNode -> winnerIndex;
            tempOffsetValueCode = currentNode->parent-> offsetValueCode;
            currentNode->parent-> offsetValueCode = computeOffSetValueCode (currentNode->parent-> winnerKey, currentNode->parent-> key);
            bubbleUp(currentNode->parent, tempOffsetValueCode, currentNode->parent -> winnerIndex, binList);
        }
    }

    //When an active tree node is repaced. 
    void bufferReplaceBubbleUp (NTournamentTreeNode * currentNode, int index, std::vector< std::queue<NRecord> >& binList){
        if (currentNode == root){return;}
        

        //need to redo comparison give up offsetvaluecoding
        if (currentNode-> parent -> left -> winnerKey < currentNode -> parent -> right -> winnerKey) {
            currentNode-> parent -> winnerIndex = currentNode ->parent-> left -> winnerIndex;
            currentNode-> parent -> winnerKey = currentNode -> parent -> left -> winnerKey;
            currentNode-> parent -> key = currentNode -> parent -> right -> winnerKey;
            currentNode ->parent -> index = currentNode ->parent -> right -> winnerIndex;
            bufferReplaceBubbleUp(currentNode->parent, currentNode->parent->winnerIndex, binList);
        } else if (currentNode-> parent -> left -> winnerKey > currentNode -> parent -> right -> winnerKey) {
            currentNode-> parent -> index = currentNode ->parent-> left -> winnerIndex;
            currentNode-> parent -> key = currentNode -> parent -> left -> winnerKey;
            currentNode-> parent -> winnerKey = currentNode -> parent -> right -> winnerKey;
            currentNode ->parent -> winnerIndex = currentNode ->parent -> right -> winnerIndex;
            bufferReplaceBubbleUp(currentNode->parent, currentNode->parent->winnerIndex, binList);
        } else{
            if (binList[currentNode-> parent -> left -> winnerIndex].front().get_value() < binList[currentNode-> parent -> right -> winnerIndex].front().get_value()){
                currentNode-> parent -> winnerIndex = currentNode ->parent-> left -> winnerIndex;
                currentNode-> parent -> winnerKey = currentNode -> parent -> left -> winnerKey;
                currentNode-> parent -> key = currentNode -> parent -> right -> winnerKey;
                currentNode ->parent -> index = currentNode ->parent -> right -> winnerIndex;
                bufferReplaceBubbleUp(currentNode->parent, currentNode->parent->winnerIndex, binList);
            }else {//if (binList[currentNode-> parent -> left -> winnerIndex] > binList[currentNode-> parent -> right -> winnerIndex]) {
                currentNode-> parent -> index = currentNode ->parent-> left -> winnerIndex;
                currentNode-> parent -> key = currentNode -> parent -> left -> winnerKey;
                currentNode-> parent -> winnerKey = currentNode -> parent -> right -> winnerKey;
                currentNode ->parent -> winnerIndex = currentNode ->parent -> right -> winnerIndex;
                bufferReplaceBubbleUp(currentNode->parent, currentNode->parent->winnerIndex, binList);
            }
        }
    }
    
    
};

#endif