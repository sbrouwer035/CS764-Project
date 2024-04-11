#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include "Constant.h"
#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "verification.h"


bool generateInput(int numberOfRecords, int recordSize);
void testTree();

int main (int argc, char * argv [])
{	
	std::cout << CACHE_SIZE << "\n";
	std::cout << DRAM_SIZE << "\n";
	std::cout << SSD_SIZE << "\n";
	std::cout << SSD_Bandwidth << "\n";

	//--------------------------Generate input file input_table, create output files output_table and trace0.txt
	#pragma region filePrep
	int numberOfRecords;
	int recordSize;

	if (atoi(argv[4]) > 2000) {
		recordSize = 2000;
	} else{
		recordSize = atoi(argv[4]);
	}

	numberOfRecords = atoi(argv[2]);
	
	if (!generateInput(numberOfRecords,recordSize)){
		std::cout << "Error: having issue generating input. Please investigate.";
		return 0;
	};
    #pragma endregion filePrep


	TRACE (true);

	/*
	Plan * const plan = new ScanPlan (7);
	// new SortPlan ( new FilterPlan ( new ScanPlan (7) ) );

	Iterator * const it = plan->init ();
	it->run ();

	delete it;
	delete plan;
	*/
    
	//----------------------------Validation
	#pragma region validation
	//verifyDataIntegrity();
	//verifySortOrder();

	#pragma endregion validation

	// Test Tags
	testTree();

	return 0;
} // main

bool generateInput(int numberOfRecords, int recordSize){
	std::fstream createdOutputFile;
	createdOutputFile.open("output_table", std::ios::out|std::ios::binary);
	if (!createdOutputFile.is_open()){
        std::cout<<"Error: fail to open the file. \n";
        return false;
    }

	std::fstream createdOutputTraceFile;
	createdOutputTraceFile.open("trace0.txt", std::ios::out|std::ios::binary);
	if (!createdOutputTraceFile.is_open()){
        std::cout<<"Error: fail to open the file. \n";
        return false;
    }
	
	std::fstream generatedInput;
	generatedInput.open("input_table", std::ios::out|std::ios::binary);
	if (!generatedInput.is_open()){
        std::cout<<"Error: fail to open the file. \n";
        return false;
    }

	static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < numberOfRecords; i++){
		std::string temp;
    	temp.reserve(recordSize);
		for (int j = 0; j < recordSize; ++j) {
        	temp += alphanum[rand() % (sizeof(alphanum) - 1)];
		}
		generatedInput << temp;
		generatedInput <<std::endl;
	}
    generatedInput.close();	
	createdOutputFile.close();
	createdOutputTraceFile.close();
	return true;
}

void testTree() {
	std::cout << "In testTree\n";
	Run * run1 = new Run();
	run1->addValue("000Z");
	Run * run2 = new Run();
	run2->addValue("100Z");
	Run * run3 = new Run();
	run3->addValue("a00Z");
	Run * run4 = new Run();
	run4->addValue("A00Z");
	Run * runlist[] = {run1, run2, run3, run4};
	SortTree * mergeTree = new SortTree(runlist, 4);
	std::cout << "tree built\n";
	std::string value = mergeTree->nextValue();
	while (value != MAX_VALUE) {
		std::cout << value << std::endl;
	}
	delete run1;
	delete run2;
	delete run3;
	delete run4;
	delete mergeTree;
	return;
}

