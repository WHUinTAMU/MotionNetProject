#ifndef PKTPARSER_H
#define PKTPARSER_H

#include <Windows.h>	//Contain definitions for SYSTEMTIME and GetSystemTime

#include "SerialPort.h"
#include "DataNode.h"

// Packet recognition flags
#define DLE	0x10
#define SOH 0x01
#define EOT	0x04

unsigned int getLocalTime();

int toMs(unsigned char, unsigned char, unsigned int numOverflows);

int uCharToInt(unsigned char, unsigned char);

PktData blockingReadOnePacket(HANDLE hComm);

#endif	//PKTPRSER_H
