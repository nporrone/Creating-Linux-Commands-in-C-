//
// Created by nporrone on 2019-10-03.
//

#include "filemanager.h"

/*
 * This creates an instance of the class and initiatives it.
 * It will take the name of a file as a parameter and use the
 * stat() function to initialize everything else.
 */

filemanager::filemanager(std::string filename) {

    struct stat buf; // Create a buffer in memory to store the return of stat below

    if(stat(filename.c_str(),&buf) != -1 ){
        this->filename = filename;
        filetype = buf.st_mode;
        gettypestr(filetype); //initialize typestr (string containing filetype)
		filesize = buf.st_size; 
        owner = buf.st_uid;
        struct passwd* ownerpasswd = getpwuid(buf.st_uid); // extract the owner name
        ownerstr = ownerpasswd->pw_name; // store the name as a string
        group = buf.st_gid;
        struct group* grouppasswd = getgrgid(buf.st_gid); // extract group name
        groupstr = grouppasswd->gr_name;
        premissons = buf.st_mode;
        getpremstr(premissons); //initialize premstr (string containing premissons)
        accesstime = buf.st_atime;
        modtime = buf.st_mtime;
        stattime = buf.st_ctime;
        blocksize = buf.st_blksize;
        children ={};
        errorcode = errno;
    } else {
        errorcode = errno;
        this->filename = "File does not exist";
        typestr = "File does not exist";
        ownerstr = "No Owner, file does not exist";
        groupstr = "No Group, file does not exist";
        premstr = "File does not exist";
    }

    buf = {0}; // clear the buffer

}

/*
 * This destroys and frees up any resources attached to the object
 * the destructor was called on.
 */

filemanager::~filemanager() = default;

/*
 * This function will take a file stream as a parameter and dump the contents of
 * the named file to that file stream.  This can be used to copy the file, display
 * its contents to the terminal, and so on.
 */

int filemanager::dump(std::ostream &filestream) {
	
	if(typestr != "Regular File") { // check if this is a regular file.
		return ENOTSUP;
	}
	
    int tempsize = filesize; // create a temp size for a counter
    std::fstream thisstream; // create an input stream to dump the file into
    thisstream.open(filename); // open the stream and allow the file to be written too
	
	char* dbuf = new char[blocksize]; // create a buffer array to store each block size

    if(thisstream.is_open()) {

        while(tempsize > 0){
			
			if(tempsize < blocksize) {
				
				thisstream.read(dbuf,tempsize);
				filestream.write(dbuf,tempsize);
				thisstream.close();
				return errorcode;
				
			} else {
				
				thisstream.read(dbuf,blocksize);
				filestream.write(dbuf,blocksize);
				tempsize -= blocksize;		
			}
			
			dbuf = {nullptr}; // clear the buffer
			
		} 
		
    } else {
		errorcode = errno;
		return errorcode;
	}
}

// This changes the name of the file from its existing name to the new name provided as a parameter to this function.

int filemanager::rename(std::string newname){
	::rename(filename.c_str(),newname.c_str());
    filename = newname;
	errorcode = errno;
	return errorcode;
}

// This removes the file from the file system.  

int filemanager::remove() {
	::unlink(filename.c_str());
	errorcode = errno;
	return errorcode;
}

// This function takes another one of the file objects as a parameter and will compare the contents of the file object this function was invoked upon to this other file object.

int filemanager::compare(std::string cmpfile){
	
	filemanager *checkfile = new filemanager(cmpfile); // create an instance of the file to check
	std::fstream currentstream; // create a stream for the current file
	std::fstream checkstream; // create a stream for the check file
	char* cbuf1 = new char[blocksize]; // create a buffer array to store each block size of current file
	char* cbuf2 = new char[blocksize]; // create a buffer array to store each block size of check file
	
	if (filesize != checkfile->filesize || filetype != checkfile->filetype){ // edge case (different file size means different file)
		return -1;
	
	} else {

		currentstream.open(filename);
		checkstream.open(checkfile->filename);
		int tempsize = filesize; // create a temp size and set to filesize since both files are of same length
		
		if(currentstream.is_open() && currentstream.is_open()){
			
			while(tempsize > 0){
			
				if(tempsize < blocksize) {
				
					currentstream.read(cbuf1,tempsize);
					checkstream.read(cbuf2,tempsize);
					
					if(strcmp(cbuf1,cbuf2) != 0) { // check if blocks dont match
						return -1;
						
					} else {
						currentstream.close();
						checkstream.close();
						return 0; // Files match
					}
				
				} else {
				
					currentstream.read(cbuf1,blocksize);
					checkstream.read(cbuf2,blocksize);
					
					if(strcmp(cbuf1,cbuf2) != 0 ){ 
						return -1;
					}
					
					tempsize -= blocksize;	// decrement block size
				}

                cbuf1 = {nullptr};  // clear both buffers
                cbuf2 = {nullptr};
		
			}
		
		} else {
			currentstream.close();
			checkstream.close();
			errorcode = errno;
			return errorcode;
		}
	
	}		
	
}

// This function operates on directories only and is used to fill in the children of the file object this function was invoked upon. 

int filemanager::expand(){
	
	DIR *dir; // Create a directory to use later
	struct dirent* entry; // Use as a file in each directory
	
	if(typestr != "Directory") { // check if this is a Directory.
		return ENOTSUP;
	}
	
	if((dir = opendir(filename.c_str())) != NULL) {
		
		while ((entry = readdir(dir)) != NULL) {
			
			auto *child = new filemanager(entry->d_name);
			children.push_back(*child);
			
		}
		closedir(dir);
		errorcode = errno;
		return errorcode;
		
	} else {
		
		errorcode = errno;
		return errorcode;
		
	}
	
}

/*
 * The following functions are getter methods for all attributes for a filemanager object
 *
 * Be sure to check out the helper functions used in the constructer at the bottom of the file.
 */

const std::string &filemanager::getFilename() const {
    return filename;
}

mode_t filemanager::getFiletype() const {
    return filetype;
}

const std::string &filemanager::getTypestr() const {
    return typestr;
}

off_t filemanager::getFilesize() const {
    return filesize;
}

uid_t filemanager::getOwner() const {
    return owner;
}

const std::string &filemanager::getOwnerstr() const {
    return ownerstr;
}

gid_t filemanager::getGroup() const {
    return group;
}

const std::string &filemanager::getGroupstr() const {
    return groupstr;
}

mode_t filemanager::getPremissons() const {
    return premissons;
}

const std::string &filemanager::getPremstr() const {
    return premstr;
}

time_t filemanager::getAccesstime() const {
    return accesstime;
}

time_t filemanager::getModtime() const {
    return modtime;
}

time_t filemanager::getStattime() const {
    return stattime;
}

blksize_t filemanager::getBlocksize() const {
    return blocksize;
}

const std::vector<filemanager> &filemanager::getChildren() const {
    return children;
}

int filemanager::getErrorcode() const {
    return errorcode;
}

char* filemanager::getErrorcodestr() const {
    return strerror(errorcode);
}


// Helper Method used to get filetype as a string

void filemanager::gettypestr(mode_t filetype) {
    if(S_ISREG(filetype)){ typestr.append("Regular File");}
    if(S_ISDIR(filetype)){ typestr.append("Directory");}
    if(S_ISCHR(filetype)){ typestr.append("Character Device");}
    if(S_ISBLK(filetype)){ typestr.append("Block Device");}
    if(S_ISFIFO(filetype)){ typestr.append("FIFO(named pipe?)");}
    if(S_ISLNK(filetype)){ typestr.append("Symbolic Link");}
    if(S_ISSOCK(filetype)){ typestr.append("Socket");}
}

// Helper method used to get premissons as a string

void filemanager::getpremstr(mode_t premissons) {
    (premissons & S_ISDIR(premissons)) ? premstr.append("d") : premstr.append("-");
    (premissons & S_IRUSR) ? premstr.append("r") : premstr.append("-");
    (premissons & S_IWUSR) ? premstr.append("w") : premstr.append("-");
    (premissons & S_IXUSR) ? premstr.append("x") : premstr.append("-");
    (premissons & S_IRGRP) ? premstr.append("r") : premstr.append("-");
    (premissons & S_IWGRP) ? premstr.append("w") : premstr.append("-");
    (premissons & S_IXGRP) ? premstr.append("x") : premstr.append("-");
    (premissons & S_IROTH) ? premstr.append("r") : premstr.append("-");
    (premissons & S_IWOTH) ? premstr.append("w") : premstr.append("-");
    (premissons & S_IXOTH) ? premstr.append("x") : premstr.append("-");
}



