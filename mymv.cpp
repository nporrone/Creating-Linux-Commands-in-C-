//
// Created by nporrone on 2019-10-03.
//

#include "filemanager.h"

using namespace std;

int main(int argc, char* argv[]) {

    if(argc < 3) {
        cerr << "Not enough arguments" << endl;
    }

    if(argc > 3) {
        cerr << "Too many arguments" << endl;
    }

    auto current = new filemanager(argv[1]);

    if ((current->rename(argv[2])) == EXDEV){

        ofstream fixer;
        fixer.open(argv[2]);
        current->dump(fixer);
        current->remove();
        fixer.close();

    }

    return 0;

}