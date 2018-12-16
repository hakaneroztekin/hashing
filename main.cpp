#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
// #include <windows.h>
#include <random>

using namespace std;
int tableSize = 21929; // default table size, it's capacity not determined in the pdf. this is also a prime number
int primeNumber = 251;

class HashNode { // Hash Node
public:
    int key;
    string value;
    HashNode(int key, string value);
    ~HashNode(){}
};

class HashMap{
private:
    HashNode **hashTable;
    long long int collisionOnInsertion = 0, collisionOnSearch = 0;
public:
    HashMap();
    int getCollisionOnInsertion(){return collisionOnInsertion;};
    int getCollisionOnSearch(){return collisionOnSearch;};

    int hashFunction(int k); // linear probing
    void insert(int k, string v);
    int search(string value);
    void deleteHash();
};

class DoubleHash {
    HashNode **hashTable;
    long long int collisionOnInsertion = 0, collisionOnSearch = 0;

public:
    DoubleHash();
    int getCollisionOnInsertion(){return collisionOnInsertion;};
    int getCollisionOnSearch(){return collisionOnSearch;};
    int firstHashFunction(int key){ return key % tableSize;};
    int secondHashFunction(int key){ return primeNumber - (key % primeNumber);};
    int generateNewIndex(int firstIndex, int secondIndex, int i);
    void insert(int key, string value);
    int search(string value);
    void deleteHash();
};
/* Universal Hashing;
 * Step 1 - m is selected as prime (tableSize is a prime number)
 *
 * */

class UniversalHash {
    HashNode **hashTable;
    int k0, k1, k2;
    int a0, a1, a2;
    unsigned long long int collisionOnInsertion = 0, collisionOnSearch = 0;

public:
    UniversalHash();
    int getCollisionOnInsertion(){return collisionOnInsertion;};
    int getCollisionOnSearch(){return collisionOnSearch;};

    void decomposeKey(int key); // Step 2 of Universal Hashing (Step 1 is already done because tableSize is a prime #)
    void generateChunksofA(); // Step 3
    int hash(int key); // Step 4

    void insert(int key, string value);
    int search(string value);
    void deleteHash();
};



class IOActions{
    ifstream dataFile;

public:
    IOActions(){}; // default constructor
    void openFile(string fileName); // take input file
    void saveFile(); // save output file
    void closeFile(); // close input file
    int readVocabToLinearHash(HashMap *hashMap);
    int readSearchToLinearHash(HashMap *hashMap);

    int readVocabToDoubleHash(DoubleHash *doubleHash);
    int readSearchToDoubleHash(DoubleHash *doubleHash);

    int readVocabToUniversalHash(UniversalHash *universalHash);
    int readSearchToUniversalHash(UniversalHash *universalHash);

    ifstream & getDataFile();
    ~IOActions(){}; // destructor
};

//
//string ExePath(){
//    char buffer[256];
//    GetModuleFileName( NULL, buffer, 256);
//    string::size_type pos = string( buffer ).find_last_of( "\\/" );
//    return string( buffer ).substr( 0, pos);
//}

void IOActions::openFile(string fileName){  // take input file
 //   string currentPath = ExePath(); // this returns cmake-build-debug folder (a folder for CLion config)
 //   string mainDirectory = currentPath.substr(0, currentPath.find_last_of("/\\")); // so the root directory path is extracted
 //   string fileFullPath = mainDirectory + "\\" + fileName;
    string fileFullPath = fileName;
    cout << "file full path: " << fileFullPath << endl;
    dataFile.open(fileFullPath);
    if (!dataFile.is_open()){
        cout << "File open error\n";
    }
    else{
		cout << fileName + " opened successfully.\n" << endl;
        return;
    }
}

int IOActions::readVocabToLinearHash(HashMap *hashMap){
    string line;
    int lineCounter = 0;

    while(getline(dataFile, line)){
        lineCounter++;
        hashMap->insert(lineCounter, line);
    }
    cout << "reading from vocab.txt is completed" << endl;
    return 0;
}

int IOActions::readVocabToDoubleHash(DoubleHash *doubleHash){
    string line;
    int lineCounter = 0;

    while(getline(dataFile, line)){
        lineCounter++;
        doubleHash->insert(lineCounter, line);
    }
    cout << "reading from vocab.txt is completed" << endl;
    return 0;
}

int IOActions::readVocabToUniversalHash(UniversalHash *universalHash){
    string line;
    int lineCounter = 0;

    while(getline(dataFile, line)){
        lineCounter++;
        universalHash->insert(lineCounter, line);
    }
    cout << "reading from vocab.txt is completed" << endl;
    return 0;
}

int IOActions::readSearchToLinearHash(HashMap *hashMap){
    string line;
    int lineCounter = 0;
    int foundKey;

    while(getline(dataFile, line)){
        lineCounter++;
        foundKey = hashMap->search(line);
        // cout << foundKey;
    }
    cout << "read search.txt completed" << endl;
    return 0;
}


int IOActions::readSearchToDoubleHash(DoubleHash *doubleHash){
    string line;
    int lineCounter = 0;
    int foundKey;

    while(getline(dataFile, line)){
        lineCounter++;
        foundKey = doubleHash->search(line);
        // cout << foundKey;
    }
    cout << "read search.txt completed" << endl;
    return 0;
}


int IOActions::readSearchToUniversalHash(UniversalHash *universalHash){
    string line;
    int lineCounter = 0;
    int foundKey;

    while(getline(dataFile, line)){
        lineCounter++;
        foundKey = universalHash->search(line);
        // cout << foundKey;
    }
    cout << "read search.txt completed" << endl;
    return 0;
}

void IOActions::saveFile(){ // save output file, not needed for this assignment
}

void IOActions::closeFile(){
    dataFile.close();
}


HashNode::HashNode(int key, string value){
    this->key = key;
    this->value = value;
}


HashMap::HashMap() {
    hashTable = new HashNode *[tableSize];
    for (int i = 0; i < tableSize; i++)
        hashTable[i] = NULL;
}

void HashMap::deleteHash(){
    for(int i = 0; i < tableSize; i++){
        if(hashTable[i] != NULL)
            delete hashTable[i];
    }
    delete [] hashTable;
}

int HashMap::hashFunction(int key){ // In the pdf, it's said as, h(k,i) = (k+i)modm for linear probing.
    // though, in Ninova, it's said that we'll use line numbers as keys. So here I use the mod operation.
    return key % tableSize;
};

void HashMap::insert(int key, string value){
    int index = hashFunction(key);
    int initialValue = -1;

    if(hashTable[index] != NULL)
        collisionOnInsertion++;;

    while(index != initialValue && (hashTable[index] != NULL && hashTable[index]->key != key)){
        if(initialValue == -1)
            initialValue = index;
        index = hashFunction(index + 1);
    }
    if (hashTable[index] == NULL || index == initialValue){
            hashTable[index] = new HashNode(key, value);
    }
    if(index != initialValue){
        if (hashTable[index] != NULL) {
            if (hashTable[index]->key == key)
                hashTable[index]->value = value;
        }
        else
            hashTable[index] = new HashNode(key, value);
    }

    //cout << "hashTable[" << index << "]" << hashTable[index] << endl;
}

/*
 * Because of the misconceptions discussed in Ninova, it's not crystal clear on how to implement the hashes.
 * Here especially, we hashed as <key,value> (i.e. <line#,word>) but because the line # for the same word differs in both of the txt files
 * Even if a word exists in both files, it won't be found, because of the line numbers.
 * So, I implemented this to search for values instead of keys.
*/
int HashMap::search(string value) {
    int index = 1; // key counter, i.e. line counter
    string hashValueAtIndex;
    int foundKey = -1;

    while(index < tableSize){
        hashValueAtIndex = this->hashTable[index]->value;
        if(hashTable[index] != NULL){
            collisionOnSearch++;
        }
        if(hashValueAtIndex.compare(value) == 0){ // value is found
            foundKey = index;
            cout << "Found:\t #" << foundKey << ": " << value << endl;
            return foundKey; // how we use search results is not declared in the pdf. we'll return this but won't use it.
        }
        index++;
    }
    return -1;
    /*
     * int index = hashFunction(key);
    string foundValue;
    int initialValue = -1;

    while (index != initialValue && (hashTable[index] != NULL && hashTable[index]->value != value)) {
        if (initialValue == -1)
            initialValue = index;
        index = hashFunction(index + 1);
    }
    if (hashTable[index] == NULL || index == initialValue) {
        foundValue = "Not Found."; // the cout message
    }
    else{
        foundValue = hashTable[index]->value;
        cout << "Found: "; // first half of the cout message
    }
    cout << foundValue << endl; // the common print out message
    return foundValue;*/
}


DoubleHash::DoubleHash(){
    hashTable = new HashNode *[tableSize];
    for (int i = 0; i < tableSize; i++)
        hashTable[i] = 0;
}

void DoubleHash::insert(int key, string value) {
    int index = firstHashFunction(key);
    int i, takeNewIndex = 0;
    int newIndex = 1;

    // if there is a collision, get help from second hash function
    if(hashTable[index] != 0){ // if not 0
        collisionOnInsertion++;
        i = 1;
        int secondIndex = secondHashFunction(key);
        while(hashTable[newIndex] != 0) { // loop until finding an empty index
            newIndex = generateNewIndex(index, secondIndex, i);
            i++;
        }
    }
    else
        hashTable[index] = new HashNode(key, value);

    //cout << "hashTable[" << index << "]" << hashTable[index] << endl;
}

int DoubleHash::generateNewIndex(int firstIndex, int secondIndex, int i) {
    // h(k, i) = (h1(k) + i * h2(k)) mod m
    return (firstIndex + i * secondIndex) % tableSize;
}

int DoubleHash::search(string value) {
    int index = 1; // key counter, i.e. line counter
    string hashValueAtIndex;
    int foundKey = -1;

    while(index < tableSize){
        hashValueAtIndex = this->hashTable[index]->value;
        if(hashTable[index] != NULL){
            collisionOnSearch++;
        }
        if(hashValueAtIndex.compare(value) == 0){ // value is found
            foundKey = index;
            cout << "Found:\t #" << foundKey << ": " << value << endl;
            return foundKey; // how we use search results is not declared in the pdf. we'll return this but won't use it.
        }
        index++;
    }
    return -1;
}

void DoubleHash::deleteHash(){
    for(int i = 0; i < tableSize; i++){
        if(hashTable[i] != NULL)
            delete hashTable[i];
    }
    delete [] hashTable;
}

UniversalHash::UniversalHash(){
    k0 = 0; k1 = 0; k2 = 0;
    a0 = 0; a1 = 0; a2 = 0;

    hashTable = new HashNode *[tableSize];
    for (int i = 0; i < tableSize; i++)
        hashTable[i] = NULL;
};

void UniversalHash::decomposeKey(int key){
    if(key < 100){
        k2 = key;
    }

    else if(key < 10000){
        k2 = key % 100;
        int tempKey = key;
        tempKey = tempKey / 100;
        k1 = int(tempKey);
    }

    else if(key < 100000){
        k2 = key % 100;

        int tempKey = key;
        tempKey = tempKey / 100;
        k1 = int(tempKey % 100);

        tempKey = tempKey / 100;
        k2 = int(tempKey % 100);
    }

   // cout << "key: " << key << " k1: " << k1 << " k2: " << k2 << endl;
};

void UniversalHash::generateChunksofA(){
    a0 = rand() % tableSize;
    a1 = rand() % tableSize;
    a2 = rand() % tableSize;
};

int UniversalHash::hash(int key){
    int hashValue = 0;

    decomposeKey(key);
    generateChunksofA();

    hashValue = a0 * k0 + a1 * k1 + a2 * k2; // because r = 3, arrays and loops are not used
    return hashValue % tableSize;
};

void UniversalHash::insert(int key, string value) {
    int index = hash(key);
    int initialValue = -1;

    if(hashTable[index] != NULL)
        collisionOnInsertion++;;

    while(index != initialValue && (hashTable[index] != NULL && hashTable[index]->key != key)){
        if(initialValue == -1)
            initialValue = index;

        index = hash(index + 1);
    }
    if (hashTable[index] == NULL || index == initialValue){
        hashTable[index] = new HashNode(key, value);
    }
    if(index != initialValue){
        if (hashTable[index] != NULL) {
            if (hashTable[index]->key == key)
                hashTable[index]->value = value;
        }
        else
            hashTable[index] = new HashNode(key, value);
    }

    // cout << "hashTable[" << index << "]" << hashTable[index] << endl;
}

int UniversalHash::search(string value) {
    int checkedKeyCounter = 1; // key counter
    int index = hash(checkedKeyCounter); // hash the line # to search in the hashed keys

    string hashValueAtIndex;
    int foundKey = -1;

    while(index < tableSize){
        index = hash(checkedKeyCounter);
        if(hashTable[index] != NULL){
            collisionOnSearch++;
            hashValueAtIndex = hashTable[index]->value;
            if(hashValueAtIndex.compare(value) == 0){ // value is found
                foundKey = index;
                cout << "Found:\t #" << foundKey << ": " << value << endl;
                return foundKey; // how we use search results is not declared in the pdf. we'll return this but won't use it.
            }
            checkedKeyCounter++;
        }
    }
    return -1;
}

void UniversalHash::deleteHash(){
    for(int i = 0; i < tableSize; i++){
        if(hashTable[i] != NULL)
            delete hashTable[i];
    }
    delete [] hashTable;
}
int main(int argc, char * argv[]){
    HashMap hashMap; // linear probing
    DoubleHash doubleHash;  // double hash
    UniversalHash universalHash; // universal hash
    IOActions ioActions; // file and file&hash operations
    int choice = 0;
    string vocabFile = argv[1];
    string searchFile = argv[2];

    while(choice != 5){
        cout << "\nSEARCH WITH:" << endl
             << "1- Linear Probing" << endl
             << "2- Double Hashing" << endl
             << "3- Universal Hashing" << endl
             << "4- Collision Results" << endl
             << "5- Exit" << endl;
        cin >> choice;
        switch(choice){

            case 1:
                cout << "Using linear probing.\n";
                // Insert Vocab.txt to hash here
                cout << "Inserting vocab.txt to hash\n";
                ioActions.openFile("vocab.txt");
                ioActions.readVocabToLinearHash(&hashMap);
                ioActions.closeFile();

                // Search operation
                ioActions.openFile("search.txt");
                ioActions.readSearchToLinearHash(&hashMap);
                ioActions.closeFile();
                break;

            case 2:
                cout << "Using Double Hashing.\n";
                // Insert Vocab.txt to hash here
                cout << "Inserting vocab.txt to hash\n";
                ioActions.openFile("vocab.txt");
                ioActions.readVocabToDoubleHash(&doubleHash);
                ioActions.closeFile();

                // Search operation
                ioActions.openFile("search.txt");
                ioActions.readSearchToDoubleHash(&doubleHash);
                ioActions.closeFile();
                break;

            case 3:
                cout << "Using Universal Hashing.\n";
                // Insert Vocab.txt to hash here
                cout << "Inserting vocab.txt to hash\n";
                ioActions.openFile(vocabFile);
                ioActions.readVocabToUniversalHash(&universalHash);
                ioActions.closeFile();

                // Search operation
                ioActions.openFile(searchFile);
                ioActions.readSearchToUniversalHash(&universalHash);
                ioActions.closeFile();
                break;

            case 4:
                cout << "Collusion Results.\n";
                // Insert Vocab.txt to hash here
                cout << "Collision On Insertion\n";
                cout << "#1 Linear Probing: "    << hashMap.getCollisionOnInsertion()       << endl
                     << "#2 Double Hashing: "    << doubleHash.getCollisionOnInsertion()    << endl
                     << "#3 Universal Hashing: " << universalHash.getCollisionOnInsertion() << endl;

                cout << "Collision On Search\n";
                cout << "#1 Linear Probing: "    << hashMap.getCollisionOnSearch()       << endl
                     << "#2 Double Hashing: "    << doubleHash.getCollisionOnSearch()    << endl
                     << "#3 Universal Hashing: " << universalHash.getCollisionOnSearch() << endl;

                break;

            default:
                break;
        }
    }
    return 0;
}

