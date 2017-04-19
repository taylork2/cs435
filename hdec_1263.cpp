/* TU Taylor    cs435 1263 mp */

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<cstring>
#include<alloca.h>

using namespace std;

map<string, char> codes; //stores the code then the character the code represents 

//usage_1263 function to tell the user how to run the program  
void usage_1263(char *progname, string msg){
    cerr << "Error: " << msg << endl;
    cerr << "usage is: " << progname<< " [filename]" << endl;
}

//uses the codes map to convert the message from huffman 
string decode_1263(char * message){
    string encStr = "";
    string decMsg = "";
    for (char *it=message; *it; it++){
        encStr += *it;
        if (codes.find(encStr) != codes.end()){
            // cout << codes[encStr];
            decMsg += codes[encStr];
            encStr = "";
        }
    }
    // cout << decMsg;
    return decMsg;
}

int main(int argc, char *argv[]){ 
    if (argc == 2){ //to ensure that there is a filename argument 
        
        //check that file extension is .huf
        string fileName = string(argv[1]);
        if (fileName.substr(fileName.find_last_of(".") +1) != "huf"){
            usage_1263(argv[0], "Must be .huf file.");
            return 1;
        }
        
        int msgLength; //length of message 
        char * message; //will hold the encoded message contents 
    
        //the following reads in the binary file 
        ifstream infile (argv[1], ios::binary); 
        if (infile.is_open()){
            infile.seekg(0, ios::end);
            msgLength = infile.tellg();
            infile.seekg(0, ios::beg);

            message = new char[msgLength];
            infile.getline(message,msgLength, '\0');
            char * code = new char[msgLength];

            //maps the code to the character it stands for 
            while (infile.getline(code, msgLength, '\0')){
                string codeStr = string(code);
                codes[codeStr.substr(1,codeStr.length())] = code[0];
            }

            infile.close();
            
        } else {
            usage_1263(argv[0], "Cannot open file, '" + fileName + "'.");
            return 1; 
        }

        string decMsg = decode_1263(message);

        //overwrites original file with decoded message 
        ofstream outfile (argv[1], ios::binary|ios::out|ios::trunc);
        outfile << decMsg;

        //rename the file without .huf 
        fileName = fileName.substr(0, fileName.find_last_of("."));
        char * newFileName = (char *)alloca(fileName.size() + 1);
        memcpy(newFileName, fileName.c_str(), fileName.size()+1);
        rename(argv[1], newFileName);

        outfile.close();

    } else {
        usage_1263(argv[0], "Incorrect number of arguments.");
        return 1;
    }

    return 1;
}