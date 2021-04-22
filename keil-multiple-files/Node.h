#pragma once
//====================================================================//
class Node {
	int nmr, id, evt, hours, mins, day, month, year;
protected:
	Node* next;
public:
	Node(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, Node* nxt);
	int getVal(int sel);
	Node* getNext();
	void setVal(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8);
	void setNext(Node* nxt);
};
//====================================================================//