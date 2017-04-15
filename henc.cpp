/* TU Taylor    cs435 1263 mp */

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<math.h>

using namespace std;

string dataArray = ""; //contains non repeated characters in message 
vector<int> freq; //contains the frequecies of each letter in array 

//Usage function to tell the user how to run the program  
void usage(char *progname, string msg){
    cerr << "Error: " << msg << endl;
    cerr << "Usage is: " << progname<< " [filename]" << endl;
}

//minHeap implementation
struct minHeapNode{
    char data;
    int freq;
    struct minHeapNode *left, *right;
};

struct minHeap{
    int size;
    vector<minHeapNode> nodes; 
};

void printHeap(minHeap A){
    vector<minHeapNode> nodes = A.nodes;
    for (int i=0; i<A.size; i++){
        cout << nodes[i].data << nodes[i].freq << endl;
    }
}

//function to swap nodes in the minHeap 
void swapNodes(minHeap * A, int i, int j){
    minHeapNode t = minHeapNode();
    t = A->nodes[i];
    A->nodes[i] = A->nodes[j];
    A->nodes[j] = t;
}

void minHeapify(minHeap * A, int i){
    int curr = A->nodes[i].freq;
    if (2*i+1 < A->size && 2*i+2 < A->size) {
        int left = A->nodes[2*i+1].freq;
        int right = A->nodes[2*i+2].freq;
        if (left <= right && left < curr){
            swapNodes(A, 2*i+1, i);
        } else if (right < left && right < curr){
            swapNodes(A, 2*i+2, i);
        } 
    } else if (2*i+1 < A->size){
        if (A->nodes[2*i+1].freq < curr){
            swapNodes(A, 2*i+1, i);
        }
    }
}

void buildMinHeap(minHeap * A){
    for (int i=floor(A->size/2)-1; i>=0; i--){
        minHeapify(A, i);
    }
}

struct minHeap createMinHeap(string dataArray, vector<int> freq){
    minHeap heap = minHeap();
    heap.size = dataArray.length();
    for (int i = 0; i<dataArray.length(); i++){
        minHeapNode node = minHeapNode();
        node.data = dataArray[i];
        node.freq = freq[i];
        (heap.nodes).push_back(node);
    }
    buildMinHeap(&heap);
    return heap;
}

struct minHeapNode * extractMin(minHeap * A){
    minHeapNode * min = &A->nodes[0];
    A->nodes[0] = A->nodes[A->size-1];
    A->size--;
    buildMinHeap(A);
    return min;
}

void insertMinHeapNode(minHeap * A, minHeapNode node){
    A->size++;
    A->nodes[A->size-1] = node;
    buildMinHeap(A);
}

//compute the frequencies of each letter stored in dataArray and store in freq array 
void comFreq(char * buffer){
    for (char* it=buffer; *it; it++){
        if (dataArray.find(*it) == string::npos){
            dataArray+=*it;
            freq.push_back(1);
        } else {
            freq[dataArray.find(*it)]++;
        }
    }
}

struct minHeapNode * buildHuffmanTree(minHeap * A){
    while (A->size > 1){
        minHeapNode * a = (minHeapNode *)extractMin(A);
        minHeapNode * b = (minHeapNode *)extractMin(A);
        minHeapNode c = minHeapNode();
        c.freq = a->freq + b->freq;
        c.data = '$';
        c.left = a;
        c.right = b;
        insertMinHeapNode(A, c);
    }
    return extractMin(A);
}

void printHuffmanCodes(minHeapNode * root, string code){
    if (!root->left && !root->right){
        cout << root->data << code << endl;
    }
    
    if (root->left){
        code = code + '0';
        printHuffmanCodes(root->left, code);
    } 
    if (root->right){
        code = code + '1';
        printHuffmanCodes(root->right, code);
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
        minHeap heap = createMinHeap(dataArray, freq);
        printHeap(heap);
        minHeapNode * root = buildHuffmanTree(&heap);

    } else {
        usage(argv[0], "Incorrect number of arguments.");
        return 1;
    }

    return 0;
}