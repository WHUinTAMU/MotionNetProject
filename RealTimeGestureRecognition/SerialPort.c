#include "SerialPort.h"

HANDLE openPort(TCHAR *gszPort) {
    HANDLE hComm = CreateFile(gszPort,                                  // pointer to name of the file
                              GENERIC_READ | GENERIC_WRITE,					// access (read-write) mode
                              0,                                               // share mode
                              0,                                               // pointer to security attributes
                              OPEN_EXISTING,									// how to create
                              0,                                               // file attributes
                              0);												// handle to file with attributes to copy
    return hComm;
}

bool setupPort(HANDLE hComm) {
    DCB dcb;
    if (!GetCommState(hComm, &dcb)) {
        printf("getDCB failed\n");
        return 0;
    }

    dcb.BaudRate = 9600;   // changed from 115200, now is 9600 to collect beacons' data
    dcb.fParity = FALSE;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.ByteSize = 8;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;

    if (!SetCommState(hComm, &dcb)) {
        printf("failed to set port state (%ld)\n", GetLastError());
        return 0;
    } else {
        return 1;
    }
}

/*
	Discards all characters from the output or input buffer of a specified communications resource.
	It can also terminate pending read or write operations on the resource.
*/
bool purgePort(HANDLE hComm) {
    if (PurgeComm(hComm, PURGE_RXCLEAR)) {
        return 1;
    } else {
        printf("Port purge failed\n");
        return 0;
    }
}

bool closePort(HANDLE hComm) {
    if (CloseHandle(hComm)) {
        printf("Port closed\n");
        return 1;
    } else {
        printf("Port close failed\n");
        return 0;
    }
}

bool sendSByte(unsigned char byteToWrite, HANDLE hComm) {
    DWORD dwWritten;
    if (WriteFile(hComm, &byteToWrite, sizeof(byteToWrite), &dwWritten, 0)) {
        printf("wrote byte %Xh (%c) to serial port\n", byteToWrite, byteToWrite);
        return 1;
    } else {
        printf("serial port write failed\n");
        return 0;
    }
}

unsigned char readSByte(HANDLE hComm) {
    DWORD dwRead;
    unsigned char lpBuf;

    ReadFile(hComm,                          // handle of file to read
             &lpBuf,                         // address of buffer that receives data
             sizeof(lpBuf),                  // number of bytes to read
             &dwRead,                        // address of number of bytes read
             0);                             // address of structure for data
    //printf("Read byte %Xh from serial port\n",lpBuf);
    return lpBuf;
}
