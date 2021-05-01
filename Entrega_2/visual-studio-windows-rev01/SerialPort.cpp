#include "SerialPort.h"

/*Constructors*/
SerialPort::SerialPort()
    : isConnect(false) {}

SerialPort::SerialPort(const char* port, unsigned long BaudRate)
    : isConnect(false)
{
    Initialize(port, BaudRate);
}

/*Initialization*/
void SerialPort::Initialize(const char* port, unsigned long BaudRate)   //GIORDANO const char* to WCHAR
{   
    //project -> properties -> Configuration Properties -> Advanced -> Character Set (From "Use Unicode Character Set" to "Use Multi-Byte Character Set")
    handler = CreateFile(port, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handler == INVALID_HANDLE_VALUE)
    {
        std::cout << "ERROR!::Error during opening port" << port << std::endl;
        return;
    }

    DCB serialParameters;
    if (!GetCommState(handler, &serialParameters)) /*Get com parameters*/
    {
        std::cout << "ERROR!::failed to get current serial parameters" << std::endl;
        return;
    }

    serialParameters.DCBlength = sizeof(DCB);
    serialParameters.BaudRate = BaudRate;
    serialParameters.ByteSize = 8; /*8 bit data format*/
    serialParameters.StopBits = ONESTOPBIT;
    serialParameters.Parity = PARITY_NONE;

    if (!SetCommState(handler, &serialParameters)) /*Send modified com parameters*/
    {
        std::cout << "ALERT!:Failed to set THE Serial port parameters" << std::endl;
        return;
    }

    isConnect = true;
    PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

/*Serial I/O*/
void SerialPort::receive(unsigned char& data, unsigned int byteSize)
{
    ReadFile(handler, &data, byteSize, NULL, NULL);
}

void SerialPort::transmit(unsigned char* data, unsigned int byteSize)
{
    WriteFile(handler, data, byteSize, NULL, NULL);
}

/*State*/
void SerialPort::connect()
{
    isConnect = true;
}

void SerialPort::disconnect()
{
    isConnect = false;
}
bool SerialPort::isConnected()
{
    return isConnect;
}

/*Destructors*/
SerialPort::~SerialPort()
{
    if (isConnect)
    {
        isConnect = false;
        CloseHandle(handler);
    }
}

void SerialPort::purge() {
    PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
}
