/* TU Taylor    cs435 1263 mp */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <alloca.h>
#include <limits>
#include <algorithm>

using namespace std;

vector<string> g_codes; //stores the huffman code representation of each character 
string g_data = ""; //stores the data or characters 

//usage_1263 function to tell the user how to run the program  
void usage_1263(char *progname, string msg){
    cerr << "Error: " << msg << endl;
    cerr << "usage is: " << progname<< " [filename]" << endl;
}

//uses the codes to convert the message from huffman 
string decode_1263(string message){
    string encStr = "";
    string decMsg = "";
    for (int i=0; i<message.length(); i++){
        encStr += message[i];
        ptrdiff_t pos = find(g_codes.begin(), g_codes.end(), encStr) - g_codes.begin();
        if (pos < g_codes.size()){
            // cout << g_data[pos];
            decMsg += g_data[pos];
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
         
        string mess; //will hold just the message to be decoded
    
        //the following reads in the binary file 
        ifstream infile (argv[1], ios::binary); 
        if (infile.is_open()){

            //gets the length of the infile 
            infile.ignore( numeric_limits<streamsize>::max() );
            int msgLength = infile.gcount(); //message length 
            infile.clear();   //  Since ignore will have set eof.
            infile.seekg( 0, std::ios_base::beg );
            
            vector<char> message(msgLength); //will hold the entire file contents    
            infile.read(message.data(), msgLength);
            infile.close();

            //convert to string to make finding easier 
            string msgStr = string(message.begin(), message.end());
            string delimiter = "END"; //delimiter as set in encoding 

            //get the message to be decoded
            size_t msgEnd = msgStr.find(delimiter);
            mess = msgStr.substr(0, msgEnd);
            msgStr.erase(0, msgEnd + delimiter.length());

            string codeStr; //first element is data, remaining is code
            size_t pos = 0; //position of delimiter 

            //keeps finding delimiter to map the code to the character it stands for 
            while ((pos = msgStr.find(delimiter)) != string::npos){
                codeStr = msgStr.substr(0, pos); 
                g_codes.push_back(codeStr.substr(1,codeStr.length()));
                g_data.push_back(codeStr[0]);
                msgStr.erase(0, pos + delimiter.length());
            }
            
        } else {
            usage_1263(argv[0], "Cannot open file, '" + fileName + "'.");
            return 1; 
        }

        string decMsg = decode_1263(mess);

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