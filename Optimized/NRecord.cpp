#include "NRecord.h"

NRecord::NRecord (std::string in_key, std::string in_value, int size)
    : key(in_key), value(in_value), record_size(size)
{    
}

NRecord::~NRecord()
{
}
