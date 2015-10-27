#ifndef LAMPCMD_H
#define LAMPCMD_H
#include<stdio.h>
#include <winsock2.h>					//Socket communication



//const char* httpGetRequest_fmt = "GET /api/%s HTTP/1.1\r\nHost: %s\r\n";
//const char* httpPostRequest_fmt = "POST %s HTTP/1.1\r\nHost: %s\r\n%s\r\n";
//const char* httpPutRequest_fmt = "PUT /api/%s HTTP/1.1\r\nHost: %s\r\nContent-Length: 40\r\nContent-Type: text/plain;charset=UTF-8\r\n\r\n%s\r\n";
//const char* httpDeleteRequest_fmt = "DELETE /%s?%s HTTP/1.1\r\n\r\n";

char* createRequestStr(char* method, char* _url, char* headers);

char* sendTcpRequest(char* request, int port, char* addr);

int setLightState(int lampId, char* newState);

int getLightState(int lampId);

int getUserName(char* deviceType);

int getBridgeIP();

void sendCmd(char* method, char* object, int lampId, char* attr, char* msgBody, int port, char* ip);
#endif
