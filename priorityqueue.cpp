#include "priorityqueue.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>
using namespace std;


extern int smart_compare(std::string& str_a, std::string& str_b);


PriorityQueue::PriorityQueue()
{
    this->heap.push_back("");


}

PriorityQueue::~PriorityQueue()
{

}

void PriorityQueue::push(string& val){
    this->heap.push_back(val);
}

void PriorityQueue::print(){

    for (unsigned int i = 0; i < this->heap.size(); i++){
        cout << heap[i];
        cout << endl;
    }
}

void PriorityQueue::insert(std::string& value)
{

    if (this->heap.size() == 1){
        this->heap.push_back(value);
        return;
    }

    this->heap.push_back(value);

    int i = this->heap.size() - 1;

    while (i > 1){
        int compare = smart_compare(heap[i], heap[i/2]);


        if (compare < 0){
            std::swap(heap[i], heap[i/2]);
            i = i / 2;
        }
        else if (compare == 0 || compare > 0){
            break;
        }
     
    }   

    //ordering of children
    for (unsigned int i = 1; i < this->heap.size(); i++){
        unsigned int left = i * 2;
        unsigned int right = i * 2 + 1;
        if (left < heap.size() && right < heap.size() && smart_compare(heap[right], heap[left]) < 0){
            std::swap(heap[left], heap[right]);
        }
    }

}

string PriorityQueue::pop_first()
{
    if (this->heap.size() <= 1){
        throw std::runtime_error("Nothing in the Queue to pop");
    }

    string return_val = this->heap[1];

    std::swap(heap[1], heap.back());

    this->heap.pop_back();

    if (this->heap.size() <= 1){
        return return_val;
    }

    int i = 1;

    while (true){
    unsigned int left = i * 2;
    unsigned int right = (i * 2) + 1;
    int current = i;


    if (left < heap.size() && smart_compare(heap[left], heap[current]) < 0){
        current = left;
    }

    if(right < heap.size() && smart_compare(heap[right], heap[current]) < 0 ){
        current = right;
    }

    if (current != i){
        std::swap(heap[i], heap[current]);
        i = current;
    }
    else{
        break;
    }

    }


    return return_val;
}