//
// Created by roland on 2/28/19.
//

#ifndef CPPBLASTED_DEF_H
#define CPPBLASTED_DEF_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <iostream>

typedef struct
{
    int major;
    int minor;
    int build;
} Version;

typedef struct
{
    Version version;
    char* id;
    void* payment;
    int last_error;
} Session;

typedef struct
{
    uint8_t* content;
    uint64_t size;
}   ContentInfo;

typedef int (*create_session)(Session*  , char*);
typedef int (*BKKCheck)(Session*,  uint8_t* , uint64_t);

#endif //CPPBLASTED_DEF_H
