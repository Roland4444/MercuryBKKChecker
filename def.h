
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
    int checkFile(const char* filename) {
        ContentInfo *ci = this->loadContent(filename);;
        if (ci == NULL)
            return -1;

        if (strstr(filename, "wav") != NULL) {
            printf("CHECKING WAV FILE %s", filename);
            if (!this->v_check(this->sessions[soundindex], ci->content, ci->size)) {
                freeMem(ci);
                printf("Check failed!\n");
                return -3;
            } else {
                printf("Checking passed\n");
                freeMem(ci);
                return 0;
            }
        }
    }

    void foreach_(char * filename)
    {
        FILE* fp;
        char buf[1024];
        if ((fp = fopen(filename, "r")) == NULL)
        {
            perror("fopen source-file");
            return;
        }

        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
            this->checkFile(buf);
        }
        fclose(fp);
        return ;
    }

    Checker(){
        this->handles[soundindex] = dlopen(soundso, RTLD_LAZY);
        this->handles[photoindex] = dlopen(photoso, RTLD_LAZY);

        if (!this->handles[soundindex])
            printf("error loading sound so");
        else
            printf("Load success sound so");

        if (!this->handles[photoindex])
            printf("error loading photo so");
        else
            printf("Load success photo so");

        this->initSessions();
        this->loadcheckers();

    }

private:

    void loadcheckers(){
        this->i_check = (BKKCheck)(dlsym(this->handles[photoindex],"i_check_format"));
        this->v_check = (BKKCheck)(dlsym(this->handles[soundindex],"v_check"));
        if (!this->i_check)
            printf("error load i_check");
        if (!this->v_check)
            printf("error load v_check");

    }

    void initSessions()
    {
        this->sessions[photoindex]=this->initSession(this->handles[photoindex], "i_create_session", photoconfig);
        this->sessions[soundindex]=this->initSession(this->handles[soundindex], "v_create_session", soundconfig);
        int i;
        for (i=0;i<2; i++){
            if (this->sessions[i]==NULL)
                printf("Error create session #%d\n", i);
        }
    }

    bool read_file_content(const char *file_path, uint8_t **content, size_t *content_size)
    {
        FILE* fd = fopen(file_path, "rb");
        if (fd == NULL)
        {
            fprintf(stderr, "file \"%s\" not found\n", file_path);
            return false;
        }
        fseek(fd, 0L, SEEK_END);
        (*content_size) = (size_t) ftell(fd);
        rewind(fd);
        (*content) = (uint8_t*) calloc(1, (*content_size));
        fread((*content), (*content_size), 1, fd);
        fclose(fd);
        return true;
    };

    void freeMem(ContentInfo* ci)
    {
        free(ci->content);
        free(ci);
    }

    ContentInfo* loadContent(char * filename){
        ContentInfo* ci=(ContentInfo*)malloc(sizeof(ContentInfo));
        if (read_file_content(filename, &ci ->content, &ci->size))
            return ci;
        freeMem(ci);
        return NULL;
    };

    Session* initSession(void* handle, char* symbol, char* config)
    {
        Session* sess = (Session*)malloc(sizeof(Session));
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
    };
};


