#ifndef NRecord_H
#define NRecord_H
#include <string>

class NRecord
{  
public:
    NRecord(std::string in_key, std::string in_val, int size);
    ~NRecord();

    //getters
    std::string get_key()const{
        return key;
    }

    std::string get_value()const{
        return value;
    }

    int get_size()const{
        return record_size;
    }

private: 
    std::string key;
    std::string value;
    int record_size;
};

#endif