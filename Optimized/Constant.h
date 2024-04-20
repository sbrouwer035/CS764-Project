#ifndef CONSTANT_H
#define CONSTANT_H

#include <inttypes.h>
#include <stdio.h>
/*real stats
const uint64_t CACHE_SIZE = 1 << 20;              //1MB Cache
const uint64_t DRAM_SIZE = 100 << 20;             //100MB DRAM
const uint64_t SSD_SIZE = ((uint64_t)10) << 30;   //10 GB SSD
const int SSD_Bandwidth = 210;                    //200 MB/s => 210 byte/us SSD
const int HDD_Bandwidth = 105;                    //100 MB/s => 105 byte/us HDD
const int SSD_LATENCY = 100;                      //100 micro second SSD or 0.1 ms
const int HDD_LATENCY = 5000;                     //5000 micro second HDD or 5 ms
*/

//testing stats
const uint64_t CACHE_SIZE = 4000;              //4000b
const uint64_t DRAM_SIZE = 40000;              //40000b
const uint64_t SSD_SIZE = 4000000;               //400000b
const int SSD_Bandwidth = 210;                    //200 MB/s => 210 byte/us
const int HDD_Bandwidth = 105;                    //100 MB/s => 105 byte/us
const int SSD_LATENCY = 100;                      //100 micro second
const int HDD_LATENCY = 5000;                     //5000 micro second

const std::string maxKey = "zzzzzzzzz";             //

#endif