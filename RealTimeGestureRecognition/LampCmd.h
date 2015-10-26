#include<stdio.h>
#include <winsock2.h>					//Socket communication

#pragma comment(lib,"ws2_32.lib") 		//Winsock Library

#define MULTI_CAST_SERVER "239.255.255.250" //Multicast ip address
#define MULTI_CAST_PORT 1900   				//The port listens for incoming multicast data
#define RECV_BUFLEN 1024  					//Max length of buffer
#define EMPTY_STR ""						//Empty str returned when error occurs
#define REQ_COUNT  10						//Request totally 10 times to get response
#define WAIT_PUSH_BTN_TIME	6 				//Wait totally 60s to detect user's push bridge link button action

char LAMP_BRIDGE_IP[16] = "";	//Lamp bridge IP, at most xxx.xxx.xxx.xxx\0
char USER_NAME[40] = "";		//USER_NAME is used to control lamps

//flag strings
const char* BRIDGE_TAG = "IpBridge";
const char* IP_HEAD_FLAG = "LOCATION: http://";
const char* IP_TAIL_FLAG = ":80";
const char * USER_NAME_HEAD_FLAG = "username\":\"";
const char * USER_NAME_TAIL_FLAG = "\"}}";
const char* PRESS_BTN_ERROR = "link button not pressed";

/******************************  HTTP POST/PUT request fmt
<Method> /api/<url> HTTP/1.1
Host: xxx.xxx.xxx.xxx
Content-Length: <length>		//Notice: GET don't need Content fields and json strings
Content-Type: text/plain;charset=UTF-8

<Json cmd string>
*/

//URL fmt: <username>/<object>/<Id>/<attribute>
const char* url_fmt = "%s%s%d%s";
const char* httpRequest_fmt = "%s /api/%s HTTP/1.1\r\nHost: %s\r\n%s";
const char* httpHeaders_fmt = "Content-Length: %d\r\nContent-Type: text/plain;charset=UTF-8\r\n\r\n%s";

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
