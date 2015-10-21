#include "DataNode.h"

SqQueue* create_empty_queue() {
	SqQueue* queue = (SqQueue* ) malloc(sizeof(SqQueue));
	if (NULL == queue) {
		printf("\n Queue creation failed \n");
		exit(-1);
	}
	queue->front = queue->rear = 0;
	return queue;
}

void free_queue(SqQueue* queue) {
	free(queue);
}

void clear_queue(SqQueue* queue) {
	queue->front = queue->rear = 0;
}

bool is_empty_queue(SqQueue * queue) {
	if(queue->front == queue->rear)
		return true;
	else
		return false;
}
bool is_full_queue(SqQueue * queue) {
	if(queue->front == (queue->rear + 1) % MAX_SIZE)
		return true;
	else
		return false;
}

int add_to_queue(SqQueue * queue, PktData pktData) {
	if(is_full_queue(queue)) {
		queue->front = (queue->front + 1) % MAX_SIZE;
	//	printf("full, override front\n");
	}

	queue->accXData[queue->rear] = pktData.accX;
	queue->gyroXData[queue->rear] = pktData.gyroX;
	queue->magXData[queue->rear] = pktData.magX;

	queue->accYData[queue->rear] = pktData.accY;
	queue->gyroYData[queue->rear] = pktData.gyroY;
	queue->magYData[queue->rear] = pktData.magY;

	queue->accZData[queue->rear] = pktData.accZ;
	queue->gyroZData[queue->rear] = pktData.gyroZ;
	queue->magZData[queue->rear] = pktData.magZ;

	queue->rear = (queue->rear + 1) % MAX_SIZE;
	return (queue->rear - 1 + MAX_SIZE ) % MAX_SIZE;
}

bool delete_from_queue(SqQueue * queue) {
	if(is_empty_queue(queue))
		return false;
	else {
		queue->front = (queue->front + 1) % MAX_SIZE;
		return true;
	}
}
int  get_queue_length(SqQueue* queue) {
	return (queue->rear - queue->front + MAX_SIZE) % MAX_SIZE;
}

bool compare_list_and_delete_queue(SqQueue* queue, int* startList, int length) {
    int i = 0;
    int min = 0;
    int num = -1;
    for(i = 0; i < length; i++)
    {
        int distance = startList[i] - queue->front;
        distance = distance >= 0 ? distance : (1000 - queue->front) + startList[i];
        if(distance < min)
        {
            min = distance;
            num = i;
        }
    }

    queue->front = i;
    return true;
}

bool compare_two_position(SqQueue* queue, int a, int b) {
    int distanceA = a - queue->front;
    distanceA = distanceA >= 0 ? distanceA : (1000 - queue->front) + a;

    int distanceB = b - queue->front;
    distanceB = distanceB >= 0 ? distanceB : (1000 - queue->front) + b;

    if(distanceA >= distanceB) {
        return true;
    }else{
        return false;
    }
}



bool equals(PktData data1, PktData data2) {
	if ((data1.accX == data2.accX) && (data1.accY == data2.accY) && (data1.accZ == data2.accZ)
	        && (data1.gyroX == data2.gyroX) && (data1.gyroY == data2.gyroY) && (data1.gyroZ == data2.gyroZ)
	        && (data1.magX == data2.magX) && (data1.magY == data2.magY) && (data1.magZ == data2.magZ))
		return true;
	else
		return false;
}

DataHeadNode* create_list_with_head() {
//	printf("\n creating a list with a head node that stores list length \n");
	//printf(" malloc memory\n");
	DataHeadNode * ptr = (DataHeadNode*)malloc(sizeof(DataHeadNode));
	if (NULL == ptr) {
		printf("\n List creation failed \n");
		exit(-1);
	}

	ptr->length = 0;
	ptr->head = NULL;
	ptr->tail = NULL;
	return ptr;
}


DataNode* create_node(PktData packetData) {
//	printf(" malloc memory\n");
	DataNode *ptr = (DataNode*)malloc(sizeof(DataNode));
	if (NULL == ptr) {
		printf("\n Node creation failed \n");
		exit(-1);
	}
	ptr->packetData = packetData;
	ptr->next = NULL;
	return ptr;
}

void add_to_list_end(DataHeadNode *pHead, PktData packetData) {
	DataNode * ptr = create_node(packetData);

	if (pHead->tail == NULL) {
		pHead->tail = ptr;
		pHead->head = ptr;
	} else {
		pHead->tail->next = ptr;
		pHead->tail = ptr;
	}
	printf("\n Adding node to end of list \n");
	pHead->length ++;
}

//Add one node to the head of the list
void add_to_list_head(DataHeadNode *pHead, PktData packetData) {
	DataNode * ptr = create_node(packetData);

	if (pHead->tail == NULL) {
		pHead->tail = ptr;
		pHead->head = ptr;
	} else {
		ptr->next = pHead->head;
		pHead->head = ptr;
	}
//	printf("\n Adding node to head of list \n");
	pHead->length ++;
}

//Delete from the end of the list
bool delete_from_list_end( DataHeadNode *pHead) {
	DataNode *pNext = pHead->head;
	if (pHead->tail == NULL) {
		printf("list is already empty!\n");
		return false;
	}
	while(pNext->next != pHead->tail) {
		pNext = pNext->next;
	}

	pNext->next = NULL;
//	printf(" free memory\n");
	free(pHead->tail);

	if(pNext == pHead->head) {
		pHead->head = NULL;
		pHead->tail = NULL;
	} else {
		pHead->tail = pNext;
	}
	printf("delete last node\n");
	pHead->length --;
	return true;
}


bool delete_from_list_head(DataHeadNode *pHead) {
	DataNode *pNext;
	if (pHead->head == NULL) {
		printf("list is already empty\n");
		return false;
	}
	pNext = pHead->head;
	pHead->head = pNext->next;

	printf(" free memory\n");
	free(pNext);
	if(pHead->head == NULL) {
		pHead->tail = NULL;
	}

	pHead->length --;
	printf("delete first node\n");
	return true;
}

void print_list(DataHeadNode *pHead) {
	DataNode *ptr = pHead->head;

	printf("\n -------Printing list Start------- \n");
	printf("List length: %d\n", pHead->length);
	while (ptr != NULL) {
		print_pktData(ptr->packetData);
		ptr = ptr->next;
	}
	printf("\n -------Printing list End------- \n");
}

void print_pktData(PktData packetData) {
	printf("%5f, %5f, %5f, %5f, %5f, %5f, %4f, %4f, %4f, %ld, %ld\n",
	       packetData.accX, packetData.accY, packetData.accZ,
	       packetData.gyroX, packetData.gyroY, packetData.gyroZ,
	       packetData.magX, packetData.magY, packetData.magZ,
	       packetData.pktNumber,
	       packetData.timeStamp);
}

PktData get_element_from_head(DataHeadNode *pHead) {
	DataNode * p = pHead->head;
	PktData pktData = {0.0};
	if (p == NULL) {
		printf("empty list\n");
		return pktData;
	}
	return p->packetData;
}

PktData get_element_from_end( DataHeadNode *pHead) {
	DataNode * p = pHead->tail;
	PktData pktData = {0.0};
	if (p == NULL) {
		printf("empty list\n");
		return pktData;
	}
	return p->packetData;
}

void clear_list(DataHeadNode *pHead) {
	printf("start clearList \n");
	if(pHead == NULL){
        printf("head node is NULL\n");
        return;
	}

	DataNode *pNext = pHead->head;
	DataNode *ptr = NULL;

	if (pNext == NULL) {
		printf("list is already empty\n");
		return;
	}

	while (pNext != NULL) {
		ptr = pNext->next;
		free(pNext);
		pNext = ptr;
		pHead->length --;
	}

	if(pHead->length == 0)
		printf("now list is empty\n");
	else {
		printf("pay attention, not clear completely!\n");
	}
	pHead->head = NULL;
	pHead->tail = NULL;
	pHead->length = 0;
	printf("clearList done\n");
}

void free_list(DataHeadNode *pHead) {
    clear_list(pHead);
    if(pHead != NULL)
        free(pHead);
    pHead = NULL;
    printf("free list done\n");
}

//the headNode stores the length of the list, get array of magdata
void fillMagDataArray(DataHeadNode* pHead, double magDataX[], double magDataY[], double magDataZ[]) {
	int length = pHead->length;
	if(length <= 0) {
		printf("Empty list, cannot get data array!\n");
		return;
	}
	int i = 0;

	DataNode * ptr = pHead->head;
	while(ptr != NULL) {
		magDataX[i] = ptr->packetData.magX;
		magDataY[i] = ptr->packetData.magY;
		magDataZ[i] = ptr->packetData.magZ;

		ptr = ptr->next;
		i ++;
	}

	if(i != length)
		printf("pay attention, might miss some magnetic data!\n");
}

//int main(void) {
//	SqQueue* queue = create_empty_queue();
//	PktData pktData;
//
//	pktData.accX = 1.0;
//	add_to_queue(queue, pktData);
//
//	pktData.accX = 2.0;
//	add_to_queue(queue, pktData);
//
//	pktData.accX = 3.0;
//	add_to_queue(queue, pktData);
//
//	pktData.accX = 4.0;
//	add_to_queue(queue, pktData);
//
//	printf("Queue length: %d\n", get_queue_length(queue));
//	print_queue(queue);
//
//	delete_from_queue(queue);
//	pktData.accX = 5.0;
//	add_to_queue(queue, pktData);
//
//	print_queue(queue);
//
//	delete_from_queue(queue);
//	pktData.accX = 6.0;
//	add_to_queue(queue, pktData);
//
//	delete_from_queue(queue);
//
//	printf("Queue length: %d\n", get_queue_length(queue));
//	print_queue(queue);
//
//	free(queue);
//}

//int main(void)
//{
//	int i = 0, ret = 0;
//	 DataNode *ptr = NULL;
//
//	print_list();
//
//	for (i = 5; i<10; i++)
//		add_to_list(i, true);
//
//	print_list();
//
//	for (i = 4; i>0; i--)
//		add_to_list(i, false);
//
//	print_list();
//
//	for (i = 1; i<10; i += 4)
//	{
//		ptr = search_in_list(i, NULL);
//		if (NULL == ptr)
//		{
//			printf("\n Search [packetData = %d] failed, no such element found\n", i);
//		}
//		else
//		{
//			printf("\n Search passed [packetData = %d]\n", ptr->packetData);
//		}
//
//		print_list();
//
//		ret = delete_from_list(i);
//		if (ret != 0)
//		{
//			printf("\n delete [packetData = %d] failed, no such element found\n", i);
//		}
//		else
//		{
//			printf("\n delete [packetData = %d]  passed \n", i);
//		}
//
//		print_list();
//	}
//
//	return 0;
//}
