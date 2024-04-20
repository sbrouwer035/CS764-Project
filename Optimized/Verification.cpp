#include <string>
#include <fstream>
#include <iostream>
#include <set>


//Make sure the outputfile has records sorted
bool verifySortOrder(){
    std::ifstream outputfile;
    outputfile.open("output_table", std::ios::out);

    if (!outputfile.is_open()){
        std::cout<<"Error: fail to open the outputfile. \n";
        return false;
    }
    
    if (outputfile.peek() == std::ifstream::traits_type::eof()){
        return true;
    }

    std::string previousVal;
    std::string currentVal;
    
    while(std::getline(outputfile, currentVal)){
        if(previousVal >= currentVal){
            std::cout << "Validation error: the output is not sorted. \n";
            return false;
        }
        previousVal = currentVal;
    }
    std::cout << "Output is sorted. Validation success. \n";
    outputfile.close();
    return true;
}


//Make sure the outputfile has all inputfile records 
bool verifyDataIntegrity(){
    std::ifstream outputfile;
    outputfile.open("output_table", std::ios::out);
    
    std::ifstream inputfile;
    inputfile.open("input_table", std::ios::out);
    
    if (!outputfile.is_open() || !inputfile.is_open()){
        std::cout<<"Error: fail to open the files. \n";
        return false;
    }

    if (outputfile.peek() == std::ifstream::traits_type::eof()){
        if(inputfile.peek() == std::ifstream::traits_type::eof()){
            std::cout << "All input rows are found in output. Validation pass. \n";
            return true;
        } else {
            std::cout << "Validation error: not all inputs can be found in the output file. \n";
            return false;
        }
    }
    
    std::set <std::string> outputSet;
    std::string currentVal;

    while(std::getline(outputfile, currentVal)){
        outputSet.insert(currentVal);
    }
    
    currentVal="";
    while(std::getline(inputfile,currentVal)){
        if (outputSet.find(currentVal) == outputSet.end()){
            std::cout << "Validation error: not all inputs can be found in the output file. \n";
            return false;
        }
    }
    std::cout << "All input rows are found in output. Validation pass. \n";
    outputfile.close();
    inputfile.close();
    return true;
}

