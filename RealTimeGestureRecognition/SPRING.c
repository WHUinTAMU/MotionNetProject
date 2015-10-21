#include "SPRING.h"

/**
*TASK:the process struct of one kind of gesture
*grp:the process struct of the specific type of gesture
*xt:the current data inputed
*/
void update_array(GRProcess *grp, PktData xt, int position)
{
    char from[2];
    grp->distanceArray[0] = 0;
    grp->startArray[0] = position;
    grp->timeArray[0] = xt.timeStamp;
    DataNode *p = grp->originalGesture.head->head;
    int m = grp->originalGesture.m;

    int i = 0;
    /*for(i = 0; i <= m;i++)
    {
        printf("%lf  ",grp->distanceArrayLast[i]);
    }*/

    for(i = 1; i < m + 1; i++)
    {
        int startTmp;
        long int timeTmp;
        double distanceTmp = pow((xt.accX - p->packetData.accX), 2)
        + pow((xt.accY - p->packetData.accY), 2) + pow((xt.accZ - p->packetData.accZ), 2)
        + pow((xt.gyroX - p->packetData.gyroX), 2) + pow((xt.gyroY - p->packetData.gyroY), 2)
        + pow((xt.gyroZ - p->packetData.gyroZ), 2);

        //printf("d[%d - 1] = %lf:::d'[%d] = %lf:::d'[%d - 1] = %lf\n",i,grp->distanceArray[i - 1],i,grp->distanceArrayLast[i],i,grp->distanceArrayLast[i - 1]);

        if(grp->distanceArray[i - 1] <= grp->distanceArrayLast[i])
        {
            if(grp->distanceArray[i - 1] <= grp->distanceArrayLast[i - 1])
            {
                distanceTmp += grp->distanceArray[i - 1];
                startTmp = grp->startArray[i - 1];
                timeTmp = grp->timeArray[i - 1];
                from[0] = '<';
                from[1] = '-';
            }
            else
            {
                distanceTmp += grp->distanceArrayLast[i - 1];
                startTmp = grp->startArrayLast[i - 1];
                timeTmp = grp->timeArrayLast[i - 1];
                from[0] = '^';
                from[1] = '\\';
            }
        }
        else
        {
            if(grp->distanceArrayLast[i] <= grp->distanceArrayLast[i - 1])
            {
                distanceTmp += grp->distanceArrayLast[i];
                startTmp = grp->startArrayLast[i];
                timeTmp = grp->timeArrayLast[i];
                from[0] = '^';
                from[1] = '|';
            }
            else
            {
                distanceTmp += grp->distanceArrayLast[i - 1];
                startTmp = grp->startArrayLast[i - 1];
                timeTmp = grp->timeArrayLast[i - 1];
                from[0] = '^';
                from[1] = '\\';
            }
        }
        grp->distanceArray[i] = distanceTmp;
        grp->startArray[i] = startTmp;
        grp->timeArray[i] = timeTmp;
        p = p->next;
    }
}

/**
*TASK:the main part of the DTW algorithm
*grProcess:the process struct of the specific type of gesture
*xt:the type recognized
*/
int SPRING(PktData xt, GRProcess *grProcess, int position, SqQueue* queue)
{
    int is_gesture = NONE_TYPE;

    int m = grProcess->originalGesture.m;
    double *dmin = &(grProcess->dmin);
    int *ts = &(grProcess->ts);
    int *te = &(grProcess->te);
    long int *times = &(grProcess->times);
    long int *timee = &(grProcess->timee);
    DataHeadNode *originalGestureList = grProcess->originalGesture.head;
    double threshold = grProcess->threshold;

    update_array(grProcess, xt, position);

    //check whether the temporary optimal subsequence is a right one
    if(*dmin <= threshold)
    {
        bool is_di_largerthan_dmin = true;
        bool is_si_largerthan_te = true;

        //whether dm > dmin
        int i = 0;
        for(i = m; i <= m; i++)
        {
            if(grProcess->distanceArray[i] < *dmin)
            {
                is_di_largerthan_dmin = false;
            }

            if(grProcess->startArray[i] <= *te)
            {
                is_si_largerthan_te = false;
            }

            if(is_di_largerthan_dmin == false /*&& is_si_largerthan_te == false*/)
            {
                break;
            }
        }

        if(is_di_largerthan_dmin == true /*|| is_si_largerthan_te == true*/)
        {
            //check the time span of the temporary optimal subsequence
            int timeGap = *timee - *times;
            //printf("%d\n",timeGap);
            if(timeGap >= grProcess->timeLimit)
            {
                is_gesture = true;
                int t = grProcess->type;

                //report the right optimal subsequence
                switch(t)
                {
                    case POINT_TYPE:printf("\n\n!!!!!!!!\nsuccess!\npoint!!!\ndmin=%f\nts=%d\nte=%d\nt=%d\ntime span=%d\n!!!!!!!!\n\n",
                                  *dmin,*ts,*te,position,*timee - *times);is_gesture = POINT_TYPE;break;
                    case ROTATE_RIGHT_TYPE:printf("\n\n!!!!!!!!\nsuccess!\nrotate right!!!\ndmin=%f\nts=%d\nte=%d\nt=%d\ntime span=%d\n!!!!!!!!\n\n",
                                  *dmin,*ts,*te,position,*timee - *times);is_gesture = ROTATE_RIGHT_TYPE;break;
                    case ROTATE_LEFT_TYPE:printf("\n\n!!!!!!!!\nsuccess!\nrotate left!!!\ndmin=%f\nts=%d\nte=%d\nt=%d\ntime span=%d\n!!!!!!!!\n\n",
                                  *dmin,*ts,*te,position,*timee - *times);is_gesture = ROTATE_LEFT_TYPE;break;
                    case SLIDE_OVER_TYPE:printf("\n\n!!!!!!!!\nsuccess!\nslide over!!!\ndmin=%f\nts=%d\nte=%d\nt=%d\ntime span=%d\n!!!!!!!!\n\n",
                                  *dmin,*ts,*te,position,*timee - *times);is_gesture = SLIDE_OVER_TYPE;break;
                    case STAND_UP_TYPE:printf("\n\n!!!!!!!!\nsuccess!\nstand up!!!\ndmin=%f\nts=%d\nte=%d\nt=%d\ntime span=%d\n!!!!!!!!\n\n",
                                  *dmin,*ts,*te,position,*timee - *times);is_gesture = STAND_UP_TYPE;break;
                    case SIT_DOWN_TYPE:printf("\n\n!!!!!!!!\nsuccess!\nsit down!!!\ndmin=%f\nts=%d\nte=%d\nt=%d\ntime span=%d\n!!!!!!!!\n\n",
                                  *dmin,*ts,*te,position,*timee - *times);is_gesture = SIT_DOWN_TYPE;break;
                    case TARGET_TYPE:/*printf("\n\n!!!!!!!!\nsuccess!\ntarget!!!\ndmin=%f\nts=%d\nte=%d\nt=%d\ntime span=%d\n!!!!!!!!\n\n",
                                  *dmin,*ts,*te,position,*timee - *times);*/is_gesture = TARGET_TYPE;break;
                    case WALK_TYPE:printf("\n\n!!!!!!!!\nsuccess!\nwalk!!!\ndmin=%f\nts=%d\nte=%d\nt=%d\ntime span=%d\n!!!!!!!!\n\n",
                                  *dmin,*ts,*te,position,*timee - *times);is_gesture = WALK_TYPE;break;

                }

                //reinitialize the dmin,d
                *dmin = DBL_MAX;

                int i = 0;
                for(i = 1; i <= m; i++)
                {
                    if(compare_two_position(queue, *te, grProcess->startArray[i]))
                    {
                        grProcess->distanceArray[i] = DBL_MAX;
                    }
                }
            }

        }
    }


    //check whether the current subsequence can be determined as a temporary optimal subsequence
    if(grProcess->distanceArray[m] <= threshold && grProcess->distanceArray[m] < *dmin)
    {
        *dmin = grProcess->distanceArray[m];
        *ts = grProcess->startArray[m];
        *te = position;
        *times = grProcess->timeArray[m];
        *timee = xt.timeStamp;

        //printf("dmin=%lf:::te=%d:::ts=%d:::timee=%d:::times=%d:::time=%d\n",*dmin,*te,*ts,*timee,*times,*timee-*times);
    }

    //if(grProcess->type == 1)
        //printf("%d::dm = %lf::sm = %d\n", xt.pktNumber, grProcess->distanceArray[m], grProcess->startArray[m]);

    // replace the d with d', and s with s'
    double *dtmp = grProcess->distanceArray;
    grProcess->distanceArray = grProcess->distanceArrayLast;
    grProcess->distanceArrayLast = dtmp;

    int *stmp = grProcess->startArray;
    grProcess->startArray = grProcess->startArrayLast;
    grProcess->startArrayLast = stmp;

    long int *ttmp = grProcess->timeArray;
    grProcess->timeArray = grProcess->timeArrayLast;
    grProcess->timeArrayLast = ttmp;

    return is_gesture;
}
