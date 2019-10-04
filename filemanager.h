//
// Created by nporrone on 2019-10-03.
//

#include <cstring>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <cstdlib>

#ifndef INDIVIDUAL_ASSIGNMENT_FILEMANAGER_H
#define INDIVIDUAL_ASSIGNMENT_FILEMANAGER_H


class filemanager {

public:
    filemanager(std::string filename); // Constructer
    ~filemanager(); // Deconstructer
    int dump(std::ostream &filestream); // This function will take a file stream as a parameter and dump the contents of the named file to that file stream.
    int rename(std::string newname); // This changes the name of the file from its existing name to the new name provided as a parameter to this function.
    int remove(); // This removes the file from the file system.
    int compare(std::string cmpfile); // his function takes another one of the file objects as a parameter and will compare the contents of the file object this function was invoked upon to this other file object.
    int expand(); //This function operates on directories only and is used to fill in the children of the file object this function was invoked upon.

    /*
     *  The following functions are getters for each attribute of type filemanager
     *
     */

    const std::string &getFilename() const;
    mode_t getFiletype() const;
    const std::string &getTypestr() const;
    off_t getFilesize() const;
    uid_t getOwner() const;
    const std::string &getOwnerstr() const;
    gid_t getGroup() const;
    const std::string &getGroupstr() const;
    mode_t getPremissons() const;
    const std::string &getPremstr() const;
    time_t getAccesstime() const;
    time_t getModtime() const;
    time_t getStattime() const;
    blksize_t getBlocksize() const;
    const std::vector<filemanager> &getChildren() const;
    int getErrorcode() const;
    char *getErrorcodestr() const;

private:
    std::string filename; // The name of the file
    mode_t filetype; // The type of the file
    std::string typestr; // Type of file in form of string
    off_t filesize; // file size
    uid_t owner; // Owner of the files ID
    std::string ownerstr; // Owner of the file as a string
    gid_t group; // The group of the file
    std::string groupstr; // group as a string
    mode_t premissons; // Premissons of the file
    std::string premstr; // The Premissons as a string
    time_t accesstime; // Last time of access
    time_t modtime; // Last time of modification
    time_t stattime; // last time of a status change
    blksize_t blocksize; // The block size for the file
    std::vector<filemanager> children; // Vector storing the parents children of type filemanager
    int errorcode; // The value of errno will be stored here
    void gettypestr(mode_t filetype); // returns filetype as a string
    void getpremstr(mode_t premissons); // returns premissons as a string

};


#endif //INDIVIDUAL_ASSIGNMENT_FILEMANAGER_H