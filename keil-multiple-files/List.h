#pragma once

#include "Node.h"

//====================================================================//
class List {
public:
	Node* head;
	List();
	~List();
	void insertAfterLast(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8);
	int removeFirst();
	int listSize();
	void sendList();
};
//====================================================================//