#include "List.h"

//====================================================================//
//Constructor
List::List() {
	head = 0;
}
List::~List() {
	Node* cursor = head;
	while (head) {
		cursor = cursor->getNext();
		delete head;
		head = cursor;
	}
	head = 0;
}

//Return the size of the list
int List::listSize() {
	int aux_listSize = 0;
	Node* a = head;
	while (a != 0) {
		aux_listSize++;
		a = a->getNext();
	}
	return aux_listSize;
}
//Send the Linked List via USART and delete the list
void List::sendList() {
	Node* q = head;

	int size = 0;
	size_g = Log_1.listSize();

	EMB.USART_Write(size_g);
	EMB.USART_Write(q->getVal(1));

	//int usartOK = 0x41;

	//if (usartOK == 0x41){
	EMB.USART_Write(0xAA);		//Cabeçalho
	while (q != 0) {
		//EMB.USART_Write(q->getVal(1));
		EMB.USART_Write(q->getVal(2));
		EMB.USART_Write(q->getVal(3));
		EMB.USART_Write(q->getVal(4));
		EMB.USART_Write(q->getVal(5));
		EMB.USART_Write(q->getVal(6));
		EMB.USART_Write(q->getVal(7));
		EMB.USART_Write(q->getVal(8));
		q = q->getNext();
		removeFirst();
	}
	EMB.USART_Write(0x55);		//Rodapé
//}
//usartOK = 0;
}

//Insert Node after last Node
void List::insertAfterLast(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8) {
	Node* p = head;
	Node* q = head;

	if (head == 0)
		head = new Node(d1, d2, d3, d4, d5, d6, d7, d8, head);
	else {
		while (q != 0) {
			p = q;
			q = p->getNext();
		}
		p->setNext(new Node(d1, d2, d3, d4, d5, d6, d7, d8, 0));
	}
}

//Remove the first Node
int List::removeFirst() {
	int retval = 0;
	if (head != 0) {
		retval = head->getVal(1);
		Node* oldHead = head;
		head = head->getNext();
		delete oldHead;
	}
	return retval;
}
//====================================================================//