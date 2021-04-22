
#include "List.h"
#include <iostream>
using namespace std;

template <typename T>
void Swap(T& n1, T& n2) {
	T temp;
	temp = n1;
	n1 = n2;
	n2 = temp;
}

// Constructor - initialize the list
List::List() {
	head = 0;
}

// Destructor - Deinitialize the node
List::~List() {
	Node* cursor = head;
	while (head) {
		cursor = cursor->getNext();
		delete head;
		head = cursor;
	}
	head = 0; 
}

// insertAfterLast insert a node in the linked list after the last one already existing
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

// removeFirst remove the first node on the linked list
int List::removeFirst() {
	int retval = 0;
	if (head != 0) {
		cout << "Removendo: " << head << endl;
		cout << "e fica:" << head->getVal(1) << endl;
		retval = head->getVal(1);
		Node* oldHead = head;
		head = head->getNext();
		delete oldHead;
	}
	return retval;
}

// listAll print all the Nodes of a list withOUT Operator Overloading. 
/* void List::listAll() {
	Node* aux = head;
	while (aux != 0) {
		cout << setfill('0') << setw(3) << aux->getVal(1) << " | ";
		cout << setfill('0') << setw(3) << aux->getVal(2) << " | ";
		cout << aux->getVal(3) << " | ";
		cout << setfill('0') << setw(2) << aux->getVal(4) << ":" << setfill('0') << setw(2) << aux->getVal(5) << " | ";
		cout << setfill('0') << setw(2) << aux->getVal(6) << "/" << setfill('0') << setw(2) << aux->getVal(7) << "/"
			<< setfill('0') << setw(2) << aux->getVal(8) << endl;
		aux = aux->getNext();
	}
}*/

// listAll print all the Nodes of a list with Operator Overloading. 
//OBS: Print only the Nodes of the list related to the inserted Id
void List::listAll() {
	Node* aux = head;
	while (aux != 0) {
		cout << aux;		//Sobrecarga de operadores aqui. Definição do operator<< no Node.cpp
		aux = aux->getNext();
	}
}

// listSomeEvents print the nodes that exists between the two dates entered
void List::listSomeEvents(int d1, int m1, int y1, int d2, int m2, int y2) {
	//Organize entries (if needed), so that date 1 is more recent than date 2
	if (y1 > y2) {
		//Invert
		Swap(y1, y2);
		cout << "Inverti - y" << endl;
	}
	if (y1 == y2) {
		if (m1 > m2) {
			//Invert
			Swap(m1, m2);
			cout << "Inverti - m" << endl;
		}
		if (m1 == m2) {
			if (d1 > d2) {
				//Invert
				Swap(d1, d2);
				cout << "Inverti - d" << endl;
			}
			if (d1 == d2) cout << "Error: Equal dates..." << endl;
		}
	}
	cout << "Events between " << d1 << "/" << m1 << "/" << y1 << " and " << d2 << "/" << m2 << "/" << y2 << endl;

	Node* aux = head;
	bool FLAG_None = true;

	while (aux != 0) {
		bool FLAG_Aux_1 = false, FLAG_Aux_2 = false;
	
		if (y1 < aux->getVal(8)) FLAG_Aux_1 = true;
		if (y1 == aux->getVal(8)) {
			if (m1 < aux->getVal(7)) FLAG_Aux_1 = true;
			if (m1 == aux->getVal(7)) {
				if ((d1 < aux->getVal(6)) || (d1 == aux->getVal(6))) FLAG_Aux_1 = true;
			}
		}
		if (y2 > aux->getVal(8)) FLAG_Aux_2 = true;
		if (y2 == aux->getVal(8)) {
			if (m2 > aux->getVal(7)) FLAG_Aux_2 = true;
			if (m2 == aux->getVal(7)) {
				if ((d2 > aux->getVal(6)) || (d2 == aux->getVal(6))) FLAG_Aux_2 = true;
			}
		}

		if (FLAG_Aux_1 && FLAG_Aux_2) {
			FLAG_None = false;
			cout << aux;
			FLAG_Aux_1 = false; FLAG_Aux_2 = false;
		}
		aux = aux->getNext();
	}
	if (FLAG_None) cout << "No events between these dates" << endl;
	else FLAG_None = true;
}	//End listSome()

// listTimeOn print the total time that the system was ON between two entered dates
void List::listTimeON(int d1, int m1, int y1, int d2, int m2, int y2) {
	int total_time_on = 0;
	bool FLAG_Aux_1 = false, FLAG_Aux_2 = false;
	int h_on = 0, h_off = 0, m_on = 1, m_off = 0;
	bool FLAG_Event_OFF = false, FLAG_Event_ON = false;

	//Organize entries (if needed), so that date 1 is more recent than date 2
	if (y1 > y2) {
		//Invert
		Swap(y1, y2);
		cout << "Inverti - y" << endl;
	}
	if (y1 == y2) {
		if (m1 > m2) {
			//Invert
			Swap(m1, m2);
			cout << "Inverti - m" << endl;
		}
		if (m1 == m2) {
			if (d1 > d2) {
				//Invert
				Swap(d1, d2);
				cout << "Inverti - d" << endl;
			}
			if (d1 == d2) cout << "Error: Equal dates..." << endl;
		}
	}
	
	Node* aux = head;

	//It will be very difficult to understand this bottom part :P
	//But the important thing is that it works XD
	//Depending on the date entry, the value is down by 1 minut, because im not getting the seconds, and I didn't spend a lot
	//of time in this specific section
	while (aux != 0) {
		if (y1 < aux->getVal(8)) FLAG_Aux_1 = true;
		if (y1 == aux->getVal(8)) {
			if (m1 < aux->getVal(7)) FLAG_Aux_1 = true;
			if (m1 == aux->getVal(7)) {
				if ((d1 < aux->getVal(6)) || (d1 == aux->getVal(6))) FLAG_Aux_1 = true;
			}
		}
		if (y2 > aux->getVal(8)) FLAG_Aux_2 = true;
		if (y2 == aux->getVal(8)) {
			if (m2 > aux->getVal(7)) FLAG_Aux_2 = true;
			if (m2 == aux->getVal(7)) {
				if ((d2 > aux->getVal(6)) || (d2 == aux->getVal(6))) FLAG_Aux_2 = true;
			}
		}
		if (FLAG_Aux_1 && FLAG_Aux_2) {
			if (aux->getVal(3) == 2) {	//Event ON
				h_on = aux->getVal(4);
				m_on = aux->getVal(5);
				FLAG_Event_ON = true;
			}
			if (aux->getVal(3) == 1) {	//Event OFF
				h_off = aux->getVal(4);
				m_off = aux->getVal(5);
				FLAG_Event_OFF = true;
			}
			if (FLAG_Event_OFF) {
				total_time_on += ((h_off * 60 + m_off) - (h_on * 60 + m_on));
				FLAG_Event_OFF = false;
				FLAG_Event_ON = false;
			}
			FLAG_Aux_1 = false;  FLAG_Aux_2 = false;
		}
		int d = aux->getVal(6);
		aux = aux->getNext();
		//If the system was on in the day in question, but turned off only the day befor, counts only untill midnight
		if (FLAG_Event_ON && d < aux->getVal(6)) {
			total_time_on += (1440 - (h_on * 60 + m_on));
			FLAG_Event_ON = false;
		}
		FLAG_Aux_1 = false;  FLAG_Aux_2 = false;
	}
	cout << "Between " << d1 << "/" << m1 << "/" << y1 << " and " << d2 << "/" << m2 << "/" << y2 
		 << " the system has been ON for " << (total_time_on / 60) << "h e " << (total_time_on % 60) << "min" << endl;
	total_time_on = 0;
}

// listBusyPeriod show the total time that the system was ON by turn (Dawn, Morning, Afternoon and Night)
void List::listBusyPeriod(int d, int m, int y) {
	Node* aux = head;
	int dawn = 0, morning = 0, evening = 0, night = 0;
	int h_on = 0, h_off = 0, m_on = 1, m_off = 0;
	bool FLAG_Event_OFF = false, FLAG_Event_ON = false;

	//It will be very difficult to understand this bottom part :P
	//But the important thing is that it works XD
	//Depending on the date entry, the value is down by 1 minut, because im not getting the seconds, and I didn't spend a lot
	//of time in this specific section
	int cont_while = 0;
	while (aux != 0) {
		if (y == aux->getVal(8)) {
			if (m == aux->getVal(7)) {
				if (d == aux->getVal(6)) {
					if (aux->getVal(3) == 2) {	//Event ON
						h_on = aux->getVal(4);
						m_on = aux->getVal(5);
						FLAG_Event_ON = true;
					}
					if (aux->getVal(3) == 1) {	//Event OFF
						h_off = aux->getVal(4);
						m_off = aux->getVal(5);
						FLAG_Event_OFF = true;
					}
					if (FLAG_Event_OFF) {
						if ((h_on * 60 + m_on) <= 360 && (h_on * 60 + m_on) > 0) { //Madrugada
							if ((h_off * 60 + m_off) <= 360) dawn += ((h_off * 60 + m_off) - (h_on * 60 + m_on));
							else {
								dawn += (360 - ((h_on*60) + m_on));
								if ((h_off * 60 + m_off) > 1080) {	//Caso 1
									morning += 360;
									evening += 360;
									night += ((h_off * 60 + m_off) - 1080);
								}
								if ((h_off * 60 + m_off) <= 1080 && (h_off * 60 + m_off) > 720) {	//Caso 2
									morning += 360;
									evening += ((h_off * 60 + m_off) - 720);
								}
								if ((h_off * 60 + m_off) <= 720 && (h_off * 60 + m_off) > 360) {	//Caso 3
									morning += ((h_off * 60 + m_off) - 360);
								}
							}
						}
						if ((h_on * 60 + m_on) <= 720 && (h_on * 60 + m_on) > 360) {	//Manha
							if ((h_off * 60 + m_off) <= 720) morning += ((h_off * 60 + m_off) - (h_on * 60 + m_on));
							else {
								morning += (720 - (h_on + m_on));
								if ((h_off * 60 + m_off) > 1080) {	//Caso 4
									evening += 360;
									night += ((h_off * 60 + m_off) - 1080);
								}
								if ((h_off * 60 + m_off) <= 1080 && (h_off * 60 + m_off) > 720) {	//Caso 5
									evening += ((h_off * 60 + m_off) - 720);
								}
							}
						}
						if ((h_on * 60 + m_on) <= 1080 && (h_on * 60 + m_on) > 720) {	//Tarde
							if ((h_off * 60 + m_off) <= 1080) evening += ((h_off * 60 + m_off) - (h_on * 60 + m_on));
							else {
								evening += (1080 - ((h_on*60) + m_on)); //Caso 6
								night += ((h_off * 60 + m_off) - 1080);
							}
						}
						if ((h_on * 60 + m_on) <= 1440 && (h_on * 60 + m_on) > 1080) {	//Noite
							if ((h_off * 60 + m_off) <= 1440) night += ((h_off * 60 + m_off) - (h_on * 60 + m_on));
						}
						h_off = 24; m_off = 0;
						FLAG_Event_ON = false;
						FLAG_Event_OFF = false;
					}
				}
			}
		}
		aux = aux->getNext();
		//If the system was on in the day in question, but turned off only the day befor, counts only untill midnight
		if (FLAG_Event_ON && d < aux->getVal(6)) {
			night += ((h_off * 60 + m_off) - (h_on * 60 + m_on));
			FLAG_Event_ON = false;
		}
	}

	cout << "Time that the system has been ON by period" << endl;
	cout << "Dawn: " << (dawn / 60) << "h " << (dawn % 60) << "min" << endl;
	cout << "Morning: " << (morning / 60) << "h " << (morning % 60) << "min" << endl;
	cout << "Evening: " << (evening / 60) << "h " << (evening % 60) << "min" << endl;
	cout << "Night: " << (night / 60) << "h " << (night % 60) << "min" << endl;
	dawn = 0; morning = 0; evening = 0; night = 0;
}

