#ifndef priorityqueue
#define priorityqueue

#include <string>
#include <vector>


class PriorityQueue
{
protected:
    std::vector<std::string> heap;


public:

    PriorityQueue();
    virtual ~PriorityQueue();

    void push(std::string& val);
    void print();
    void insert(std::string& value);
    std::string pop_first();

};










#endif