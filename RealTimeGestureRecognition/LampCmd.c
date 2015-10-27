#include "LampCmd.h"
#pragma comment(lib,"ws2_32.lib") 		//Winsock Library

#define MULTI_CAST_SERVER "239.255.255.250" //Multicast ip address
#define MULTI_CAST_PORT 1900   				//The port listens for incoming multicast data
#define RECV_BUFLEN 1024  					//Max length of buffer
#define EMPTY_STR ""						//Empty str returned when error occurs
#define REQ_COUNT  5						//Request totally 10 times to get response
#define WAIT_PUSH_BTN_TIME	6 				//Wait totally 60s to detect user's push bridge link button action

char LAMP_BRIDGE_IP[16];	//Lamp bridge IP, at most xxx.xxx.xxx.xxx\0
char USER_NAME[40];		//USER_NAME is used to control lamps

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

char* subString (const char* input, int offset, int len, char* dest) {
	if(input == NULL || dest == NULL || offset < 0 || len < 0)
		return EMPTY_STR;

	int input_len = strlen (input);
	if (offset + len > input_len) {
		return EMPTY_STR;
	}

	strncpy (dest, input + offset, len);
	return dest;
}

//copy specific string into target between headFlag and tailFlag in response string
char* splitStr(char * response, const char* headFlag, const char* tailFlag, char* target) {
	if(response == NULL || headFlag == NULL || tailFlag == NULL || target == NULL)
		return EMPTY_STR;

	int headFlagLen = strlen(headFlag);

	const char* headFlagPtr = strstr(response, headFlag);
	const char* tailFlagPtr = strstr(response, tailFlag);

	if(headFlagPtr == NULL || tailFlagPtr == NULL)
		return EMPTY_STR;

	int subStrLen = tailFlagPtr - headFlagPtr - headFlagLen;
	if(subStrLen <= 0)
		return EMPTY_STR;
	else
		return subString (headFlagPtr, headFlagLen , subStrLen, target);
}

//remember free url
char* createURL(char* _object, int lampId, char* _attr) {
	if(USER_NAME == NULL || strlen(USER_NAME) <= 0) {
		printf("initialize username first!\n");
		return EMPTY_STR;
	}

	char* object = ( _object == NULL ) ? EMPTY_STR : _object;
	char* attr = ( _attr == NULL ) ? EMPTY_STR : _attr;

	int urlLen = strlen(url_fmt) + strlen(USER_NAME) + strlen(object) + strlen(attr) ;
	char* url =(char*) malloc(sizeof(char) * urlLen);
	memset(url,'\0', urlLen);

	if(lampId > 0)
		sprintf(url, url_fmt, USER_NAME, object, lampId, attr);
	else
		sprintf(url, "%s%s%s", USER_NAME, object, attr);
	return url;
}

//remember free request
char* createRequestStr(char* method, char* _url, char* headers) {
	if(method == NULL) {
		printf("HTTP request Method field cannot be NULL!\n");
		return EMPTY_STR;
	}

	if(LAMP_BRIDGE_IP == NULL || strlen(LAMP_BRIDGE_IP) <= 0) {
		printf("initialize bridge ip first!\n");
		return EMPTY_STR;
	}

	char* url = ( _url == NULL ) ? EMPTY_STR : _url;

	int headerLen = (headers == NULL ) ? 0 : strlen(headers);
	int reqLen = strlen(httpRequest_fmt) + strlen(method) + strlen(url) + strlen(LAMP_BRIDGE_IP) + headerLen  ;

	char* req ;

	if(headerLen > 0) {
		int descripLen = headerLen + strlen(httpHeaders_fmt);
		char* description = (char* )malloc(descripLen * sizeof(char));
		memset(description, '\0', descripLen);

        reqLen += descripLen;
		req =(char*) malloc(sizeof(char) * reqLen);
		memset(req,'\0', reqLen);

		sprintf(description, httpHeaders_fmt, strlen(headers), headers);
		sprintf(req, httpRequest_fmt, method, url, LAMP_BRIDGE_IP, description);

		free(description);
	} else{
	    req =(char*) malloc(sizeof(char) * reqLen);
		memset(req,'\0', reqLen);
	    sprintf(req, httpRequest_fmt, method, url, LAMP_BRIDGE_IP, EMPTY_STR);
	}

	printf("===========================  Request  ==========================\n");
	//puts(req);
	return req;
}

//remember free response
char* sendTcpRequest(char* request, int port, char* addr) {
	if(request == NULL)
		return EMPTY_STR;

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		return EMPTY_STR;
	}

	int sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sclient == INVALID_SOCKET) {
		printf("invalid socket !");
		return EMPTY_STR;
	}

	struct sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	serAddr.sin_addr.S_un.S_addr = inet_addr(addr);

	if (connect(sclient, (struct sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		printf("connect error !");
		closesocket(sclient);
		return EMPTY_STR;
	}

	char recData[RECV_BUFLEN];
	int ret = 0;
	int i = 0;
	while(i < REQ_COUNT) {
		send(sclient, request, strlen(request), 0);
		ret = recv(sclient, recData, RECV_BUFLEN, 0);
		if(ret <= 0) {
			i ++;
			memset(recData,'\0', RECV_BUFLEN);
			continue;
		}

		recData[ret] = 0x00;
		printf("===========================  Response  ==========================\n");
		//puts(recData);

		if(strstr(recData, "200 OK") != NULL) {
			//receive more info from bridge
			memset(recData,'\0', RECV_BUFLEN);
			ret = recv(sclient, recData, RECV_BUFLEN, 0);
			recData[ret] = 0x00;
			if(ret > 0) {
				printf("===========================  Response  ==========================\n");
				//puts(recData);
			}
			break;
		}
	}
	closesocket(sclient);
	WSACleanup();

	if(i >= REQ_COUNT) {
		printf("after %d times request, server still don't respond!\n", REQ_COUNT);
		return EMPTY_STR;
	} else {
		char* response = (char*) malloc(sizeof(char) * (strlen(recData) + 1));
		strcpy(response, recData);
		return response;
	}
}

/*
Address	http://<bridge ip address>/api/<username>/lights/<lampId>/state
Body	<json>
Method	PUT
*/
int setLightState(int lampId, char* newState) {
	if(newState == NULL)
		return 0;
	if(strlen(newState) <= 0) {
		printf("need new state json description! \n");
		return 0;
	}

	char* url = createURL("/lights/", lampId, "/state");
	char* request = createRequestStr("PUT", url, newState);
	free(url);

	char* response = sendTcpRequest(request, 80, LAMP_BRIDGE_IP);

	free(request);
	//can also analyse response here
	free(response);
	return 1;
}

/*
Address	http://<bridge ip address>/api/<username>/lights/<lampId>
Body
Method	GET
*/
int getLightState(int lampId) {
	char* url = createURL("/lights/", lampId, "");
	char* request = createRequestStr("GET", url, NULL);
	free(url);

	char* response = sendTcpRequest(request, 80, LAMP_BRIDGE_IP);
	free(request);
	free(response);
	return 1;
}


int getUserName(char* deviceType) {
	char* request = createRequestStr("POST", NULL, deviceType);
	if(strcmp(request, EMPTY_STR) == 0)
		return 0;

	int countTime = 0;
	char* response = sendTcpRequest(request, 80, LAMP_BRIDGE_IP);

	while((strstr(response, USER_NAME_HEAD_FLAG) == NULL) || (strstr(response, PRESS_BTN_ERROR) != NULL) || (countTime >= WAIT_PUSH_BTN_TIME)) {
		free(response);
		printf("\nplease push the button on the lamp bridge in the next 10s!\n");
		Sleep(3000);
		countTime ++;
		response = sendTcpRequest(request, 80, LAMP_BRIDGE_IP);
	}

	splitStr(response, USER_NAME_HEAD_FLAG, USER_NAME_TAIL_FLAG, USER_NAME);
	free(request);
	free(response);

	if((countTime >= WAIT_PUSH_BTN_TIME) && (strcmp(USER_NAME, EMPTY_STR) == 0)) {
		printf("wait for push bridge link button timeout ......\n");
		return 0;
	} else {
		printf("get username succeed, username now is :%s\n", USER_NAME);
		return 1;
	}
}

int getBridgeIP() {
	struct sockaddr_in si_other;
	int s; 							// client udp socket
	int slen = sizeof(si_other);
	char response[RECV_BUFLEN];
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 0;
	}
	printf("Initialised.\n");

	//create socket
	if ( (s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
		printf("socket() failed with error code : %d" , WSAGetLastError());
		return 0;
	}

	//set socket as a broadcast type
	const int opt = 1;
	if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt)) == -1) {
		printf("set socket broadcast failed with error code : %d\n" , WSAGetLastError());
		return 0;
	}

	//setup address structure
	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(MULTI_CAST_PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(MULTI_CAST_SERVER); //htonl(INADDR_BROADCAST);

	//the broadcast request, ie "Any upnp devices out there?"
	char *request = "M-SEARCH * HTTP/1.1\r\nHOST:239.255.255.250:1900\r\nMAN:\"ssdp:discover\"\r\nST:ssdp:all\r\nMX:5\r\n\r\n";

	printf("Search Lamp Bridge Request: \n%s", request);

	while(1) {
		if (sendto(s, request, strlen(request) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR) {
			printf("sendto() failed with error code : %d" , WSAGetLastError());
			return 0;
		}

		//clear the buffer by filling null, it might have previously received data
		memset(response,'\0', RECV_BUFLEN);

		//try to receive some data, this is a blocking call
		if (recvfrom(s, response, RECV_BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR) {
			printf("recvfrom() failed with error code : %d" , WSAGetLastError());
			return 0;
		}

		//parse the response, get bridge IP
		if(strstr(response, BRIDGE_TAG) != NULL) {
			printf("===========================  Response  ==========================\n");
			puts(response);

			splitStr(response, IP_HEAD_FLAG, IP_TAIL_FLAG, LAMP_BRIDGE_IP);
			closesocket(s);
			WSACleanup();
			break;
		}
	}
	return 1;
}


// watch the port and recv alive msg every 5 s
void watchBidgeAlive() {

}

//remember free response
void sendCmd(char* method, char* object, int lampId, char* attr, char* msgBody, int port, char* ip) {
	char* url = createURL(object, lampId, attr);
	char* request = createRequestStr(method, url, msgBody);
	char* response = sendTcpRequest(request, port, ip);
	free(url);
	free(request);
	free(response);
}
