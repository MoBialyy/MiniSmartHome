#ifndef LAB1_H
#define LAB1_H

// Character
typedef char char8;                     // 1 byte
typedef unsigned char uchar8;           // 1 byte
typedef char* char8_ptr;                // pointer to char8
typedef unsigned char* uchar8_ptr;      // pointer to uchar8

// Short integer
typedef short sint16;                   // 2 bytes
typedef unsigned short usint16;         // 2 bytes
typedef short* sint16_ptr;              // pointer to sint16
typedef unsigned short* usint16_ptr;    // pointer to usint16

// Integer
typedef int int32;                      // 4 bytes
typedef unsigned int uint32;            // 4 bytes
typedef int* int32_ptr;                 // pointer to int32
typedef unsigned int* uint32_ptr;       // pointer to uint32

// Long integer
typedef long int long32;                // 4 bytes
typedef unsigned long ulong32;          // 4 bytes
typedef long int* long32_ptr;           // pointer to long32
typedef unsigned long* ulong32_ptr;     // pointer to ulong32

// Float
typedef float float32;                  // 4 bytes
typedef float* float32_ptr;             // pointer to float32

// Double
typedef double double64;                // 8 bytes
typedef double* double64_ptr;           // pointer to double64

/*
   typedef is better for syntax and debugging
   while
   #define is better for constants
*/

#endif
