# Software Requirements Specification
## Nucleus Memory Manager | Nov, 2022
---
### [Goncharov Mark](https://github.com/MarkGoncharovAl) - MIPT
### [Kuznetsova Margarita](https://github.com/Margarita-Kuznetsova) - MIPT
### [Shcherbakov Alexey](https://github.com/pr0kuk) - MIPT
---
# Content
1. [Purpose](#purpose)
    1. [Definitions](#definitions)
    2. [Background](#background)
    3. [System overview](#system-overview)
    4. [References](#references)
2. [Overall description](#overall-description)
    1. [Product perspective](#product-perspective)
    2. [Product functions](#product-functions)
    3. [User characteristics](#user-characteristics)
    4. [Constraints, assumptions and dependencies](#constraints-assumptions-and-dependencies)
3. [Specific requirements](#specific-requirements)
    1. [External interface requirements](#external-interface-requirements)
    2. [User interface](#user-characteristics)
    3. [Hardware interface](#hardware-interface)
    4. [Software interface](#software-interface)
    5. [Functional requirements](#functional-requirements)
    6. [Non-Functional requirements](#non-functional-requirements)

# Purpose
## Definitions
User: Someone who interacts with the Nucleus MM library  
OS: Operating System  
DESC: Description  
RAT: Rational  
DEP: Dependency  
## Background
During the execution of a binary translation, the intermediate representation can change arbitrarily, for which the translator needs to constantly allocate and free memory, and it is safe to do so. The standard memory manager is either too slow or insecure.
## System overview
The memory manager is a C library that includes the Region class with which the user is able to allocate, use, and release memory as needed quickly and safely.
## References
[1] IEEE Software Engineering Standards Committee, “IEEE Std 830-1998, IEEE Recommended Practice for Software Requirements Specifications”, October 20, 1998.  
[2] Davis M A, “Just Enough Requirements Management: Where Software Development Meets
Marketing”, New York, Dorset House Publishing, 2005.

# Overall description
## Product perspective
The user interacts with the Nucleus library to perform the operations he needs, Nucleus accesses the standard C++20 library, which in turn passes the request to the OS, that performs the necessary memory operation.
## Product functions
### Init table
The user selects the size of data to work.
The system creates the table.
### Allocate memory
The system returns index to data and allocates memory if necessary.
### Get pointer
The user selects index.
The system returns associated pointer to data. 
### Free memory
The user selects the memory to free.
The system frees memory.
### Destroy table
The system frees all memory that was allocated with the table
## User characteristics
The user is expected to use C and to know the descriptions of all methods of the Region class.
## Constraints, assumptions and dependencies
The manager's work is limited by the physical size of the memory and the size allocated by the operating system. Also, to use the library, a basic knowledge of C is assumed, as well as the correct use of the functions of the library.
# Specific requirements
## External interface requirements
No external interfaces required.
## User interface
Functions of library:
* init_table
* allocate
* get
* free
* destroy
## Hardware interface
No hardware interfaces required.
## Software interface
libc.
## Functional requirements
### ID: FR1
TITLE: Initialize table 
DESC: A user should be able to choose size of memory block.
RAT: In order to use the libraries functions.   
DEP: None  

### ID: FR2 
TITLE: Allocate memory  
DESC: Given that the table is initialized, then the user should be able to allocate memory.  
RAT: In order for a user to allocate memory.  
DEP: FR1  

### ID: FR3 
TITLE: Get pointer  
DESC: Given that the memory is allocated, then the user should be able to get pointer to the allocated space by index. The system should check the index.  
RAT: In order for a user to read/write data.  
DEP: FR2  

### ID: FR4 
TITLE: Free memory  
DESC: Given that the memory is allocated, then the user should be able to free the memory. The system should check the fact of allocation.  
RAT: In order for a user to free memory.  
DEP: FR2  

### ID: FR5 
TITLE: Destroy table
DESC: Given that the table is initialized then the user should be able to free all the memory in it..  
RAT: In order for a user to free all memory.  
DEP: FR1
## Non-Functional requirements

### ID: QR1
TITLE: Secure access  
DESC: The system should recognize incorrect references.  
RAT: In order for a user to read correct data.  
DEP: None  

### ID: QR2
TITLE: Secure write  
DESC: The system should recognize attempts to write out of allocate bounds.  
RAT: In order for a user to secure writing.  
DEP: None  

### ID: QR3
TITLE: Manage memory leaks  
DESC: The system should recognize unfreed memory.  
RAT: In order to secure user from leaks.  
DEP: None  