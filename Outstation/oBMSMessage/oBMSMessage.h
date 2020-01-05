//
// Created by Connor Jones on 2019-03-09.
//
#ifndef OBMSMESSAGE_LIBRARY_H
#define OBMSMESSAGE_LIBRARY_H

#ifdef __cplusplus
extern "C"
{
#endif

//  Structure to generate the message
typedef struct {
    // no_Sensor = number of sensors on the outstation
    // H-O comms USE both = 0;
    int type;
    char senseID[2];
    char mac[14];
    char mes[2];
    float val;

} oBMSMessage;


// Link all library functions
// Format the message structure into a string
char* oBMSMesFormat(oBMSMessage mes);

// Retrieve information from the received string
int oBMSMesType(char* mes_string);
int oBMSMesNoSensors(char* mes_string);
char* oBMSMesSenseID(char* mes_string);
double oBMSMesVal(char* mes_string);
char* oBMSMesMes(char *mes_string);
char* oBMSMesMAC(char* mes_string);


void oBMSMesDeformat(char* message,oBMSMessage* ret_mes);



#ifdef __cplusplus
}
#endif

#endif


