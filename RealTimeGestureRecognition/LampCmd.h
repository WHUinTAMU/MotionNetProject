#ifndef LAMPCMD_H
#define LAMPCMD_H
#include<stdio.h>
#include <winsock2.h>					//Socket communication

char* createRequestStr(char* method, char* _url, char* headers);

char* sendTcpRequest(char* request, int port, char* addr);

int setLightState(int lampId, char* newState);

int getLightState(int lampId);

int getUserName(char* deviceType);

int getBridgeIP();

void sendCmd(char* method, char* object, int lampId, char* attr, char* msgBody, int port, char* ip);
#endif
