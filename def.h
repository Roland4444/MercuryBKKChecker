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
#define soundso "./libsqc.so"
#define photoso "./libBKKAdaptor.so"
#define soundconfig "./cv_configuration.json"
#define photoconfig "./ci_configuration.json"
#define soundindex 0
#define photoindex 1

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


class Checker
{
public:
    void* handles[2];
    Session* sessions[2];
    BKKCheck v_check;
    BKKCheck i_check;
    Session * session;
    int ResultCreating;
    void * handle;
    char * config = "./cv_configuration.json";
    int checkFile(char * filename);

private:

    Session* initSession(void* handle, char* symbol, char* config);
    void initSessions_();
    bool read_file_content(const char *file_path, uint8_t **content, size_t *content_size) ;
    ContentInfo* loadContent(char * filename);

};

#endif //CPPBLASTED_DEF_H
