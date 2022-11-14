# Software Requirements Specification
## Nucleus Memory Manager | Nov, 2022
---
### Goncharov Mark - MIPT
### Kuznetsova Margarita - MIPT
### Shcherbakov Alexey - MIPT
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
STL: Standard Template Library of C++  
DESC: Description  
RAT: Rational  
DEP: Dependency  
## Background
During the execution of a binary translation, the intermediate representation can change arbitrarily, for which the translator needs to constantly allocate and free memory, and it is safe to do so. The standard memory manager is either too slow or insecure.
## System overview
The memory manager is a C++ library that includes the Region class with which the user is able to allocate, use, and release memory as needed quickly and safely.
## References
[1] IEEE Software Engineering Standards Committee, “IEEE Std 830-1998, IEEE Recommended
Practice for Software Requirements Specifications”, October 20, 1998.
[2] Davis M A, “Just Enough Requirements Management: Where Software Development Meets
Marketing”, New York, Dorset House Publishing, 2005.

# Overall description
## Product perspective
The user interacts with the Nucleus library to perform the operations he needs, Nucleus accesses the standard C++20 library, which in turn passes the request to the OS, that performs the necessary memory operation.
## Product functions
### Allocate memory
The user selects the type of data to be allocated.
The system allocates memory.
### Write data
The user selects a location for the recording.
The user specifies the data to write.
The system writes to the specified location, if possible.
### Read data
The user selects the location and type of data.
The system provides data for the specified location.
### Free memory
The user selects the memory to free.
The system frees memory.
## User characteristics
The user is expected to use C++20 and to know the descriptions of all methods of the Region class.
## Constraints, assumptions and dependencies
The manager's work is limited by the physical size of the memory and the size allocated by the operating system. Also, to use the library, a basic knowledge of C ++ is assumed, as well as the correct use of the functions of the library.
# Specific requirements
## External interface requirements
No external interfaces required.
## User interface
Methods of Region class:

        * allocate
        * write
        * read
        * free
## Hardware interface
No hardware interfaces required.
## Software interface
C++20 STL.
## Functional requirements
### ID: FR1
TITLE: Download Nucleus MM  
DESC: A user should be able to download the library through the Internet or get it through another sources.  
RAT: In order for a user to download the library.   
DEP: None  

### ID: FR2 
TITLE: Allocate memory  
DESC: Given that the library is downloaded, then the user should be able to allocate memory. User should be able to specify data type.  
RAT: In order for a user to allocate memory.  
DEP: FR1  

### ID: FR3 
TITLE: Write data  
DESC: Given that the memory is allocated, then the user should be able to write data in the allocated space. The system should check the type compability and size of the allocation.  
RAT: In order for a user to write data.  
DEP: FR2  

### ID: FR3 
TITLE: Read data  
DESC: Given that the memory is allocated, then the user should be able to read data from the allocated space. The system should check the type compability and size of the allocation.  
RAT: In order for a user to read data.  
DEP: FR2  

### ID: FR3 
TITLE: Free memory  
DESC: Given that the memory is allocated, then the user should be able to free the memory. The system should check the fact of allocation.  
RAT: In order for a user to free memory.  
DEP: FR2  
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