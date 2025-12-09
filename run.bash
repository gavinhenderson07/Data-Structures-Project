#!/bin/bash
set -e
g++ -Wall main.cpp config.cpp charmatrix.cpp LinkedListNode.cpp binarysearch.cpp priorityqueue.cpp hash_table.cpp -o count
./count $*