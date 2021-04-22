#pragma once

#include <windows.h>
#include <iostream>

class SerialPort {
public:
    //Constructors
    SerialPort();
    SerialPort(const char* port, unsigned long BaudRate);

    //Initialization
    void Initialize(const char* port, unsigned long BaudRate);

    //Serial I/O
    void receive(unsigned char& data, unsigned int byteSize);
    void transmit(unsigned char* data, unsigned int byteSize);

    //State
    void connect();
    void disconnect();
    bool isConnected();

    void purge();

    //Destructor
    ~SerialPort();

private:
    HANDLE handler;
    bool isConnect;


};

