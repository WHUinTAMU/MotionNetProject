#ifndef DATANODE_H
#define DATANODE_H

#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

typedef struct multiParam{
    char gszPort[20];
    //int caliDataNum;
    int magDataNum;
}Params;

typedef struct packetData {
    double accX;
    double accY;
    double accZ;
    double gyroX;
    double gyroY;
    double gyroZ;
    double magX;
    double magY;
    double magZ;
    unsigned long timeStamp;
    long pktNumber;
} PktData;

///////////////////////////////////// Data Queue ///////////////////////////////////////
#define MAX_SIZE 1001
//circle queue whose length is ( MAX_SIZE - 1 ), use one element space to distinguish empty and full.
typedef struct {
	double accXData[MAX_SIZE];
	double gyroXData[MAX_SIZE];
	double magXData[MAX_SIZE];

	double accYData[MAX_SIZE];
	double gyroYData[MAX_SIZE];
	double magYData[MAX_SIZE];

	double accZData[MAX_SIZE];
	double gyroZData[MAX_SIZE];
	double magZData[MAX_SIZE];

	int front, rear;
}SqQueue;

SqQueue* create_empty_queue();

bool is_empty_queue(SqQueue * queue);

bool is_full_queue(SqQueue * queue);

int add_to_queue(SqQueue * queue, PktData pktData);

bool delete_from_queue(SqQueue * queue);

void free_queue(SqQueue* queue);

void clear_queue(SqQueue* queue);

int  get_queue_length(SqQueue* queue);

bool compare_list_and_delete_queue(SqQueue* queue, int* startList, int length);

bool compare_two_position(SqQueue* queue, int a, int b);


///////////////////////////////////// DataNode List ///////////////////////////////////////
typedef struct dataNode {
    PktData packetData;
    struct dataNode *next;
} DataNode;

typedef struct dataHeadNode {
    int length;
    DataNode * head;
    DataNode * tail;
} DataHeadNode;

//Compare 2 packet data, if all their fields are the same, return true.
bool equals(PktData data1, PktData data2);

//Create an empty list with the headNode that stores the count of the nodes.
DataHeadNode* create_list_with_head();

//Create a new DataNode
DataNode* create_node(PktData packetData);

void add_to_list_end(DataHeadNode *pHead, PktData packetData);

void add_to_list_head(DataHeadNode *pHead, PktData packetData);

bool delete_from_list_end( DataHeadNode *pHead);

bool delete_from_list_head( DataHeadNode *pHead);

PktData get_element_from_head( DataHeadNode *pHead);

PktData get_element_from_end( DataHeadNode *pHead);

void print_pktData(PktData packetData);

void print_list( DataHeadNode *pHead);

void clear_list( DataHeadNode *pHead);

void free_list(DataHeadNode *pHead);

//Get 3D magnetic data array from list
void fillMagDataArray(DataHeadNode* pHead, double magDataX[], double magDataY[], double magDataZ[]);

#endif // DATANODE_H
