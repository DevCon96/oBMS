#include <Arduino.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "oBMSSettings.h"
#include "oBMSWiFi.h"

#include <oBMSMessage.h> 
#include <oBMSMessage.cpp> 
#include "oBMSBatteryMonitor.h"

//#include <oBMSMessageHandle.h> 
#include "oBMSTRV.h"
#include "oBMSTRV.cpp"

// Message Handle defines
#define BOIL_CONTR 0
#define TRV_CONTR 1
#define STATUS 2

long lastMsg = 0;
char msg[50];
int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);


void oBMSMessageHandle(char* message){

    // Extract message information
    oBMSMessage mes_str;
    mes_str.type = oBMSMesType(message);
    strcpy(mes_str.senseID, oBMSMesSenseID(message));
    strcpy(mes_str.mac, oBMSMesMAC(message));
    strcpy(mes_str.mes, oBMSMesMes(message));
    mes_str.val = oBMSMesVal(message);
    Serial.printf("MAC assigned %s, position from funct %f\n",mes_str.mac,oBMSMesVal(message));
    char eepromPutChar = 1;
    EEPROM.put(15, eepromPutChar);
    EEPROM.commit();
    
    switch (mes_str.type){
        case BOIL_CONTR:
            // Boiler Control from head 
            // Send pin d5 or GPIO14 high
            break;
        case TRV_CONTR:
            // TRV control from head
            //Serial.printf("mes str = %s",mes_str.mes);
            if(!strcmp(mes_str.mes, "M")){
                // Set the TRV to  max
                Serial.println("Setting TRV to Maximum");
                setMaxBool = 1;
                setMinBool = 0;
                setPositionBool = 0;
               
            }
            else if(!strcmp(mes_str.mes, "Z")){
                // Set TRV to min
                Serial.println("Setting TRV to Minimum");
                setMaxBool = 0;
                setMinBool = 1;
                setPositionBool = 0;
                
            }
            else if(!strcmp(mes_str.mes, "P")){
                // Set TRV to position
                Serial.printf("Setting TRV to Position %f\n",mes_str.val);
                setMaxBool = 0;
                setMinBool = 0;
                setPositionBool = 1;
                setPosition = mes_str.val;
            }
            break;
        case STATUS:
            // Status check from the headstaation
            // if(mes_str.mes == "ba"){
            //     // Battery stauts message
            //     if(oBMSBattery() == 0){
            //         // Battery needs charging 
                    
            //     }
            // }
            break;
        default:
            break;
    }
}

// MQTTpub(char ptopic,char messagetype, float data){
// char messageformat[100]="Publish ";
// char *messageformatp = messageformatp;
// char * messagep = messagetype;

// strcat(messageformatp,messagep);
// strcat(messageformatp,": ");
//  Serial.print(messageformat);
//  Serial.println(data);
//  client.publish(ptopic, msg);
// }

//Gets MAC address of current microcontroller, removes colons and returns it as a String
//Can use this to directly subscribe to MAC address via client.subscribe
String oBMSGetMAC(){
  String strMAC = WiFi.macAddress();
  strMAC.remove(2, 1);
  strMAC.remove(4, 1);
  strMAC.remove(6, 1);
  strMAC.remove(8, 1);
  strMAC.remove(10, 1);
  //Prints MAC to serial for debugging purposes
  Serial.println(strMAC);
  return strMAC;
}

//Scans for new MQTT payload and prints out, checks message 
void oBMSWiFiMQTTCallback(char* topic, byte* payload, int length) 
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    char wholeMessage[length];
    for (int i = 0; i < length; i++)    //print each individual character of payload consecutively
      {                                 
        Serial.print((char)payload[i]);
        // char *tempPayload;
        // strcpy(tempPayload,(const char)payload[i]);
        wholeMessage[i] = (char)payload[i];
      }
    Serial.println();
    oBMSMessageHandle(wholeMessage);
}
// Reconnect function
void oBMSWiFiMQTTReconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    //Serial.println(oBMSGetMAC().c_str());
    // char *MACAddr;
    // strcpy(MACAddr,oBMSGetMAC().c_str());
    char *clientName = "client";
    strcpy(clientName,oBMSGetMAC().c_str());
    if (client.connect(clientName)) 
    {
      Serial.println();
      delay(10);
      Serial.println("connected to MQTT");
      // Once connected, publish an announcement...
      // ... and resubscribe
      Serial.println("now about to subscribe");
      //Serial.printf("\nsubscribing to %s\n", MACAddr);

      //Connects to own MAC address as MQTT subscription topic uses oBMSGetMAC to return MAC as const char*
      client.subscribe(oBMSGetMAC().c_str(),1);

    } else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" Attempting to reconnect");
      // Wait 5 seconds before retrying
      delay(RECONNECT_DELAY);
    }
  }
}


void oBMSWiFiSetup() 
{
  delay(10);
  // Begin by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); //begin attempts at connecting to network

  while (WiFi.status() != WL_CONNECTED) //print "..." while connecting
  {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected"); //print connection details once connected
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);  //Setting server as IP address on port 1883
  client.setCallback(oBMSWiFiMQTTCallback);

  Serial.println("MAC ADDRESS: ");
  Serial.println(WiFi.macAddress());
  char tempIsTRV;
  EEPROM.get(15,tempIsTRV);
  isTRV = tempIsTRV;
  Serial.printf("Set isTRV to: %d\n",isTRV);

}


void oBMSWiFiLoop(){
  if (!client.connected()) {
    Serial.println("MQTT has disconnected");
    oBMSWiFiMQTTReconnect();
  }
  client.loop();
}

//Takes Temperatures from main.cpp and formats them into JSON Payloads then sends over MQTT
//Takes in order of Temp A, B, C then D
//NEED TO ADD MAC ADDRESS AUTOMATION
void oBMSTempToMQTT(float Temperature_A, float Temperature_B, float Temperature_C, float Temperature_D){
  
  String JSONMAC = oBMSGetMAC();
  int strLength = JSONMAC.length();
  char arrayMAC[strLength+2];
  JSONMAC.toCharArray(arrayMAC, strLength+1);
  Serial.println("returned temp: ");
  Serial.println(Temperature_A);
  Serial.println(Temperature_B);
  Serial.println(Temperature_C);
  Serial.println(Temperature_D);
  

  //Defines structure and members, formats into char*, sends over PUBTOPIC
  oBMSMessage oBMSMsg;
  oBMSMsg.type = 1;
  strcpy(oBMSMsg.senseID,"1");
  strcpy(oBMSMsg.mac,arrayMAC);
  strcpy(oBMSMsg.mes,"1");
  oBMSMsg.val = Temperature_B;
   
  char* JSONPayload1 = oBMSMesFormat(oBMSMsg);
  client.publish(PUBTOPIC, JSONPayload1);
  Serial.println(JSONPayload1); //Print for debugging purposes

  // //Repeat for next 3 sensors
  // oBMSMessage oBMSTemp2Msg = {DATA, "B", arrayMAC, "T", Temperature_B};
  // char* JSONPayload2 = oBMSMesFormat(oBMSTemp2Msg);
  // client.publish(PUBTOPIC, JSONPayload2);
  // Serial.println(JSONPayload2); //Print for Debugging purposes

  // oBMSMessage oBMSTemp3Msg = {DATA, "C", arrayMAC, "T", Temperature_C};
  // char* JSONPayload3 = oBMSMesFormat(oBMSTemp3Msg);
  // client.publish(PUBTOPIC, JSONPayload3);
  // Serial.println(JSONPayload3); 

  // oBMSMessage oBMSTemp4Msg = {DATA, "D", arrayMAC, "T", Temperature_D};
  // char* JSONPayload4 = oBMSMesFormat(oBMSTemp4Msg);
  // client.publish(PUBTOPIC, JSONPayload4);
  // Serial.println(JSONPayload4); 

}