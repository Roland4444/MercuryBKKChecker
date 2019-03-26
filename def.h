
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

class Version{
public:
    int major;
    int minor;
    int build;
};

class Session{
public:
    Version version;
    char* id;
    void* payment;
    int last_error;
};

class ContentInfo{
public:
    uint8_t* content;
    uint64_t size;
};

typedef int (*create_session)(Session*  , char*);
typedef int (*BKKCheck)(Session*,  uint8_t* , uint64_t);

class Checker
{
public:
    void* handles[2];
    Session* sessions[2];
    BKKCheck v_check;
    BKKCheck i_check;

    Checker()
    {
        initSo();
        initSessions();
        loadscheckers();
    };

    void freeMem(ContentInfo* ci)
    {
        delete(ci);
    }

    int checkFile(char* filename)
    {
        ContentInfo* ci = loadContent(filename);;
        if (ci == NULL)
            return -1;

        if (strstr(filename, "wav")!=NULL){
            printf("CHECKING WAV FILE %s", filename);
            if (!v_check(sessions[soundindex], ci->content, ci->size))
            {
                freeMem(ci);
                printf("Check failed!\n");
                return -3;
            }
            else
            {
                printf("Checking passed\n");
                freeMem(ci);
                return 0;
            }
        }
        std::cout<<"CHECKING photo FILE "<< filename;
        if (!i_check(sessions[photoindex], ci->content, ci->size))
        {
            printf("Check failed!\n");
            freeMem(ci);
            return -3;
        }
        else
        {
            printf("Checking passed\n");
            freeMem(ci);
            return 0;
        }
    }

private:
    void initSo()
    {
        this->handles[soundindex] = dlopen(soundso, RTLD_LAZY);
        this->handles[photoindex] = dlopen(photoso, RTLD_LAZY);
        if (!handles[soundindex])
            std::cout<<("error loading sound so");
        else
            std::cout<<("Load success sound so");

        if (!handles[photoindex])
            printf("error loading photo so");
        else
            printf("Load success photo so");
    }

    void initSessions()
    {
        this->sessions[photoindex]=this->initSession(this->handles[photoindex], "i_create_session", photoconfig);
        this->sessions[soundindex]=this->initSession(this->handles[soundindex], "v_create_session", soundconfig);
        for (int i=0;i<2; i++){
            if (this->sessions[i]==NULL)
                printf("Error create session #%d\n", i);
        }
    }

    void loadscheckers(){
        this->i_check = (BKKCheck)(dlsym(this->handles[photoindex],"i_check_format"));
        this->v_check = (BKKCheck)(dlsym(this->handles[soundindex],"v_check"));
        if (!this->i_check)
            std::cout<<("error load i_check");
        if (!this->v_check)
            std::cout<<("error load v_check");

    }

    ContentInfo * loadContent(char * filename){
        ContentInfo * ci=new ContentInfo;
        read_file_content(filename, &ci ->content, &ci->size);
        return ci;
    }
    Session* initSession(void* handle, char* symbol, char* config)
    {
        Session* sess = new Session();
        create_session load = (create_session)(dlsym(handle, symbol));
        if (!load){
            printf("error loading %s\n\n\n", symbol );
            sess=NULL;
        }
        sess->id="my_session";
        if (!load(sess, config)){
            printf("error create session\n\n");
            sess=NULL;
            return sess;
        }
        printf("Session creates succesfully\n\n");
        return sess;
    }

    bool read_file_content(const char *file_path, uint8_t **content, size_t *content_size) {
        FILE *fd = fopen(file_path, "rb");
        if (fd == NULL) {
            fprintf(stderr, "file \"%s\" not found\n", file_path);
            return false;
        }
        fseek(fd, 0L, SEEK_END);
        (*content_size) = (size_t) ftell(fd);
        rewind(fd);
        (*content) = (uint8_t *) calloc(1, (*content_size));
        fread((*content), (*content_size), 1, fd);
        fclose(fd);
        return true;
    }

};





