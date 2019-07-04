//
// Created by Connor Jones on 2019-03-09.
//
#ifndef OBMSMESSAGE_LIBRARY_H
#define OBMSMESSAGE_LIBRARY_H

#ifdef __cplusplus
extern "C"
{
#endif

#define DATA_MES 1
// #define COMMAND_MES 2
#define STATUS_MES 2

//  Structure to generate the message
typedef struct {
    // no_Sensor = number of sensors on the outstation
    // H-O comms USE both = 0;
    int type;

    	// 0 = TRV
    	// 1 = Bool output so on off switch for heater
    	// 2 = weatherstation?

    char senseID[2];
    char mac[13];
    char mes[2];
    	// "sp" = setpoint temperature

    float val;

} oBMSMessage;


// Link all library functions
char* oBMSMesFormat(oBMSMessage mes);
int oBMSMesType(char* mes_string);
int oBMSMesNoSensors(char* mes_string);
char* oBMSMesSenseID(char* mes_string);

float oBMSMesVal(char* mes_string);

char* oBMSMesMes(char *mes_string);
char* oBMSMesMAC(char* mes_string);
//char* oBMSMesformat(char *type, char *mes, int  value);
// TODO Use the structure here! CAPITAL "F" in format

void oBMSMesDeformat(char* message,oBMSMessage* ret_mes);



#ifdef __cplusplus
}
#endif

#endif


