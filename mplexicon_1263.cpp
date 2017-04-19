/* TU Taylor    cs435 1263 mp */

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <iterator>

using namespace std;

int g_Asize;
int g_m;
int g_Alength;

//usage function to tell the user how to run the program  
void usage_1263(char *progname, string msg){
    cerr << "Error: " << msg << endl;
    cerr << "usage is: " << progname<< " [filename]" << endl;
}

struct lexicon_1263{
	int * T;
	char * A;
};

//hashes string 
int Hash_1263(int i, string word){	
	int sum = 0;

	//converts the word into sum of ascii chars
	for (int j=0; j<word.length(); j++){
		sum += (int)word[j];
	}

	return ((sum%g_m) + i*i) % g_m; //quadratic probing
}


//make sure the hashes are empty
void HashCleanup_1263(lexicon_1263 * L){
	if ((*L).A){
		free((*L).A);
	} 
	if ((*L).T){
		free((*L).T);
	}
	(*L).A = NULL;
	(*L).T = NULL;

}

// Create T, A. T will have m slots; A should be 8m
void HashCreate_1263 (lexicon_1263 * L, int m){
	g_m = m;

	//free and fill T with -1 values 
	(*L).T = (int *)malloc(sizeof(int) * m);
	memset((*L).T, -1, sizeof(int) * m);

	//free and fill A with spaces 
	(*L).A = (char*)malloc(8*m);
	memset((*L).A, ' ', 8);
}

// HashEmpty_1263 (lexicon L); // Check if L is empty
// HashFull_1263 (lexicon L); // Check if L can maintain more words

// Print of L
void HashPrint_1263 (lexicon_1263 l){
	cout << endl;
	cout << "	T 				A: ";

	for (int i=0; i < g_Asize; i++){
		if (l.A[i] == ' '){
			continue;
		}
		if (l.A[i] == '\0'){
			cout << "\\";
		} else {
			cout << l.A[i];
		}
	}
	
	for (int i=0; i < g_m; i++){
		cout << i;
		if (l.T[i] == -1){
			cout << endl;
		} else {
			cout << l.T[i] << endl;
		}
	}
	cout << endl;
} 


//Insert w into L (and T and A)
int HashInsert_1263 (lexicon_1263 L, string w){
	for (int i=0; i<g_m; i++){
		int hash = Hash_1263(i, w);
		int index = L.T[hash];

		if (index == -1){ //word not in table 

			L.T[hash] = g_Alength;

			char * word = new char[w.length() + 1];
			strcpy(word, w.c_str()); 

			strncpy(&L.A[g_Alength], word, strlen(word) + 1);

			g_Alength += w.length();
			
			return hash;
		// } else if (w == string(L.A[index])){
			// return -1; //word has already been inserted 
		}
	}

	return -2; 
} 

//Delete w from L (but not necessarily from A)
void HashDelete_1263 (lexicon_1263 L, vector<char> word){

} 

//Search for string in L (and this means T)
int HashSearch_1263 (lexicon_1263 L, string w){
	for (int i=0; i<g_m; i++){
		int hash = Hash_1263(i, w);

		if (L.T[hash] != -1){
			return hash;
		}
	}

	return -1;
} 

void HashBatch_1263 (lexicon_1263 l, char * filename){
	
	ifstream infile (filename, ios::in);
	
	if(!infile.is_open()){
		usage_1263(filename, "Could not open file.");
		return;
	}

	string op;
	char ch = ' ';
	vector<char> line;

	while (infile.get(ch)){

		if (ch!='\n' && ch!=EOF){
			line.push_back(ch);
		} else {
			line.push_back('\0');
			
			op = string(line.begin(), line.begin()+2);

			cout << op;

			if (op=="13"){
				HashPrint_1263(l);
			}

			string word = string(line.begin()+3, line.end());
			cout << word << endl;
			// word.erase(word.begin(), word.begin()+3);
			// word.push_back('\0');

			if (op=="10"){
				//insert
				HashInsert_1263(l, word);

			} else if(op=="11") {
				//delete
			
			} else if (op=="12"){
				//search
				int found = HashSearch_1263(l, word);

				cout << word;

				if (found == -1){
					cout << " not found." << endl;
				} else {
					cout << " found in slot " << found << "." << endl;
				}

			} else if (op=="14"){
				//create 
				HashCleanup_1263(&l);
				HashCreate_1263(&l, atoi(word.c_str()));
			}


			line.clear();
			op.clear();
		}


		
	}

}

int main(int argc, char **argv){

	if (argc == 2){

		lexicon_1263 l = lexicon_1263();

		HashBatch_1263(l, argv[1]);

		return 0;
	} else {
		usage_1263(argv[0], "Incorrect number of arguments.");
        return 1;
	}

	return 0;
}

