/* TU Taylor    cs435 1263 mp */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <limits>

using namespace std;

string g_dataArray=""; //every unique character in file 
vector<int> g_freq; //frequencies of each unique char 
vector<string> g_codes; //the huffman codes 

//usage function to tell the user how to run the program  
void usage_1263(char *progname, string msg){
    cerr << "Error: " << msg << endl;
    cerr << "usage is: " << progname<< " [filename]" << endl;
}

//minHeap implementation
struct minHeapNode_1263{
    char data; //the character of the message
    int freq; //how many times the character occurs in message 
    struct minHeapNode_1263 *left, *right; //left and right trees used in Huffman Tree
};

struct minHeap_1263{
    int size; //number of nodes in nodes 
    vector<minHeapNode_1263 *> nodes; //pointers to each node of minHeap 
};

//displays the data and freq of each node in heap (for testing)
void printHeap_1263(minHeap_1263 * A){
    vector<minHeapNode_1263 *> nodes = A->nodes;
    for (int i=0; i<A->size; i++){
        cout << nodes[i]->data << nodes[i]->freq << endl;
    }
}

//function to swap nodes in the minHeap 
void swapNodes_1263(minHeap_1263 * A, int i, int j){
    minHeapNode_1263 * t = new minHeapNode_1263();
    t = A->nodes[i];
    A->nodes[i] = A->nodes[j];
    A->nodes[j] = t;
}

void minHeapify_1263(minHeap_1263 * A, int i){
    int curr = A->nodes[i]->freq;
    
    //if left and right nodes of minHeap  
    if (2*i+1 < A->size && 2*i+2 < A->size) {
        int left = A->nodes[2*i+1]->freq;
        int right = A->nodes[2*i+2]->freq;
        if (left <= right && left < curr){
            swapNodes_1263(A, 2*i+1, i);
        } else if (right < left && right < curr){
            swapNodes_1263(A, 2*i+2, i);
        } 
    } else if (2*i+1 < A->size){ //if only left node of minHeap
        if (A->nodes[2*i+1]->freq < curr){
            swapNodes_1263(A, 2*i+1, i);
        }
    }
}

void buildMinHeap_1263(minHeap_1263 * A){
    for (int i=floor(A->size/2)-1; i>=0; i--){
        minHeapify_1263(A, i);
    }
}

//create minHeap from map
struct minHeap_1263 createMinHeap_1263(){
    minHeap_1263 heap = minHeap_1263();
    heap.size = g_dataArray.length();
     for (int i = 0; i<g_dataArray.length(); i++){
        minHeapNode_1263 * node = new minHeapNode_1263();
        node->data = g_dataArray[i];
        node->freq = g_freq[i];
        (heap.nodes).push_back(node);
    }
    buildMinHeap_1263(&heap);
    return heap;
}

//min will be first node, extract from minHeap and buildMinHeap then return 
struct minHeapNode_1263 * extractMin_1263(minHeap_1263 * A){
    int length = A->size;

    swapNodes_1263(A, 0, length-1);

    minHeapNode_1263 * min = new minHeapNode_1263();
    min = (A->nodes[length-1]);

    (A->nodes).erase((A->nodes).begin()+length-1);
    A->size--;
    buildMinHeap_1263(A);

    return min;
}


void insertminHeapNode_1263(minHeap_1263 * A, minHeapNode_1263 * node){
    A->nodes.push_back(node);
    A->size++;
    buildMinHeap_1263(A);
}

//compute the frequencies of each letter stored in dataArray and store in freq array
void computeFreq_1263(vector<char> message){
    for (int i=0; i<message.size(); i++){

        //if not in map, add new entry 
        if (g_dataArray.find(message[i]) == string::npos){
            g_dataArray.push_back(message[i]);
            g_freq.push_back(1);
            g_codes.push_back(""); //necessary for indexing codes using dataArray later
        } else { //otherwise increment freq count 
            g_freq[g_dataArray.find(message[i])]++;
        }
    }
}

//creates the huffman tree to find the codes from 
struct minHeapNode_1263 * buildHuffmanTree_1263(minHeap_1263 * A){
    struct minHeapNode_1263 *a, *b, *c;
    while (A->size > 1){
        struct minHeapNode_1263 *a = extractMin_1263(A);
        struct minHeapNode_1263 *b = extractMin_1263(A);
        struct minHeapNode_1263 *c = new minHeapNode_1263();
        c->freq = a->freq + b->freq;
        c->data = '#'; //arbitrary character 
        c->left = a;
        c->right = b;
        insertminHeapNode_1263(A, c);
    }

    return extractMin_1263(A); //this will be root node of huffman tree 
}


void printHuffmanCodes_1263(minHeapNode_1263 * root, string code, int index){
    //if leaf of tree, store code in codes array 
    if (!(root->left) && !(root->right)){
        g_codes.at(g_dataArray.find(root->data)) = code;
    }

    if (root->left){ 
        if (code.length() < index+1){ //to avoid out_of_range error
            code += "0";
        } else {
            code[index] = '0';
        }
        printHuffmanCodes_1263(root->left, code, index+1);
    } 

    if (root->right){
        if (code.length() < index+1){
            code+="1";
        } else {
            code[index] = '1';
        }
        printHuffmanCodes_1263(root->right, code, index+1);
    }

}

//create new file and write huffman encoded message to it 
void encodeMessage_1263(const char * fileName, vector<char> message){
    ofstream outfile (fileName, ios::binary);
    
    for (int i=0; i<message.size(); i++){
        string code = g_codes[g_dataArray.find(message[i])];
        outfile << code;
        // cout << code << endl;
    }

    string delimiter = "END"; //arbitrary phrase to mark end of message, used for decoding
    outfile << delimiter;

    //output to file the character then huffman code as a key for decoding 
    for (int i=0; i<g_dataArray.length(); i++){
        outfile << g_dataArray[i] << g_codes[i] << delimiter;
        // cout << g_dataArray[i] << g_codes[i] << delimiter;
    }

    outfile.close();
}

int main(int argc, char *argv[]){  
    if (argc == 2){ //to ensure that there is a filename argument 

        //read in binary files
        ifstream infile (argv[1], ios::in|ios::binary); 
        if (infile.is_open()){

            //gets the length of the infile, potentially larger than actual file
            infile.ignore( numeric_limits<streamsize>::max() );
            streamsize msgLength = infile.gcount(); //message length 
            infile.clear();   //  Since ignore will have set eof.
            infile.seekg(0, std::ios_base::beg);
            
            vector<char> message(msgLength); //will hold the message contents    
            infile.read(message.data(), msgLength);
            infile.close();
            
            computeFreq_1263(message);

            minHeap_1263 heap = createMinHeap_1263();
            // printHeap_1263(&heap);
            minHeapNode_1263 * root = buildHuffmanTree_1263(&heap);

            string code; 
            printHuffmanCodes_1263(root, code, 0);

            //make new name of out file 
            string ofname = string(argv[1]) + ".huf"; 
            encodeMessage_1263(ofname.c_str(), message);

        } else {
            usage_1263(argv[0], "Cannot open file, '" + string(argv[1]) + "'.");
            return 1; 
        }

    } else {
        usage_1263(argv[0], "Incorrect number of arguments.");
        return 1;
    }

    return 0;
}
