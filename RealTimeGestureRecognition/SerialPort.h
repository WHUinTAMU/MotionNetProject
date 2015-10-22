#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>	//Contain definations for handle, DWORD and DCB
#include <tchar.h>		//Contain definations for _T

HANDLE openPort(TCHAR *gszPort);

bool setupPort(HANDLE hComm);

bool purgePort(HANDLE hComm);

bool closePort(HANDLE hComm);

bool sendSByte(unsigned char byteToWrite, HANDLE hComm);

unsigned char readSByte(HANDLE hComm);

#endif  //SERIALPORT_H
