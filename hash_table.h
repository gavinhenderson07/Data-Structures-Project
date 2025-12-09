#ifndef hash_table
#define hash_table

#include <string>
#include <vector>
#include <utility>


class HashTable{
protected:
    std::vector<std::vector<std::pair<std::string, std::string>>> table;
    int DATASIZE = 100;



public: 

    HashTable();
    virtual ~HashTable();

    std::string hash(std::string& word);
    void insert(std::string& key, std::string& value);
    std::string query(std::string& key);


};








#endif