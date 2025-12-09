#include "hash_table.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <utility>

using namespace std;

HashTable::HashTable()
{
    table.resize(DATASIZE);

}

HashTable::~HashTable()
{

}

string HashTable::hash(string& word)
{
    int hashval = 0;

    for (char c : word){
        int char_value = static_cast<int>(c);
        hashval = (hashval * 33 + char_value) % DATASIZE;
    }


    string hsh_final = std::to_string(abs(hashval));

    return hsh_final;
}

void HashTable::insert(string& key, string& value)
{
    string index = this->hash(key);
    int n_index = std::stoi(index);

    auto& bucket= table[n_index];

    for (auto& bkt : bucket){
        if (bkt.first == key){
            bkt.second = value;
            return;
        }
    }

    bucket.emplace_back(key, value);
    
}

string HashTable::query(string& key)
{
    string index = this->hash(key);
    int n_index = std::stoi(index);

    auto& buck = table[n_index];

    for (auto& bkt : buck){
        if (bkt.first == key){
            return bkt.second;
        }
    }
    throw std::runtime_error("Index with that Key not found");
}