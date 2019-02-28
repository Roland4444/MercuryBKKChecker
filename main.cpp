#include "def.h"

class Checker
{
public:
    Session * session;
    int ResultCreating;
    void * handle;
    char * config = "./cv_configuration.json";
    int checkFile(char * filename){
        std::cout<<"\n\nchecking =>"<<filename<<std::endl;
        ContentInfo * ci;
        if (loadContent(filename) == NULL)
            return -1;
        ci = loadContent(filename);
        BKKCheck v_check = (BKKCheck)(dlsym(handle,"v_check"));
        if (!v_check)
            return -2;

        if (!v_check(this->session, ci ->content, ci ->size)){
            std::cout<<"Check failed!";
            return -3;
        }
        else return 0;

    }

    Checker()
    {
        this -> session = (Session*)malloc(sizeof(Session));
        std::cout<<"Loads "<<sizeof(Session)<<"  mamory\n";
        this -> handle = dlopen("./libcv.so", RTLD_LAZY);
        ResultCreating=0;
        if (handle == NULL)
        {
            std::cout<<"\n\nerror loading\n\n\n";
            ResultCreating = -1;
        }
        initSession();
    };
private:
    ContentInfo * loadContent(char * filename){
        ContentInfo * ci=new ContentInfo;
        read_file_content(filename, &ci ->content, &ci->size);
        return ci;
    }
    void initSession(){
        Session * session = (Session*)malloc(sizeof(Session));
        create_session load = (create_session)(dlsym(this->handle, "v_create_session"));
        if (!load){
            std::cout<<"error loading v_create_session";
            this->session=NULL;
        }
        session->id="my_session";
        if (!load(session, config)){
            std::cout<<"error create session";
            this->session=NULL;
            return;
        }
        std::cout<<"Session creates succesfully";
        this->session=session;
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

int main(int argc, char *argv[]) {
    Checker * checker = new Checker();
    std::cout<<checker->checkFile("./tested.wav");
    for (int i=1; i<argc; i++)
        std::cout<< checker->checkFile(argv[i]);
}