# Nucleus Memory Manager | Nov, 2022
---
### [Goncharov Mark](https://github.com/MarkGoncharovAl) - MIPT
### [Kuznetsova Margarita](https://github.com/Margarita-Kuznetsova) - MIPT
### [Shcherbakov Alexey](https://github.com/pr0kuk) - MIPT
---

# Requirements
* GCC 9.4.0


# Use
To use nucleus lib include header to your source file:

    #include "nucleusN.hpp"
## Structures description
`table` - A structure that is a parameter to all functions.  
The table stores all pointers to the data.  
Each data type uses its own table.

`index` - A structure consists of 3 integer numbers.  
Number of block, number of cell in block and generation of pointer.
## Function description
`struct table init_table(int cellsize)` - initializes table by size of one element of data.  
`struct index allocate(struct table* tb)` - allocates cell.  
`void* get(struct table* tb, struct index* in)` - returns pointer to data by index.  
`ErrCode Free(struct table* tb, struct index* in)` - frees cell by index.  
`ErrCode destroy(struct table* tb)` - frees all memory in the table.

# Examples
One can see examples in `tests.cpp`
# Test
## Requirements
* GCC 9.4.0
* CMAKE 3.16.3
## Build
	cmake ./
	make
## Run
    ./nucleus