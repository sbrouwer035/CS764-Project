# CS764-Project
External merge sort project for CS 764 Spring 2024  
Shawn Feng, Shawn Brouwer  

## Running program
./ExternalSort.exe -c <record count> -s <record size> -o <trace filename>  
-c Record Count (required)  
-s Record Size (required), for variable sized records pass -1  
-s Trace Filename (optional), details on reads and writes to/from SDD and HDD will be written to this file  

Example 1: Sort 2000 records of size 1024b, output trace information to trace0.txt    
./ExternalSort.exe -c 2000 -s 1024b -o trace0.txt

Example 2: Sort 500 records of variable size, output trace information to trace1.txt  
./ExternalSort.exe -c 500 -s -1 -o trace1.txt  

Overview:
The first step of the sort is to generate random alphanumeric values of the given size. The generated input data is written to the "input_table". In general, the data is read from the "input_table", sorted, and then written to the "output_table". While reading in the data, it is sorted into runs or bins which are implemented using FIFO queues (in the code, all run-related variables have “bin” in the name). Queues entries are added in ascending order. Variable-sized bins were used to maximize bin utilization and reduce bin numbers. This strategy is implemented at every storage level. When the storage level is full, a tournament tree is created to spill records in sorted order to the next storage level. This process is repeated until all records have been read in and sorted. The sorted output is written to the "output_table".


Tournament Trees
    Implementation: NTournamentTree.cpp
    Details: build a tree with enough leaf nodes to cover the number of available bins (not including the side bin). To improve efficiency, the tree is not forced to be balanced. The tree employs the "tree of losers" concept to pop the lowest sorted record from it's root.

Replacement Selection
    Implementation: NTournamentTree::pushBufferBinFistRecord, NTournamentTree::bufferReplacementBubbleUp (NTournamentTree.cpp)

Variable-size Records
    Implmentation: NRecord::get_size() (NRecord.h but referenced throughout program)
    Details: Each record is stored as an object with it's size being one of the class members. Whenever reading in or spilling records we check the size of the record and proceed accordingly.

Offset-value Coding / Prefix Truncation
    Implementation: NTournamentTree::computeOffSetValueCode, NTournamentTree::bubbleUp (NTournamentTree.cpp)
    Details: Offset-value coding in a sense is prefix Truncation as it uses the first diff position for calculation.

Duplicate Removal
    Implementation: Duplicate checks occur throughout the program to try to eliminate them as early as possible (E.g. NDevice.cpp:110)

Spilling to larger storage devices
    Implementation: NTournamentTree::spilltoFreeSpace, NTournamentTree::spillAll (NTournamentTree.cpp)

Graceful Degradation
    Implementation: NDevice::addRecord (NDevice.cpp)
    Details: When a new record will not fit into memory we'll spill enough for it to fit. If this process repeats enough times we assume there are a lot more records coming and we'll spill everything. More specifically, if the new record sorts before the last spilled record (leveraging offset-value coding, and keys/values for comparisons), the record goes into the side bin. If the record can go into the current output run, add the record to the buffer bin (first leaf of tournament tree). If the new record can’t go into either bin, spill all records to the next storage level. The new record is stored as the first record in the, now empty, current storage level. This process will repeat as needed.

Verifying Sort Order
    Implementation: Verification.cpp

Cache-size Mini Runs
    Details: Not directly enforced but still occurs as a result of the implementation of how records are spilled to higher storage levels.

Device-optimized page sizes
    Details: This program doesn't focus on the specifics of optimizing I/O. The reads and writes are expressed in terms of individual events (spilling one record or spilling a group of records, etc.) rather than holding records in a page-sized buffer and then keeping track of page-sized reads and writes.

Optimized Merge Patterns
    Details: This is baked into the merge logic with the use of the tournament tree and offset-value coding.

Minimum Count of Row
    Details: Not all that relevant for this program because of the use of variable-sized bins.

Not Implemented:
    Quicksort
    Run Size > Memory Size
    Compresssion