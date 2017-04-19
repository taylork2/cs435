/* TU Taylor    cs435 1263 mp */

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<math.h>

using namespace std;

string dataArray = ""; //contains non repeated characters in message 
vector<int> freq; //contains the frequecies of each letter in array, using dataArray as key 
vector<string> codes; //will contain the huffman codes for each letter 

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

//create minHeap from 2 vectors 
struct minHeap_1263 createMinHeap(string dataArray, vector<int> freq){
    minHeap_1263 heap = minHeap_1263();
    heap.size = dataArray.length();
    for (int i = 0; i<dataArray.length(); i++){
        minHeapNode_1263 * node = new minHeapNode_1263();
        node->data = dataArray[i];
        node->freq = freq[i];
        (heap.nodes).push_back(node);
    }
    buildMinHeap_1263(&heap);
    return heap;
}

//min will be first node, extract from minHeap and buildMinHeap then return 
struct minHeapNode_1263 * extractMin(minHeap_1263 * A){
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
void computeFreq_1263(char * message, int msgLength){
    for (char* it=message; *it; it++){
        if (dataArray.find(*it) == string::npos){
            dataArray+=*it;
            freq.push_back(1);
            codes.push_back(""); //necessary for indexing codes using dataArray later
        } else {
            freq[dataArray.find(*it)]++;
        }
    }
}

//creates the huffman tree to find the codes from 
struct minHeapNode_1263 * buildHuffmanTree_1263(minHeap_1263 * A){
    struct minHeapNode_1263 *a, *b, *c;
    while (A->size > 1){
        struct minHeapNode_1263 *a = extractMin(A);
        struct minHeapNode_1263 *b = extractMin(A);
        struct minHeapNode_1263 *c = new minHeapNode_1263();
        c->freq = a->freq + b->freq;
        c->data = '#'; //arbitrary character 
        c->left = a;
        c->right = b;
        insertminHeapNode_1263(A, c);
    }

    return extractMin(A); //this will be root node of huffman tree 
}


void printHuffmanCodes_1263(minHeapNode_1263 * root, string code, int index){
    //if leaf of tree, store code in codes array 
    if (!(root->left) && !(root->right)){
        codes.at(dataArray.find(root->data)) = code;
    }

    if (root->left){ 
        if (code.length() < index+1){
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
void encodeMessage_1263(const char * fileName, char * message){
    ofstream outfile (fileName, ios::binary);
    for (char* it=message; *it; it++){
        string code = codes[dataArray.find(*it)];
        outfile << code;
        // cout << code << endl;
    }

    outfile << '\0'; //to mark the end of message 

    //print the character then huffman code as a key for decoding 
    for (int i=0; i<dataArray.length(); i++){
        outfile << dataArray[i] << codes[i] << '\0';
        // cout << dataArray[i] << codes[i] << endl;
    }

    outfile.close();
}

int main(int argc, char *argv[]){  
    if (argc == 2){ //to ensure that there is a filename argument 

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
            usage_1263(argv[0], "Cannot open file, '" + string(argv[1]) + "'.");
            return 1; 
        }

        computeFreq_1263(message, msgLength);
        minHeap_1263 heap = createMinHeap(dataArray, freq);
        // printHeap_1263(&heap);
        minHeapNode_1263 * root = buildHuffmanTree_1263(&heap);
        string code; 

        printHuffmanCodes_1263(root, code, 0);

        string ofname = string(argv[1]) + ".huf"; 
        encodeMessage_1263(ofname.c_str(), message);

    } else {
        usage_1263(argv[0], "Incorrect number of arguments.");
        return 1;
    }

    return 0;
}
