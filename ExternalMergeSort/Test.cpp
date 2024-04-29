#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <queue>
#include "Constant.h"
#include "defs.h"
#include "verification.h"
#include "NRecord.h"
#include "NTournamentTree.h"
#include "NTournamentTreeNode.h"
#include "NDevice.h"



bool generateInput(int numberOfRecords, int recordSize);
void runExternalSort(NDevice& cache, NDevice& outputDevice);


int main (int argc, char * argv [])
{	// extract command line parameters
	int numberOfRecords = 0;
	int recordSize = 0;
	char* traceFile = NULL;
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
			traceFile = argv[i+1];
			remove(traceFile);
			std::fstream traceFileStream;
			traceFileStream.open(traceFile, std::ios::out);
			if (!traceFileStream.is_open()){
        		std::cout<<"Error: failed to open the trace file: " << traceFile << "\n";
        		return false;
    		}
			traceFileStream.close();
		}
	}
	
	// generate records, write to input_table
	if (recordSize < 0) {
		printf("Generating %d records of variable size...\n",numberOfRecords);
	}
	else {
		printf("Generating %d records of size %d...\n",numberOfRecords,recordSize);
	}
	if (!generateInput(numberOfRecords,recordSize)){
		std::cout << "Error: having issue generating input. Please investigate. \n";
		return 0;
	};
	printf("Record generation complete!\n");

	
	// sort records, write to output_table
	printf("Sorting records...\n");
	NDevice cache (CACHE_SIZE, "CACHE", traceFile, 0, 0);
	NDevice DRAM (DRAM_SIZE, "DRAM", traceFile, 0, 0);
	NDevice SSD (SSD_SIZE, "SSD", traceFile, SSD_LATENCY, SSD_Bandwidth);
	NDevice HDD (HDD_SIZE, "HDD", traceFile, HDD_LATENCY, HDD_Bandwidth);	
	NDevice outputDevice(HDD_SIZE, "output_table", traceFile, 0, 0);
	cache.nextLevelDevice = &DRAM;
	DRAM.nextLevelDevice = &SSD;
	SSD.nextLevelDevice = &HDD;
	runExternalSort(cache, outputDevice);
    printf("Sorting complete!\n");

	// validate output
	printf("Validating output...\n");
	verifyDataIntegrity();
	verifySortOrder();
	printf("Validation complete!\n");

	return 0;
} // main

bool generateInput(int numberOfRecords, int recordSize){
	
	remove("input_table");
	remove("output_table");
	
	std::fstream createdOutputFile;
	createdOutputFile.open("output_table", std::ios::out|std::ios::binary);
	if (!createdOutputFile.is_open()){
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
	return true;
}

void runExternalSort(NDevice& cache, NDevice& outputDevice){
	
	std::ifstream inputfile;
    inputfile.open("input_table", std::ios::out);
	
    if (!inputfile.is_open()){
		traceprintf ("Error: fail to open the input_table. \n");
		return;
    }
    
    if (inputfile.peek() == std::ifstream::traits_type::eof()){
        std::cout << "\t... input_table has zero entries: default sorted. \n";
		return;
    }

    std::string currentVal;
    
    while(std::getline(inputfile, currentVal)){
        NRecord currentRecord (currentVal.substr(0,8),currentVal,currentVal.length());		
		cache.addRecord(currentRecord);
    }
	cache.end_Device(outputDevice);
	cache.nextLevelDevice->end_Device(outputDevice);									//DRAM
	cache.nextLevelDevice->nextLevelDevice->end_Device(outputDevice);					//SSD
	cache.nextLevelDevice->nextLevelDevice->nextLevelDevice->end_Device(outputDevice);  //HDD

    inputfile.close();
}

