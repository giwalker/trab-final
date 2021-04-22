#include "Node.h"

//====================================================================//
//Constructor
Node::Node(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, Node* nxt) {
    id = d1; nmr = d2; evt = d3; hours = d4; mins = d5; day = d6; month = d7; year = d8;
    next = nxt;
}

//Get the value needed from the list
int Node::getVal(int sel) {
    int val = 0;
    if (sel == 1) val = id;
    if (sel == 2) val = nmr;
    if (sel == 3) val = evt;
    if (sel == 4) val = hours;
    if (sel == 5) val = mins;
    if (sel == 6) val = day;
    if (sel == 7) val = month;
    if (sel == 8) val = year;
    return val;
}

//Returns a pointer for the next node in the list
Node* Node::getNext() {
    return next;
}

//Stores the values inside the node
void Node::setVal(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8) {
    if (d1 >= 0) id = d1;
    if (d2 >= 0) nmr = d2;
    if (d3 >= 0) evt = d3;
    if (d4 >= 0) hours = d4;
    if (d5 >= 0) mins = d5;
    if (d6 >= 0) day = d6;
    if (d7 >= 0) month = d7;
    if (d8 >= 0) year = d8;
}

//Stores the pointer to the next node in the list
void Node::setNext(Node* nxt) {
    next = nxt;
}
//====================================================================//