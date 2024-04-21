# CS764-Project
External merge sort project for CS 764 Spring 2024

In the Optimized folder: 
Tournament Trees: the code is in NTournamentTree.cpp and .h
Graceful degradation: in the function addRecord in file NDevice.cpp. The function will then utilize the tournament tree code for tree construction, utilize a buffer leaf bin to add entries that are qualified in the output bin, and handle spilling to free up space for new records. 

Replacement Selection: in the NTournamentTree.cpp file and function pushBufferBinFistRecord and bufferReplacementBubbleUp
Offset-value coding: in the NTournamentTree.cpp file and function bubbleUp
Variable-size Records: command with the last parameter set to -1. ex.  ".\Test.exe -c 99 -s -1" This command will generate 99 records with variable size between 8 to 2000 bytes.  
    in generateInput function in the Test.cpp file. 

Compression and Prefix truncation: in the Offset-value coding logic and key comparison logic
Minimum count of row: in the Tournament Tree logic and variable sized bins (Graceful degradation fully implemented) for maximun efficiency. 
Duplicate Removal: mergeTree function in NTournamentTree.cpp file for in merge removal and throughout the program. 
Cache-size mini runs (this is not very efficient): The base file does this, the optimize file uses graceful degradation even for cache level merge. 
Spilling memory-to-SSD: in spilling logic and Graceful Degradation logic.
Spilling SSD-to-disk: in spilling logic and Graceful Degradation logic. I did not code for 12 GB (this is very bad for my old laptop). Code can be easily expanded to cover any arbitrary large files easily. 
Optimized merge patterns: in bubbleUp functions and Tournament Tree logic. 
Verify sort orders: //----------------------------Validation in the main function in Test.cpp file. 
	  #pragma region validation
	  verifyDataIntegrity();
	  verifySortOrder();
	  #pragma endregion validation
