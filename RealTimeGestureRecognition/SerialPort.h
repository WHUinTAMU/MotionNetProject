#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>	//Contain definations for handle, DWORD and DCB
#include <tchar.h>		//Contain definations for _T

//int portId;				//e.g Com#3 's portId is 3.
//TCHAR commPort[11];		//e.g "\\\\.\\com3"

/*
    Definitions for the methods about serial port operations.
*/
HANDLE openPort(TCHAR *gszPort);

bool setupPort(HANDLE hComm);

bool purgePort(HANDLE hComm);

bool closePort(HANDLE hComm);

bool sendSByte(unsigned char byteToWrite, HANDLE hComm);

unsigned char readSByte(HANDLE hComm);

#endif  //SERIALPORT_H
