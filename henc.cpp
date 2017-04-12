/* TU Taylor    cs435 1263 mp */

#include<fstream>
#include<iostream>
#include<string>

using namespace std;

//Usage function to tell the user how to run the program  
void usage(char *progname, string msg){
    cerr << "Error: " << msg << endl;
    cerr << "Usage is: " << progname<< " [filename]" << endl;
}

struct minHeap{
    char letter;
    int freq;
    minHeap *left, *right;
};

void comFreq(char * buffer){
    for (char* it=buffer; *it; it++){
        cout << *it;
    }
}

int main(int argc, char *argv[]){  
    if (argc == 2){ //to ensure that there is a filename argument 

        int length;
        char * buffer; //will hold the message contents 
    
        //the following reads in the binary file 
        ifstream infile (argv[1], ios::binary); 
        if (infile.is_open()){
            infile.seekg(0, ios::end);
            length = infile.tellg();
            infile.seekg(0, ios::beg);

            buffer = new char[length];
            infile.read(buffer, length);
            infile.close();
        } else {
            usage(argv[0], "Cannot open file, '" + string(argv[1]) + "'.");
            return 1; 
        }

        comFreq(buffer);

    } else {
        usage(argv[0], "Incorrect number of arguments.");
        return 1;
    }

    return 0;
}