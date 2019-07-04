//
//  oBMSMessage.h
//  oBMSMessage Library
//  TO BE IMPLEMENTED ON HEADSTATION!!!!

//  Created by Connor Jones on 04/11/2018.
//  Copyright © 2018 Connor Jones.

#include "oBMSMessage.h"
#include "parson.h"
#include <stdio.h>
#include <string.h>


//  Format the message into a JSON string using the entire structure

char *oBMSMesFormat(oBMSMessage mes) {
    // Create the JSON object for the message
    static char *mes_output = NULL;
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    // Occupy the object with the message information.

    char smallFloat[20];

    int ret = snprintf(smallFloat, sizeof smallFloat, "%.4f", mes.val);

    json_object_set_number(root_object, "Type", mes.type);
    json_object_set_string(root_object, "Sensor ID", mes.senseID);
    json_object_set_string(root_object, "MAC", mes.mac);
    json_object_set_string(root_object, "Message", mes.mes);
    json_object_set_string (root_object, "Value", smallFloat);
    // Serialise the object into a string for transmission
    mes_output = json_serialize_to_string_pretty(root_value);
    return((char*) mes_output);
}

/*  Pass the functions the serialised string recieved
    by the client/broker
    The return is the value/type/no_Sensors/SenseID/MAC/Message of that message.
    USE "*ret_mes_info" to retrieve the string information
    Pass the serialized JSON message into the function.
*/
float oBMSMesVal(char *mes_string){
    float val;
    //  Grab information from string
    JSON_Value *root_val;
    JSON_Object *objjson;
    //    Pasrse String, create JSON object
    root_val = json_parse_string(mes_string);
    objjson = json_value_get_object(root_val);
    //  Get value from message
    val = (float) json_object_get_number(objjson,"Value");
    return val;
}

/*  Pass the function the serialised received string
    Return is the string information from the message
    "mes_string" is the serialized JSON formated string.
    "mes_info" is location the information from the JSON
    string is stored too
*/

// Return the type of message used for the message handle
int oBMSMesType(char *mes_string){
    int type;
    //Grab information from string
    JSON_Value *root_val;
    JSON_Object *objjson;
    //  Parse String create JSON object.
    root_val= json_parse_string(mes_string);
    objjson = json_value_get_object(root_val);
    //  Get the information from the JSON object.
    type = json_object_get_number(objjson, "Type");
    return(type);
}

// Retrieve the mac address from the message
char* oBMSMesSenseID(char* mes_string){
    const char* senseID;
    //Grab information from string
    JSON_Value *root_val;
    JSON_Object *objjson;
    //  Parse String create JSON object.
    root_val= json_parse_string(mes_string);
    objjson = json_value_get_object(root_val);
    //  Get the information from the JSON object.
    senseID = json_object_get_string(objjson, "MAC");
    return((char*) senseID);
}

// Retrieve the mac address from the message
char* oBMSMesMAC(char* mes_string){
    const char* mac;
    //Grab information from string
    JSON_Value *root_val;
    JSON_Object *objjson;
    //  Parse String create JSON object.
    root_val= json_parse_string(mes_string);
    objjson = json_value_get_object(root_val);
    //  Get the information from the JSON object.
    mac = json_object_get_string(objjson, "MAC");
    return((char*) mac);
}

char *oBMSMesMes(char *mes_string){
    const char *mes;
    //Grab information from string
    JSON_Value *root_val;
    JSON_Object *objjson;
    //  Parse String create JSON object.
    root_val= json_parse_string(mes_string);
    objjson = json_value_get_object(root_val);
    //  Get the information from the JSON object.
    mes = json_object_get_string(objjson, "Message");
    return ((char*)mes);
}

/*  Pass the funtion all the message information
    the return of the function is the serialized
    JSON formatted string
*/


/*
//  Format the message into a JSON string
char *oBMSMesformat(char *type, char *mes, int  value) {
    static char *mes_output = NULL;
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "Type", type);
    json_object_set_string(root_object, "Message", mes);
    json_object_set_number(root_object, "Value", value);
    mes_output = json_serialize_to_string_pretty(root_value);
    return((char*) mes_output);
}
*/


//  Deserialise the JSON string into a structure
// Return the pointer to the structure.
void oBMSMesDeformat(char* message, oBMSMessage* ret_mes) {
    // Generate the message structure
    oBMSMessage message_stc;
    oBMSMessage* mes_ptr = &message_stc;

    // Extract the information from the string
    ret_mes->type = oBMSMesType(message);
    strcat(ret_mes->mes,oBMSMesMes(message));
    strcat(ret_mes->mac,oBMSMesMAC(message));
    ret_mes->val = oBMSMesVal(message);

}

