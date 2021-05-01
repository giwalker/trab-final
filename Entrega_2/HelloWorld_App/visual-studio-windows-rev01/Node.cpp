
#include "Node.h"

// Constructor - initializes the node
Node::Node(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, Node* nxt) {
    nmr = d1; id = d2; evt = d3; hours = d4; mins = d5; day = d6; month = d7; year = d8;
    next = nxt;
}


// getVal returns the integer value stored in the node
int Node::getVal(int sel) {
    int val = 0;
    if (sel == 1) val = nmr;
    if (sel == 2) val = id;
    if (sel == 3) val = evt;
    if (sel == 4) val = hours;
    if (sel == 5) val = mins;
    if (sel == 6) val = day;
    if (sel == 7) val = month;
    if (sel == 8) val = year;
    return val;
}

// getNext returns a pointer for the next node in the linked list
Node* Node::getNext() {
    return next;
}

// setVal stores the integer value in the node
void Node::setVal(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8) {
    if (d1 >= 0) nmr = d1;
    if (d2 >= 0) id = d2;
    if (d3 >= 0) evt = d3;
    if (d4 >= 0) hours = d4;
    if (d5 >= 0) mins = d5;
    if (d6 >= 0) day = d6;
    if (d7 >= 0) month = d7;
    if (d8 >= 0) year = d8;
}

// setNext stores the pointer to the next node in the list in the "next" field
void Node::setNext(Node* nxt) {
    next = nxt;
}

// operator<< overload prints the data of the list in the form "Id | No | Event | Hour:Min | DD/MM/YY"
ostream& operator<< (ostream& c, Node* t) {
    // Id
    c << setfill('0') << setw(3) << t->getVal(1) << " | "
    // Number of the node on the list
      << setfill('0') << setw(3) << t->getVal(2) << " | "
    // Event type 
      << t->getVal(3) << " | " 
    // Hour:Minute
      << setfill('0') << setw(2) << t->getVal(4) << ":" << setfill('0') << setw(2) << t->getVal(5) << " | " 
    // Day/Month/Year
      << setfill('0') << setw(2) << t->getVal(6) << "/" << setfill('0') << setw(2) << t->getVal(7) << "/" << setfill('0') << setw(2) << t->getVal(8) << endl;

    return c;
}
