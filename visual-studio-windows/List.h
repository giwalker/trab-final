#pragma once

#include "Node.h" 

class List {
	Node* head;

public:
	List();
	~List();
	void insertAfterLast(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8);
	int removeFirst();
	void listAll();
	void listSomeEvents(int d1, int m1, int y1, int d2, int m2, int y2);
	void listTimeON(int d1, int m1, int y1, int d2, int m2, int y2);
	void listBusyPeriod(int d, int m, int y);
};

