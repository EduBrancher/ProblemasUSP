#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define PROMPT "[ep3]: "

#define BLOCK_SIZE 4096 
#define BLOCK_TERMINATOR "_"

#define MAX_SIZE (100e6)
#define FILE_METADATA_SIZE 142 
#define DIRECTORY_METADATA_SIZE 121
#define MAX_NAME 30

#define ARQ_SEPARATOR "|"

#define TERMINATOR "$-1$" 
#define FAT_MAX_SIZE 135389
#define BITMAP_SIZE 6 // em blocos

const int N_BLOCKS = MAX_SIZE/BLOCK_SIZE; 

#endif