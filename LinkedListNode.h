#ifndef linked_list
#define linked_list

#include <string>

// Represents a single node in a linked list
class LinkedListNode
{
protected:
    LinkedListNode* next;
    std::string value;

public: 
    LinkedListNode(std::string& val);
    virtual ~LinkedListNode();

    friend class LinkedList;
};



// A data structure that holds a list of strings
class LinkedList
{
protected:
    LinkedListNode* first;
    LinkedListNode* last;
    int length;

public:
    LinkedList();
    virtual ~LinkedList();
    void print();
    void push_back(std::string& s);
    void push_front(std::string& s);
    std::string pop_front();
    int size();
    void split(int n, LinkedList& other);
    void check();

    //merge sort functions
    void merge(LinkedList& other);
    void sort();

    //friend class LinkedListNode;
};

#endif