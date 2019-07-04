//
// Created by Connor Jones on 2019-03-09.
//
// #include <stdio.h>
//  #include <string.h>
#include "oBMSMessageHandle.h"
#include "oBMSMessage.h"
 //#include "oBMSSQL.c"
#include "oBMSnewemail.c"

//Handle the message
void oBMSMessageHandle(char* message){
    // Extract the information from the string into a structure
    oBMSMessage mes_t;

    //oBMSMesDeformat(message, ptr_mes);
    mes_t.type = oBMSMesType(message);
   // mes_t.no_Sensor = oBMSMesNoSensors(message);
    strcpy(mes_t.senseID, oBMSMesSenseID(message));
    strcpy(mes_t.mac, oBMSMesMAC(message));
    strcpy(mes_t.mes, oBMSMesMes(message));
    mes_t.val = oBMSMesVal(message);

    // checking that the data is sent by a sensor outstation, and not
    // a TRV outstation which hasnt been sent a message to tell it its a TRV yet
    int isSensor;
    for (int i = 0; i < numFloor; ++i){ 
        for (int j = 0; j < floorId[i].numRoom; ++j){
            for (int k =0;k< floorId[i].room[j].numOut; ++k){
                if(!strcmp(mes_t.mac,floorId[i].room[j].outstation[k].MAC)){
                    isSensor = floorId[i].room[j].outstation[k].isInput;
                    printf("This outstation has isSensor of: %d\n",isSensor );
                }
            }
        }
    }

    if(isSensor){
        // Message handle switch statement
        switch(mes_t.type){
            case DATA_MES:
                // Handle data message
                // Place data into database
                printf("Data Message\n");
                storeData(mes_t);
                break;
            case STATUS_MES:
                printf("Status Message\n");

                // Battery Message?
                if(strcmp(mes_t.mes, "Battery")) {
                    // Battery Status Message
                    if(mes_t.val == 1){
                        // Battery isn't empty
                        printf("Battery good\n");
                    }
                    else if (mes_t.val == 0){
                        printf("Battery is low\n");
                        oBMSEmailSend();
                        // Send ALERT Email - Kalu
                    }
                }
                break;
            default:
                printf("Error Message\n");
                // Error Message
                break;
        }
    }
}

void storeData(oBMSMessage message){
    char thisString[10000];

    //printf("%s  %s  %s  %f\n",message.mac, message.type, message.mes, message.val);

    time_t t = time(NULL);
    struct tm *ptm = gmtime(&t);
    char thisTime[256];
    strftime(thisTime, sizeof thisTime, "%F %T", ptm);
    printf("Time now: %s\n",thisTime );


    char thisData[1000] = "'";
    strcat(thisData,thisTime);
    strcat(thisData,"','");


    char thisValStr[12];
    sprintf(thisValStr, "%f", message.val);
    strcat(thisData,thisValStr);
    strcat(thisData,"'");
    printf("This Data: %s\n",thisData );

    char thisTable[40] = "out";
    strcat(thisTable, message.mac);
    printf("This Table: %s\n", thisTable);

    oBMSSQLAddEntry(thisTable, thisData);
    //printf("in %s table\n",thisTable );
    
}

