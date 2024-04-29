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
    std::cout << "\t... Output is sorted\n";
    outputfile.close();
    return true;
}


//Make sure the outputfile has all inputfile records 
bool verifyDataIntegrity(){
    std::string passStr = "\t... All input keys are present in output\n";
    std::string errorStr = "Validation error: not all input keys present in output\n";

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
            std::cout << passStr;
            return true;
        } else {
            std::cout << errorStr;
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
            std::cout << currentVal<< "\n";
            std::cout << errorStr;
            return false;
        }
    }
    std::cout << passStr;
    outputfile.close();
    inputfile.close();
    return true;
}

