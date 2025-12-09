#include "LinkedListNode.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include <set>
#include <cstring>

int comparisons;
//compare function for merge sort
int smart_compare(std::string& str_a, std::string& str_b) {
    comparisons += 1;
    // Strip whitespace from both ends of str_a and str_b
    std::string a = str_a;
    while (a.size() > 0 && std::isspace(a[a.size() - 1]))
        a.erase(a.end() - 1);
    while (a.size() > 0 && std::isspace(a[0]))
        a.erase(a.begin());
    std::string b = str_b;
    while (b.size() > 0 && std::isspace(b[b.size() - 1]))
        b.erase(b.end() - 1);
    while (b.size() > 0 && std::isspace(b[0]))
        b.erase(b.begin());

    // Skip the parts that are the same
    unsigned int i = 0;
    while(i < a.size() && i < b.size() && a[i] == b[i]) {
        i++;
    }
    if (i >= a.size()) {
        if (i >= b.size()) {
            return 0;
        } else {
            return -1;
        }
    } else if (i >= b.size()) {
        return 1;
    }

    // Skip zeros
    unsigned int a_start = i;
    unsigned int b_start = i;
    while (a_start < a.size() && a[a_start] == '0')
        a_start++;
    while (b_start < b.size() && b[b_start] == '0')
        b_start++;

    // Count digits
    unsigned int a_digits = a_start;
    while (a_digits < a.size() && a[a_digits] >= '0' && a[a_digits] <= '9')
        a_digits++;
    unsigned int b_digits = b_start;
    while (b_digits < b.size() && b[b_digits] >= '0' && b[b_digits] <= '9')
        b_digits++;
    if (a_digits > 0 && a_digits < b_digits) {
        return -1; // a comes first because its number is shorter
    } else if (b_digits > 0 && b_digits < a_digits) {
        return 1; // b comes first because its number is shorter
    } else {
        // The numbers are the same length, so compare alphabetically
        return strcmp(a.c_str() + a_start, b.c_str() + b_start);
    }
}





//linked list node
int allocations = 0;

LinkedListNode::LinkedListNode(std::string& val)
{
    allocations++;
    this->next = nullptr;
    this->value = val;
}

//linked list node
int deallocations = 0;

LinkedListNode::~LinkedListNode()
{
    deallocations++;
    //delete(this->next);

    /*could uncomment that delete, and add delete(this->first) in 
    the linkedlist destructor, only do this if running into issues*/
}


//linked list
LinkedList::LinkedList(){
    this->first = nullptr;
    this->last = nullptr;
    this->length = 0;
}

//linked list
LinkedList::~LinkedList(){
    while(this->first != nullptr){
        LinkedListNode* temp = this->first;
        this->first = this->first->next;
        delete temp;
    }
    this->first = nullptr;
}

//linked list
void LinkedList::print()
{
        for(LinkedListNode* pNode = this->first; pNode; pNode = pNode->next)
        {
                if (pNode != this->first)
                        std::cout << " -> ";
                std::cout << pNode->value;
        }
        std::cout << std::endl;
} 


//linked list
void LinkedList::push_back(std::string& s)
{

    LinkedListNode* objptr = new LinkedListNode(s);

    

    if (this->length == 0){
        this->first = objptr;
        this->last = objptr;
        this->length++;
    }
    else{
        this->last->next = objptr;

        this->last = objptr;
        this->length++;
    }


}



//linked list
void LinkedList::push_front(std::string& s)
{
    
    LinkedListNode* front = new LinkedListNode(s);

    if (this->length == 0){
        this->first = front;
        this->last = front;
    }
    else{
        front->next = this->first;
        this->first = front;
    }
    

    this->length++;
    
}



//linked list
std::string LinkedList::pop_front()
{
    if (this->length == 0){
        throw std::runtime_error("List is empty, cannot pop_front");
    }

    
    LinkedListNode* temp = this->first;

    std::string copy_string;
    copy_string = temp->value;

    this->first = temp->next;

    if (this->first == nullptr){
        this->last = nullptr;
    }


    delete temp;
    this->length--;
    return copy_string;
}



//linked list
int LinkedList::size()
{
    return this->length;
}


void LinkedList::split(int n, LinkedList& other)
{

    if (n == 0){
        throw std::runtime_error("n must be greater than 0");
    }



    if (n >= this->length){
        other.first = this->first;
        other.last = this->last;
        other.length = this->length;

        this->first = this->last = nullptr;
        this->length = 0;
        return;
    }


    LinkedListNode* current = this->first;
    LinkedListNode* prev = nullptr;

    for (int i = 0; i < n && current != nullptr; i++){
        prev = current;
        current = current->next;
    }

    other.first = this->first;
    other.last = prev;
    other.last->next = nullptr;
    other.length = n;


    this->first = current;
    this->length -= n;

    if(this->first == nullptr){
        this->last = nullptr;
    }

}

//linked list
void LinkedList::check()
{
    if (this->first && !this->last)
        throw std::runtime_error("first but no last");
    if (!this->first && this->last)
        throw std::runtime_error("last but no first");
    if (this->first == this->last && this->first && this->first->next)
        throw std::runtime_error("first and last are the same, but first has a next");
    if (this->first != this->last && !this->first->next)
        throw std::runtime_error("first and last are different, but first has no next");
    int count = 0;
    LinkedListNode* pLast = nullptr;
    std::set<LinkedListNode*> seen_before;
    for (LinkedListNode* pNode = this->first; pNode; pNode = pNode->next)
    {
        if (seen_before.find(pNode) != seen_before.end())
            throw std::runtime_error("cycle or duplicate node in the linked list");
        seen_before.insert(pNode);
        count++;
        pLast = pNode;
    }
    if (count != this->length)
        throw std::runtime_error("length does not match the number of nodes in the list");
    if (pLast != this->last)
        throw std::runtime_error("last is not in the list");
    if (pLast && pLast->next)
        throw std::runtime_error("last has a next");
}



void LinkedList::merge(LinkedList& other)
{

    if (this->size() == 0 || other.size() == 0 ){
        throw std::runtime_error("One of the lists is empty");
    }

    LinkedList merged;

    while(this->length > 0 && other.size() > 0 ){
        std::string list_0 = this->pop_front();
        std::string list_1 = other.pop_front();

        int compval = smart_compare(list_0, list_1);

        
        if (compval <= 0){
            merged.push_back(list_0);
            other.push_front(list_1);
        }
        else if(compval > 0){
            merged.push_back(list_1);
            this->push_front(list_0);
        }
    }


    while(other.size() > 0){
        std::string list_1 = other.pop_front();
        merged.push_back(list_1);
    }

    while(this->length > 0){
        std::string list_0 = this->pop_front();
        merged.push_back(list_0);
    }

    //making this equal to merged
    while(merged.size() > 0){
        std::string temp = merged.pop_front();
        this->push_back(temp);
    }


}



int iterations;
//linkedlist
void LinkedList::sort()
{

    if (this->length < 2){
        return;
    }

    LinkedList temp;

    int middle = this->length / 2;

    this->split(middle + (this->length - 2 * middle), temp);

    this->sort();
    temp.sort();


    this->merge(temp);

    temp.print();

}


