#ifndef LAB3_SRC_HASH_TABLE_H_
#define LAB3_SRC_HASH_TABLE_H_

#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <typeinfo>
#include <string.h>
#define INIT_SIZE (2048)

using std::string;
using std::vector;
using std::endl;
using std::cout;

// each hash entry stores a key, object pair
template <typename Data>
struct HashTableEntry {
    string _key;
    Data _data;
};

// this is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTable {
    
protected:
    enum { TableSize = INIT_SIZE };
    
    // vector of the hash buckets.
    
    
    // obtain the hash code of a key
    
    
    HashTableEntry<Data> newEntry;
    
public:
        // number of buckets
    
		vector<vector<HashTableEntry<Data> > > _buckets;
		
        int nElement;
 
        HashTable();
    
        int hash(string key);

        // add a record to the hash table & returns true if key already exists.
        // substitute content if key already exists.
        bool insertItem(string key, Data data);

        // find a key in the dictionary and place in "data" the corresponding record
        // returns false if key is does not exist
        bool find(string key, Data * data);

        // removes an element in the hash table & return false if key does not exist.
        bool removeElement(string key);

        // returns the data that corresponds to this index.
        // data is initialized to 0s if the entry does not exist
        Data operator[] (string &key);
    
        int getTableSize();
};

template <typename Data>
int HashTable<Data>::hash(string key) {
    // TODO
    unsigned int hsum = 0;
    for(int i = 0; (unsigned)i < key.length(); i++){
        hsum += (unsigned) key[i];
    }
    return hsum % TableSize;
}

template <typename Data>
HashTable<Data>::HashTable() {
    // TODO
/*	
	_buckets = new HashTableEntry<Data> *[TableSize];	
	for(int i = 0;i < TableSize; i++){
        _buckets[i] = NULL;
    }
*/
    _buckets.resize(TableSize);
	nElement = 0;
}

template <typename Data>
bool HashTable<Data>::insertItem(string key, Data data) {
    // TODO
    int hashcode = hash(key);
	for(typename vector<HashTableEntry<Data> >::iterator itr = _buckets[hashcode].begin(); itr != _buckets[hashcode].end(); itr++){
		if(!(*itr)._key.compare(key)){
			(*itr)._data = data;
            return true;
		}
	}
    newEntry._key = key;
    newEntry._data = data;
    _buckets[hashcode].push_back(newEntry);
    nElement++;
    return false;
}

template <typename Data>
bool HashTable<Data>::find(string key, Data * data) {
    // TODO
    int hashcode = hash(key);
    for(typename vector<HashTableEntry<Data> >::iterator itr = _buckets[hashcode].begin(); itr != _buckets[hashcode].end(); itr++){
        if(!(*itr)._key.compare(key)){
            *data = (*itr)._data;
            return true;
        }
        
    }
    return false;
}

template <typename Data>
Data HashTable<Data>::operator[] (string &key) {
    // TODO
    int hashcode = hash(key);
    for(typename vector<HashTableEntry<Data> >::iterator itr = _buckets[hashcode].begin(); itr != _buckets[hashcode].end(); itr++){
        if(!(*itr)._key.compare(key)){
            return (*itr)._data;
        }
    }
    
    return NULL;
}

template <typename Data>
bool HashTable<Data>::removeElement(string key) {
    // TODO
    int hashcode = hash(key);
    for(typename vector<HashTableEntry<Data> >::iterator itr = _buckets[hashcode].begin(); itr != _buckets[hashcode].end(); itr++){
        if(!(*itr)._key.compare(key)){
            _buckets[hashcode].erase(itr);
            nElement--;
            return true;
        }
    }
    
    return false;
}


template<typename Data>
int HashTable<Data>::getTableSize(){
    return TableSize;
}



template <typename Data>
class HashTableIterator {
    int _currentBucket;
    HashTable<Data> _iterativeHashTable;
    typename vector<HashTableEntry<Data> >::iterator itrOfBucket;
    
    
 public:
    HashTableIterator(HashTable<Data> hashTable);
    bool next(string &key, Data &data);
    void setIteratorHashTable(HashTable<Data> hashTable);
    void resetIteratorToInitial();
};

template <typename Data>
HashTableIterator<Data>::HashTableIterator(HashTable<Data> hashTable) {
    // TODO
    _currentBucket = 0;
    _iterativeHashTable = hashTable;
    itrOfBucket = _iterativeHashTable._buckets.at(_currentBucket).begin();
}

template <typename Data>
bool HashTableIterator<Data>::next(string &key, Data &data) {
    // TODO
    //std::cout<<"At least Enter iterator"<<std::endl;
    //

    if(itrOfBucket != _iterativeHashTable.template _buckets.at(_currentBucket).end()){
        //cout<<_currentBucket<<endl;
        //key = (*itrOfBucket)._key.c_str();
        //data = (*itrOfBucket)._data;
        std::cout<<"Key: "<<key<<std::endl;
        std::cout<<"Data: "<<data<<std::endl;
        itrOfBucket++;
        return true;
    }
    else{
        _currentBucket++;
    }
    //std::cout<<"Here"<<endl;
    if(_currentBucket >= _iterativeHashTable.template getTableSize()){
        return false;
    }
    //std::cout<<"Here2"<<endl;

    while(_currentBucket < _iterativeHashTable.template getTableSize() && _iterativeHashTable.template _buckets.at(_currentBucket).empty()){
        _currentBucket++;
    }
    //std::cout<<"Here3"<<endl;
    if(_currentBucket >= _iterativeHashTable.template getTableSize()){
        return false;
    }
    else{
        itrOfBucket = _iterativeHashTable.template _buckets.at(_currentBucket).begin();
        key = (*itrOfBucket)._key;
        data = (*itrOfBucket)._data;
        //std::cout<<"Key: "<<key<<std::endl;
        //std::cout<<"Data: "<<data<<std::endl;
        //cout<<_currentBucket<<endl;
        itrOfBucket++;
        return true;
    }
    //std::cout<<"Here4"<<endl;

}

template <typename Data>
void HashTableIterator<Data>::setIteratorHashTable(HashTable<Data> hashTable){
    _iterativeHashTable = hashTable;
}

template <typename Data>
void HashTableIterator<Data>::resetIteratorToInitial(){
    _currentBucket = 0;
}


#endif  // LAB3_SRC_HASH_TABLE_H_
