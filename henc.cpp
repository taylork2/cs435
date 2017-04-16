/* TU Taylor    cs435 1263 mp */

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<math.h>

using namespace std;

int MAXLENGTH = 100000; //random maxlength of huffman codes - to be set later 
string dataArray = ""; //contains non repeated characters in message 
vector<int> freq; //contains the frequecies of each letter in array, using dataArray as key 

//Usage function to tell the user how to run the program  
void usage(char *progname, string msg){
    cerr << "Error: " << msg << endl;
    cerr << "Usage is: " << progname<< " [filename]" << endl;
}

//minHeap implementation
struct minHeapNode{
    char data; //the character of the message
    int freq; //how many times the character occurs in message 
    struct minHeapNode *left, *right; //left and right trees used in Huffman Tree
};

struct minHeap{
    int size;
    vector<minHeapNode *> nodes; //pointers to each node of minHeap 
};

//displays the data and freq of each node in heap (for testing)
void printHeap(minHeap * A){
    vector<minHeapNode *> nodes = A->nodes;
    for (int i=0; i<A->size; i++){
        cout << nodes[i]->data << nodes[i]->freq << endl;
    }
}

//function to swap nodes in the minHeap 
void swapNodes(minHeap * A, int i, int j){
    minHeapNode * t = new minHeapNode();
    t = A->nodes[i];
    A->nodes[i] = A->nodes[j];
    A->nodes[j] = t;
}

void minHeapify(minHeap * A, int i){
    int curr = A->nodes[i]->freq;
    
    //if left and right nodes of minHeap  
    if (2*i+1 < A->size && 2*i+2 < A->size) {
        int left = A->nodes[2*i+1]->freq;
        int right = A->nodes[2*i+2]->freq;
        if (left <= right && left < curr){
            swapNodes(A, 2*i+1, i);
        } else if (right < left && right < curr){
            swapNodes(A, 2*i+2, i);
        } 
    } else if (2*i+1 < A->size){ //if only left node of minHeap
        if (A->nodes[2*i+1]->freq < curr){
            swapNodes(A, 2*i+1, i);
        }
    }
}

void buildMinHeap(minHeap * A){
    for (int i=floor(A->size/2)-1; i>=0; i--){
        minHeapify(A, i);
    }
}

//create minHeap from 2 vectors 
struct minHeap createMinHeap(string dataArray, vector<int> freq){
    minHeap heap = minHeap();
    heap.size = dataArray.length();
    for (int i = 0; i<dataArray.length(); i++){
        minHeapNode * node = new minHeapNode();
        node->data = dataArray[i];
        node->freq = freq[i];
        (heap.nodes).push_back(node);
    }
    buildMinHeap(&heap);
    return heap;
}

//min will be first node, extract from minHeap and buildMinHeap then return 
struct minHeapNode * extractMin(minHeap * A){
    int length = A->size;

    swapNodes(A, 0, length-1);

    minHeapNode * min = new minHeapNode();
    min = (A->nodes[length-1]);

    (A->nodes).erase((A->nodes).begin()+length-1);
    A->size--;
    buildMinHeap(A);

    return min;
}


void insertMinHeapNode(minHeap * A, minHeapNode * node){
    A->nodes.push_back(node);
    A->size++;
    buildMinHeap(A);
}

//compute the frequencies of each letter stored in dataArray and store in freq array
void computeFreq(char * message){
    for (char* it=message; *it; it++){
        if (dataArray.find(*it) == string::npos){
            dataArray+=*it;
            freq.push_back(1);
        } else {
            freq[dataArray.find(*it)]++;
        }
    }
}

//creates the huffman tree to find the codes from 
struct minHeapNode * buildHuffmanTree(minHeap * A){
    struct minHeapNode *a, *b, *c;
    while (A->size > 1){
        struct minHeapNode *a = extractMin(A);
        struct minHeapNode *b = extractMin(A);
        struct minHeapNode *c = new minHeapNode();
        c->freq = a->freq + b->freq;
        c->data = '#';
        c->left = a;
        c->right = b;
        insertMinHeapNode(A, c);
    }

    return extractMin(A); //this will be root node of huffman tree 
}


int printHuffmanCodes(minHeapNode * root, char code[], int index, vector<char*> codes){
    //if leaf of tree, store code in codes array 
    if (!(root->left) && !(root->right)){
        cout << root->data << code << endl;
        codes.at(dataArray.find(root->data)) = code;
    }

    if (root->left){
        code[index] = '0';
        printHuffmanCodes(root->left, code, index+1, codes);
    } 
    if (root->right){
        code[index] = '1';
        printHuffmanCodes(root->right, code, index+1, codes);
    }
    return 0;
}

void encodeMessage(){

}

int main(int argc, char *argv[]){  
    if (argc == 2){ //to ensure that there is a filename argument 

        int msgLength;
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
            usage(argv[0], "Cannot open file, '" + string(argv[1]) + "'.");
            return 1; 
        }

        computeFreq(message);
        minHeap heap = createMinHeap(dataArray, freq);
        printHeap(&heap);
        // cout << "====" << endl;
        minHeapNode * root = buildHuffmanTree(&heap);
        cout << "----------" << endl;
        char code[msgLength];
        vector<char*> codes(msgLength); //vector to store huffman codes using message array as key 
        printHuffmanCodes(root, code, 0, codes);
        // cout << root->left->data << endl;
        // cout << root->right->right->data << root->right->right->freq << endl;

    } else {
        usage(argv[0], "Incorrect number of arguments.");
        return 1;
    }

    return 0;
}