/* TU Taylor    cs435 1263 mp */

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<math.h>

using namespace std;

//Usage function to tell the user how to run the program  
void usage(char *progname, string msg){
    cerr << "Error: " << msg << endl;
    cerr << "Usage is: " << progname<< " [filename]" << endl;
}

int main(int argc, char *argv[]){ 
    if (argc == 2){ //to ensure that there is a filename argument 
        
        //check that file extension is .huf
        string fileName = string(argv[1]);
        if (fileName.substr(fileName.find_last_of(".") +1) != "huf"){
            usage(argv[0], "Must be .huf file.");
            return 1;
        }
        
        int msgLength; //length of message 
        char * message; //will hold the message contents 
    
        //the following reads in the binary file 
        ifstream infile (argv[1], ios::binary); 
        if (infile.is_open()){
            infile.seekg(0, ios::end);
            msgLength = infile.tellg();
            infile.seekg(0, ios::beg);

            message = new char[msgLength];
            infile.read(message, msgLength);
            infile.close();
        } else {
            usage(argv[0], "Cannot open file, '" + fileName + "'.");
            return 1; 
        }
    } else {
        usage(argv[0], "Incorrect number of arguments.");
        return 1;
    }

    return 1;
}