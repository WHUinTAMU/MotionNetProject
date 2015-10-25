#include "DataNode.h"

double convertReadingToHeading(double y,double x){
    //y and x are the readings from the magnetometer
    double Pi= (double) 3.14159;

    // Calculate the angle of the vector y,x; -Pi<atan2(y,x)<Pi;
    double heading = (double) ((atan2(-1*y,x) * 180) / Pi);

    // Change the pointing direction from X-axis to Y-axis
    heading = heading - 90;

    // Normalize to 0-360
    if (heading < 0)
    {
        heading = 360 + heading;
    }

    if (heading > 360)
    {
        heading = heading - 360;
    }

    return heading;
}


//z is the heading introduced, and a1 is the heading from the second lamp to the first lamp
int convertHeadingToTarget(double z, double headingFrom2To1)  {

// Convert to my own weighing system and normalize as well
    z = z - headingFrom2To1;
    if (z < 0)
    {
        z = 360 + z;
    }
    int target = 0 ;

//Judge which lamp the heading indicates
    if (0 <= z && z < 90){
        target = 1;
    }
    else if(90 <= z && z < 180){
        target = 2;
    }
    else if(180 <= z && z < 270){
        target = 3;
    }
    else if(270 <= z && z <=360){
        target = 4;
    }

    return target;
}





int pickTarget(DataHeadNode head, double headingFrom2To1){
    double direction[head.length];

    double sum = 0;

    DataNode *ptr =  head.head;

    double magX[head.length];
    double magY[head.length];
    double magZ[head.length];

    //Compute the headings of every set of mag data and the sum of them
    int i;
    for (i = 0;i < head.length;i++){
        magX[i] = ptr->packetData.magX;
        magY[i] = ptr->packetData.magY;
        magZ[i] = ptr->packetData.magZ;
        ptr = ptr->next;
    }

    double heading[head.length];
    calibrateMagData(magX, magY, magZ, heading, head.length);

    for(i = 0; i < head.length; i++)
    {
        sum += heading[i];
    }

    double averageHeading = sum / head.length;

    //printf("-------%f-------",averageHeading);

    int targetOfLamp = convertHeadingToTarget(averageHeading,headingFrom2To1);



 return targetOfLamp;

}
