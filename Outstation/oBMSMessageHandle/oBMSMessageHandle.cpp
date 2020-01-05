// #include <stdio.h>
// #include <string.h>
#include <Arduino.h>
#include "oBMSMessageHandle.h"
#include "oBMSMessage.h"
#include "oBMSBatteryMonitor.h"
#include "oBMSTRV.h"
#include "oBMSTRV.cpp"


//#include "oBMSWiFi.h"
//#include "pubsubclient.h"

// static oBMSMessage bat_low = {2, "X", oBMSGetMac, "B", 0};

void oBMSMessageHandle(char* message){

    // Extract message information
    oBMSMessage mes_str;
    mes_str.type = oBMSMesType(message);
    strcat(mes_str.senseID, oBMSMesSenseID(message));
    strcat(mes_str.mac, oBMSMesMAC(message));
    strcat(mes_str.mes, oBMSMesMes(message));
    mes_str.val = oBMSMesVal(message);

    switch (mes_str.type){
        case BOIL_CONTR:
            // Boiler Control from head 
            // Send pin d5 or GPIO14 high
            break;
        case TRV_CONTR:
            // TRV control from head
            if(strcmp(mes_str.mes, "M")){
                // Set the TRV to  max
                setMaxBool = 1;
                setMinBool = 0;
                setPositionBool = 0;
            }
            else if(strcmp(mes_str.mes, "Z")){
                // Set TRV to min
                setMaxBool = 0;
                setMinBool = 1;
                setPositionBool = 0;
            }
            else if(strcmp(mes_str.mes, "P")){
                // Set TRV to position
                setMaxBool = 0;
                setMinBool = 0;
                setPositionBool = 1;
                setPosition = mes_str.val;
            }
            break;
        case STATUS:
            // Status check from the headstaation
            if(strcomp(mes_str.mes, "B"){
                // Battery stauts message
                if(oBMSBattery() == 0){
                    // Battery needs charging 

                }
            }
            break;
        default:
            printf("Error message!\n");
            break;
    }
}


