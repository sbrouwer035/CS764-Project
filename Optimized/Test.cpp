#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <queue>
#include "Constant.h"
#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "verification.h"
#include "NRecord.h"
#include "NTournamentTree.h"
#include "NTournamentTreeNode.h"
#include "NDevice.h"



bool generateInput(int numberOfRecords, int recordSize);
void runExternalSort(NDevice& cache);


int main (int argc, char * argv [])
{	
	TRACE (true);
	//--------------------------Generate input file input_table, create output files output_table and trace0.txt
	#pragma region filePrep
	int numberOfRecords = 0;
	int recordSize = 0;

	// extract command line parameters
	for (int i=1; i<argc; i++) {
		std::string arg(argv[i]);

		// "-c" gives the total number of records
		if (arg == "-c") {
			numberOfRecords = atoi(argv[i+1]);
		}

		// "-s" is the individual record size
		if (arg == "-s") {
			recordSize = atoi(argv[i+1]);
			if (recordSize > 2000) {
				recordSize = 2000;
			}
		}

		// "-o" is the trace of your program run
		if (arg == "-o") {
			// do trace stuff
		}
	}
	
	if (!generateInput(numberOfRecords,recordSize)){
		std::cout << "Error: having issue generating input. Please investigate. \n";
		return 0;
	};
	traceprintf("input_table generated. \n");
    #pragma endregion filePrep

	
	#pragma region ExternalSort
	NDevice cache (CACHE_SIZE,"CACHE");
	NDevice DRAM (DRAM_SIZE,"DRAM");
	NDevice SSD (SSD_SIZE,"SSD");
	NDevice HDDtemp(HDD_SIZE,"HDDtemp");
	NDevice HDD (HDD_SIZE,"HDD");
	cache.nextLevelDevice = &DRAM;
	DRAM.nextLevelDevice = &SSD;
	SSD.nextLevelDevice = &HDDtemp;
	HDDtemp.nextLevelDevice = &HDD;
	runExternalSort(cache);

	#pragma endregion ExternalSort

	//----------------------------Validation
	#pragma region validation
	verifyDataIntegrity();
	verifySortOrder();
	#pragma endregion validation

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
	//good alphanum

	//test alphanum for dup test
	//static const char alphanum[] =
    //    "bc";

	int currentRecordSize = 0;
	for (int i = 0; i < numberOfRecords; i++){
		std::string temp;
    	if (recordSize < 0){
			currentRecordSize = rand() % 1992 + 8;
		}else {
            currentRecordSize = recordSize;
		}
		temp.reserve(currentRecordSize);
		for (int j = 0; j < currentRecordSize; ++j) {
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

void runExternalSort(NDevice& cache){
	
	std::ifstream inputfile;
    inputfile.open("input_table", std::ios::out);
	TRACE (true);
	
    if (!inputfile.is_open()){
		traceprintf ("Error: fail to open the input_table. \n");
    }
    
    if (inputfile.peek() == std::ifstream::traits_type::eof()){
        traceprintf ("input_table has zero entries: default sorted. \n");
		return;
    }

    std::string currentVal;
    
    while(std::getline(inputfile, currentVal)){
        NRecord currentRecord (currentVal.substr(0,8),currentVal,currentVal.length());
		//std::cout<< "this is my new record" << currentRecord.get_size() << currentRecord.get_key()<< cache.numberofBin << "\n";
		
		cache.addRecord(currentRecord);
    }
	cache.end_Device();
	cache.nextLevelDevice->end_Device();
	cache.nextLevelDevice->nextLevelDevice->end_Device();
	cache.nextLevelDevice->nextLevelDevice->nextLevelDevice->end_Device();

    traceprintf ("Reaching end of the input_table. All entries are sorted! \n");
    inputfile.close();
}

