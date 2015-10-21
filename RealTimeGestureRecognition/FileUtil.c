#include "FileUtil.h"

void write_queue_to_file(char * fileName, SqQueue * queue) {
	if(is_empty_queue(queue))
		return;
	else {
		FILE *stream = fopen(fileName, "w+");
		fprintf(stream, "Number\tAccX\t\tAccY\t\tAccZ\t\tGyroX\t\tGyroY\t\tGyroZ\t\tMagX\t\tMagY\t\tMagZ\r\n");

		int i = queue->front;
		while(i != queue->rear) {
			fprintf(stream, "%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\r\n", i,
			        queue->accXData[i], queue->accYData[i], queue->accZData[i],
			        queue->gyroXData[i], queue->gyroYData[i], queue->gyroZData[i],
			        queue->magXData[i] , queue->magYData[i],  queue->magZData[i]);
			i = (i + 1) % MAX_SIZE;
		}
		fclose(stream);
	}
}

//Write list data to file.
void write_list_to_file(char * fileName, DataHeadNode *pHead) {
	DataNode *ptr = pHead->head;
	if(ptr == NULL)
		return;

	PktData pktData;
	FILE *stream = fopen(fileName, "w+");
	fprintf(stream, "AccX\t\tAccY\t\tAccZ\t\tGyroX\t\tGyroY\t\tGyroZ\t\tMagX\t\tMagY\t\tMagZ\t\tNumber\tTimeStamp\r\n");
	while (ptr != NULL) {
		pktData = ptr->packetData;
		fprintf(stream, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%ld\t%lu\r\n", pktData.accX, pktData.accY, pktData.accZ,
		        pktData.gyroX, pktData.gyroY, pktData.gyroZ, pktData.magX, pktData.magY, pktData.magZ, pktData.pktNumber, pktData.timeStamp);
		ptr = ptr->next;
	}
	fclose(stream);
}

//Write packet data to file.
void write_pkt_to_file(char * fileName, PktData pktData) {
	//printf("add to file");
	FILE *stream = fopen(fileName, "a+");
	fprintf(stream, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%ld\t%lu\r\n", pktData.accX, pktData.accY, pktData.accZ,
	        pktData.gyroX, pktData.gyroY, pktData.gyroZ, pktData.magX, pktData.magY, pktData.magZ, pktData.pktNumber, pktData.timeStamp);
	fclose(stream);
}

//Write magnetic data to file.
void write_mag_to_file(char * fileName, double x[], double y[], double z[], double heading[], int len) {
	FILE *stream = fopen(fileName, "w+");
	fprintf(stream, "Number\tMagX\t\tMagY\t\tMagZ\t\tHeading\r\n");
	int i;
	for(i = 0; i < len; i ++)
		fprintf(stream, "%d\t%f\t%f\t%f\t%.4f\r\n", i, x[i], y[i], z[i], heading[i]);
	fclose(stream);
}


OriginalGesture *read_file_to_init_original_gesture(char * fileName)
{
    FILE * fp;

    /* open */
    if ((fp = fopen(fileName,"r")) == NULL)
    {
        printf("Can't open ts_list.txt\n");
        exit(1);
    }

    bool isFirst = true;
    OriginalGesture *og = (OriginalGesture*) malloc(sizeof(OriginalGesture));
    DataNode *tmp = NULL;
    DataNode *head = NULL;
    double tmpArray[6];
    int tmpPktNum = 0;
    int i = 1;
    while(EOF != fscanf(fp, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%d", &tmpArray[0], &tmpArray[1], &tmpArray[2], &tmpArray[3],
                  &tmpArray[4], &tmpArray[5], &tmpPktNum))
    {
        PktData *pd = (PktData*) malloc(sizeof(PktData));
        pd->accX = tmpArray[0];
        pd->accY = tmpArray[1];
        pd->accZ = tmpArray[2];
        pd->gyroX = tmpArray[3];
        pd->gyroY = tmpArray[4];
        pd->gyroZ = tmpArray[5];
        pd->pktNumber = tmpPktNum;

        DataNode *dn = (DataNode*) malloc(sizeof(DataNode));
        dn->packetData = *pd;

        if(isFirst)
        {
            isFirst = false;
            head = dn;
            tmp = dn;
        }
        else
        {
            tmp->next = dn;
            tmp = dn;
        }
        i++;

    }
    tmp->next = NULL;

    /* close */
    fclose(fp);

    DataHeadNode *dhn = (DataHeadNode*) malloc(sizeof(DataHeadNode));
    dhn->head = head;
    dhn->length = i - 1;

    og->head = dhn;
    og->m = i - 1;

    return og;
}
