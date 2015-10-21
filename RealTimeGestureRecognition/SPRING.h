#ifndef SPRING_H
#define SPRING_H

#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "DataNode.h"

#define TARGET_THRESHOLD 8
#define POINT_THRESHOLD 135
#define ROTATE_RIGHT_THRESHOLD 30
#define ROTATE_LEFT_THRESHOLD 60
#define SLIDE_OVER_THRESHOLD 45
#define STAND_UP_THRESHOLD 28
#define SIT_DOWN_THRESHOLD 20
#define WALK_THRESHOLD 50

#define TARGET_TIMELIMIT 0
#define POINT_TIMELIMIT 500
#define ROTATE_RIGHT_TIMELIMIT 400
#define ROTATE_LEFT_TIMELIMIT 400
#define SLIDE_OVER_TIMELIMIT 500
#define STAND_UP_TIMELIMIT 550
#define SIT_DOWN_TIMELIMIT 550
#define WALK_TIMELIMIT 800

#define NONE_TYPE -1
#define TARGET_TYPE 0
#define POINT_TYPE 1
#define ROTATE_RIGHT_TYPE 2
#define ROTATE_LEFT_TYPE 3
#define SLIDE_OVER_TYPE 4
#define STAND_UP_TYPE 5
#define SIT_DOWN_TYPE 6
#define WALK_TYPE 7

#define DTW_NUM 8


/**
*original gesture model struct
*/
typedef struct originalGesture{
    DataHeadNode *head; //the head node of the list of the model data
    int m;// the length of the list
}OriginalGesture;

/**
*the process struct of one kind of gesture
*/
typedef struct gestureRecognitionProcess{
    OriginalGesture originalGesture;// the gesture model struct
    double *distanceArray;//array d
    double *distanceArrayLast;//array d'
    int *startArray;//array s
    int *startArrayLast;//array s'
    int *timeArray;//array s
    int *timeArrayLast;//array s'
    double dmin;// dmin
    int ts;//ts
    int te;//te
    long int times;//time stamp of the start
    long int timee;//time stamp of the end
    double threshold;// threshold
    int type;// determine which gesture it is
    long int timeLimit;// time limit of a piece of right data
}GRProcess;

/**
*TASK:the process struct of one kind of gesture
*grp:the process struct of the specific type of gesture
*xt:the current data inputed
*/
void update_array(GRProcess *grProcess, PktData xt, int position);

/**
*TASK:the main part of the DTW algorithm
*grProcess:the process struct of the specific type of gesture
*xt:the current data inputed
return: the front of the queue
*/
int SPRING(PktData xt, GRProcess *grProcess, int position, SqQueue* queue);

#endif // SPRING_H
