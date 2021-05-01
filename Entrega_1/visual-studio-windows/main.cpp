#include "List.h"
#include <iostream>
#include "SerialPort.h"

#define BAUD_RATE 9600
#define SERIAL_PORT "com7"

using namespace std;

List* l;

void getDataBase(void);

int main() {

	SerialPort port(SERIAL_PORT, BAUD_RATE);

	List lista1, lista2, lista3, lista4, lista5;
	l = &lista1;

	getDataBase();

	int menu_option = 0, target_id = 0;

	//1 - Listar todos os eventos ocorridos em um determinado intervalo de datas
	//2 - Informar o tempo total em horas e minutos que o sistema manteve o aquecedor alimentado em um intervalo de datas
	//NOPS //3 - Listar o período do dia com mais utilização do sistema de controle
	while (1) {
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		cout << "What do you want to do?" << endl
			<< "1 - List the events that occurred between two dates" << endl
			<< "2 - Get the total time that the Air Conditionung was energized between two dates" << endl
			//<< "3 - List the period of the day with more system usage" << endl
			<< "3 - List all available data" << endl
			<< "4 - Receive data with Serial Communication" << endl;
		cout << "Input: ";
		cin >> menu_option;
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;

		//Get the Id and make the pointer point to the correct list
		if (menu_option != 4) {
			cout << "Insert the target Embbedded System Id: ";
			cin >> target_id;
			if (!((target_id > 0) || (target_id < 256)) ) {
				cout << "Invalid Input. Using Id = 2" << endl;
				l = &lista2;
			}
			else {
				if (target_id == 254) l = &lista1; //So vou utulizar este
				if (target_id == 2) l = &lista2;
				if (target_id == 3) l = &lista3;
				if (target_id == 4) l = &lista4;
				if (target_id == 5) l = &lista5;
				target_id = 0;
			}	
		}

		//1 - Listar todos os eventos ocorridos em um determinado intervalo de datas
		if (menu_option == 1) {
			int dd1 = 1, mm1 = 1, yy1 = 1, dd2 = 2, mm2 = 2, yy2 = 2;
			bool FLAG_Date_OK = true;
			try{
				cout << "List the date 1 with this format - xx/xx/xx: ";
				scanf_s("%d/%d/%d", &dd1, &mm1, &yy1);
				cout << "List the date 1 with this format - xx/xx/xx: ";
				scanf_s("%d/%d/%d", &dd2, &mm2, &yy2);
				if (dd1 > 31 || dd1 < 1 || dd2 > 31 || dd2 < 1) throw string("Invalid Day");
				if (mm1 > 12 || mm1 < 1 || mm2 > 12 || mm2 < 1) throw string("Invalid Month");
				if (yy1 > 50 || yy2 > 50) throw string("Invalid Year");
				l->listSomeEvents(dd1, mm1, yy1, dd2, mm2, yy2);
			}
			catch(string ex){
				cout << "Error: " << ex << endl;
			}
		}

		//2 - Informar o tempo total em horas e minutos que o sistema manteve o ar-condicionado alimentado em um intervalo de datas
		if (menu_option == 2) {
			int dd1 = 1, mm1 = 1, yy1 = 1, dd2 = 2, mm2 = 2, yy2 = 2;
			bool FLAG_Date_OK = true;

			try {
				cout << "List the date 1 with this format - xx/xx/xx: ";
				scanf_s("%d/%d/%d", &dd1, &mm1, &yy1);
				cout << "List the date 2 with this format - xx/xx/xx: ";
				scanf_s("%d/%d/%d", &dd2, &mm2, &yy2);
				if (dd1 > 31 || dd1 < 1 || dd2 > 31 || dd2 < 1) throw string("Invalid Day");
				if (mm1 > 12 || mm1 < 1 || mm2 > 12 || mm2 < 1) throw string("Invalid Month");
				if (yy1 > 50 || yy2 > 50) throw string("Invalid Year");
				l->listTimeON(dd1, mm1, yy1, dd2, mm2, yy2);
			}
			catch (string ex) {
				cout << "Error: " << ex << endl;
			}
		}

		/* Não
		//3 - Listar o período do dia com mais utilização do sistema de controle
		if (menu_option == 3){
			int dd = 1, mm = 1, yy = 1;
			cout << "List the date 1 with this format - xx/xx/xx: ";
			scanf_s("%d/%d/%d", &dd, &mm, &yy);
			l->listBusyPeriod(dd, mm, yy);
		}
		*/

		//4 - Listar todos os eventos
		if (menu_option == 3) l->listAll();

		//5 - Receber dados via comunicação Serial
		if (menu_option == 4) {
			cout << "Waiting slaves send the data... " << endl;
			unsigned char dataR, dataT = 'A';
			int aux_read = 1, aux_read_2 = 0;
			unsigned char dataR_id = 0, dataR_cnt = 0, dataR_evnt = 0, dataR_h = 0, dataR_m = 0, dataR_d = 0, dataR_me = 0, dataR_y = 0;

			//Get the size of the list that will be received
			port.receive(dataR, 1);
			int list_size = dataR;
			cout << "List Size: " << list_size << endl;

			//Get the Id of the Embedded System that are sending the data
			port.receive(dataR_id, 1);
			if (dataR_id == 254) l = &lista1;
			if (dataR_id == 2) l = &lista2;
			if (dataR_id == 3) l = &lista3;
			if (dataR_id == 4) l = &lista4;
			if (dataR_id == 5) l = &lista5;
			cout << "Id: " << (int)dataR_id << endl;

			dataR = 0;

			//Get the header
			port.receive(dataR, 1);
			int cab = dataR;
			cout << "Cabecalho: " << cab << endl;

			if (cab == 0xAA) {
				if (list_size > 0) {
					for (int i = 1; i <= list_size; i++) {
						port.receive(dataR_cnt, 1);
						//port.receive(dataR_2, 1);
						port.receive(dataR_evnt, 1);
						port.receive(dataR_h, 1);
						port.receive(dataR_m, 1);
						port.receive(dataR_d, 1);
						port.receive(dataR_me, 1);
						port.receive(dataR_y, 1);
						//l = &lista1;
						l->insertAfterLast((int)dataR_cnt, (int)dataR_id, (int)dataR_evnt, (int)dataR_h, (int)dataR_m, (int)dataR_d, (int)dataR_me, (int)dataR_y);
					}
					//Get the footer
					port.receive(dataR, 1);
					int rod = dataR;
					cout << "Rodape: " << rod << endl;
					if (rod != 0x55) cout << "Rodape errado: Dados corrompidos" << endl;
				}
				else {
					cout << "Lista Vazia" << endl;
				}
				cout << "New line" << endl;
			}
			else {
				cout << "Cabecalho errado: Recebimento cancelado" << endl;
			}
		}

	
	}
	return 0;
}


void getDataBase() {
	//1 - OFF, 2 - 0N, 3 - RST
	l->insertAfterLast(1, 254, 2, 3, 45, 31, 3, 21);  
	l->insertAfterLast(2, 254, 1, 8, 15, 31, 3, 21);  
	l->insertAfterLast(3, 254, 2, 12, 10, 31, 3, 21);  
	l->insertAfterLast(4, 254, 1, 16, 32, 31, 3, 21);  

	l->insertAfterLast(5, 254, 2, 23, 55, 31, 3, 21);
	l->insertAfterLast(6, 254, 3, 10, 12, 1, 4, 21); 
	l->insertAfterLast(7, 254, 3, 10, 20, 1, 4, 21); 
	l->insertAfterLast(8, 254, 3, 10, 30, 1, 4, 21); 
	l->insertAfterLast(9, 254, 1, 10, 45, 1, 4, 21); 
	l->insertAfterLast(10, 254, 2, 11, 30, 1, 4, 21); 
	l->insertAfterLast(11, 254, 1, 11, 59, 1, 4, 21); 
	l->insertAfterLast(12, 254, 2, 8, 17, 2, 4, 21);  
	l->insertAfterLast(13, 254, 3, 8, 30, 2, 4, 21);  
	l->insertAfterLast(14, 254, 3, 8, 35, 2, 4, 21);  
	l->insertAfterLast(15, 254, 3, 8, 43, 2, 4, 21);  
	l->insertAfterLast(16, 254, 1, 8, 43, 3, 4, 21);  

	l->insertAfterLast(17, 254, 2, 3, 45, 7, 4, 21);  
	l->insertAfterLast(18, 254, 1, 8, 15, 7, 4, 21);  
	l->insertAfterLast(19, 254, 2, 9, 45, 7, 4, 21);  
	l->insertAfterLast(20, 254, 1, 10, 00, 7, 4, 21);  
	l->insertAfterLast(21, 254, 2, 16, 15, 7, 4, 21); 
	l->insertAfterLast(22, 254, 1, 20, 30, 7, 4, 21);  

	l->insertAfterLast(23, 254, 2, 23, 0, 7, 4, 21);  
	l->insertAfterLast(24, 254, 1, 1, 0, 8, 4, 21);  

}



