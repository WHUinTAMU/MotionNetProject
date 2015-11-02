#include "LampController.h"

extern bool onValue[4] = {true,true,true,true};
extern int briValue[4] = {254,254,254,254};
extern int hueValue[4] = {31767,31767,31767,31767};

bool createCommand(int stateType, int valueChange, int target)
{
    cJSON * pJsonRoot = NULL;
    pJsonRoot = cJSON_CreateObject();
    if(NULL == pJsonRoot)
    {
        //error happend here
        printf("error when create json");
        return false;
    }

    switch(stateType){
        case ON_TYPE:
            {
                onValue[target - 1] = (onValue[target - 1] == true ? false : true);
                cJSON_AddBoolToObject(pJsonRoot, ON_STRING, onValue[target - 1]);
                break;
            }
        case BRI_TYPE:
            {
                int value = (briValue[target - 1] + valueChange) > 254 ? 254 : (briValue[target - 1] + valueChange < 0 ?
                                                                   0 : briValue[target - 1] + valueChange);
                briValue[target - 1] = value;
                cJSON_AddNumberToObject(pJsonRoot, BRI_STRING, value);
                break;
            }
        case HUE_TYPE:
            {
                int value = (hueValue[target - 1] + valueChange);
                value = value < 0 ? 65535 + value : value % 65535;
                hueValue[target - 1] = value;
                cJSON_AddNumberToObject(pJsonRoot, HUE_STRING, value);
                break;
            }
    }

    printf("%s",cJSON_Print(pJsonRoot));
    setLightState(target, cJSON_Print(pJsonRoot));
    cJSON_Delete(pJsonRoot);
}
