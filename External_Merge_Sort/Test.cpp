#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "verification.h"

bool generateInput(int numberOfRecords, int recordSize);

int main (int argc, char * argv [])
{	
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

	Plan * const plan = new ScanPlan (7);
	// new SortPlan ( new FilterPlan ( new ScanPlan (7) ) );

	Iterator * const it = plan->init ();
	it->run ();

	delete it;
	delete plan;
    
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

