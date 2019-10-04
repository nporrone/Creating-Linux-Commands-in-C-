//
// Created by nporrone on 2019-10-03.
//

#include "filemanager.h"

using namespace std;

int main(int argc, char* argv[]) {

    if(argc == 1) { // regular ls

        char* pwd = new char[256];
        getcwd(pwd,256); // get current directory

        auto current = new filemanager(pwd); // create an object of the current directory
        current->expand();
        vector<filemanager> children = current->getChildren();

        for(filemanager child : children) {
            cout << child.getFilename() << endl;
        }

        return 0;

    } else if(argc == 2) { // ls with -l or directory

        if (strcmp(argv[1], "-l") == 0) { // -l

            char *pwd = new char[256];
            getcwd(pwd, 256); // get current directory
            auto current = new filemanager(pwd); // create an object of the current directory
            current->expand();
            vector<filemanager> children = current->getChildren();

            for (filemanager child : children) {
                cout << child.getPremstr() << child.getOwnerstr() << child.getGroupstr() << child.getFilesize() <<
                     child.getAccesstime() << child.getFilename() << endl;
            }

            return 0;

        } else {  // directory

            auto current = new filemanager(argv[1]);
            current->expand();
            vector<filemanager> children = current->getChildren();
            for(filemanager child : children) {
                cout << child.getFilename() << endl;
            }

        }

    } else if(argc == 3) { // ls -l w/ a directory included

        auto current = new filemanager(argv[2]);
        current->expand();
        vector<filemanager> children = current->getChildren();
        for(filemanager child : children) {
            cout << child.getPremstr() << child.getOwnerstr() << child.getGroupstr() << child.getFilesize() <<
                 child.getAccesstime() << child.getFilename() << endl;
        }

        return 0;

    } else {

        cerr << "Invalid Arguments" << endl;

    }

}
