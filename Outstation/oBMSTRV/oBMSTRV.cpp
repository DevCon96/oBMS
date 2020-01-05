#include <Arduino.h>
#include "oBMSTRV.h"
#include "oBMSSettings.h"
#include <ESP_EEPROM.h>

long lastPosition = 0;
int currentPosition = 0;


//**************************************************************************************
  // Defines wavedrive sequence for stepper motor

const bool stepperOutA[4] = {LOW, HIGH, HIGH, LOW}; //BLACK
const bool stepperOutB[4] = {HIGH, HIGH, LOW, LOW};  //YELLOW
const bool stepperOutC[4] = {LOW, LOW, HIGH, HIGH};  //GREY
const bool stepperOutD[4] = {LOW, HIGH, HIGH, LOW};  //RED

int TRVStart;


int oBMSTRVWritePosition(int inputPosition){
  char positionString[10];
  itoa(inputPosition, positionString, 10);
  for(size_t i = 0; i < 10; i++)
  {
    EEPROM.put(i, positionString[i]);
  }
  EEPROM.commit();
  return 1;
}

void oBMSTRVSetup(){
  

  char positionString[10];
  for(size_t i = 0; i < 10; i++)
  {
   EEPROM.get(i, positionString[i]);
  }
  currentPosition = atoi(positionString);

  //Serial.print("after Reading flash current positon: ");
  //Serial.println(currentPosition);
  
  //oBMSTRVWritePosition(299);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
}


int mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



int forwardPos = 0;
int oBMSTRVForward(){
  if(currentPosition < 30000){
    forwardPos++;
    if(forwardPos >3){
      forwardPos = 0;
    }
    currentPosition ++;
    digitalWrite(coilA, !stepperOutA[forwardPos]);
    digitalWrite(coilB, !stepperOutB[forwardPos]);
    digitalWrite(coilC, !stepperOutC[forwardPos]);
    digitalWrite(coilD, !stepperOutD[forwardPos]);
    oBMSTRVWritePosition(currentPosition);


    return 1;
  }else{
    Serial.println("Current Position at maximum");
    return 0;
  }
}


int backwardPos = 3;
int oBMSTRVBackward(){
  if(currentPosition > 0){
    backwardPos--;
    if(backwardPos < 0){
      backwardPos = 3;
    } 

    currentPosition--;
    oBMSTRVWritePosition(currentPosition);

    digitalWrite(coilA, !stepperOutA[backwardPos]);
    digitalWrite(coilB, !stepperOutB[backwardPos]);
    digitalWrite(coilC, !stepperOutC[backwardPos]);
    digitalWrite(coilD, !stepperOutD[backwardPos]);
    return 1;
  }else{
    Serial.println("Current Position at minimum");
    return 0;
  }
}

int oBMSTRVSetMax(){
  if(currentPosition < PHASETOMINMAX){
    if(TRVStart > 0){
      if(TRVStart % 4){
        oBMSTRVForward();

      }
      TRVStart--;
      return 0;
    }else{
      oBMSTRVForward();
      return 0;
    }
  }else{
    Serial.println("Maxing routine finished");
    return 1;
  }
}


int oBMSTRVSetMin(){
  if(currentPosition > 0){
    if(TRVStart > 0){
      if(TRVStart % 4){
        oBMSTRVBackward();
        //return 0;
      }
      TRVStart--;
      return 0;
    }else{
      oBMSTRVBackward();
      return 0;
    }
  }else{
    Serial.println("finished Min");
    return 1;
  }
}




int oBMSTRVPositionConvert(float positionFloat){
  if(positionFloat >1 || positionFloat < -1){
    Serial.println("ERROR: position out of bounds");
    setPositionBool = 0;
    return 0;
  }else{
    int returnPosition; 
    
    returnPosition = mapFloat(positionFloat,-1,1,0,PHASETOMINMAX);
    //Serial.printf("Converted position from %f to %d\n", positionFloat, returnPosition);
    return returnPosition;
  }
}

bool maxBoolPrev, minBoolPrev, positionBoolPrev;
int lastSerial = 0;
int oBMSTRV(bool maxBool, bool minBool, bool positionBool, float position){
  if(millis()- lastPosition > 8){
    if(millis() - lastSerial > 10000 && isTRV){
      Serial.printf(" current position: %d, TRVstart:%d, position set to %d \n",currentPosition, TRVStart, oBMSTRVPositionConvert(position));
      lastSerial = millis();
    }
    if(maxBool > maxBoolPrev || minBool > minBoolPrev || positionBool > positionBoolPrev){
      TRVStart = 200;
      maxBoolPrev = 1;
      minBoolPrev = 1;
      positionBoolPrev = 1;
      Serial.println("Starting to move TRV");
    }

    lastPosition = millis();
    if(maxBool){
      //Serial.print("TRV to MAX, current position = ");
      //Serial.println(currentPosition);
      if(oBMSTRVSetMax()){
       // setMaxBool = 0;
        return 1; 
      }
    }else{
      maxBoolPrev = 0;
    }
    
    if(minBool){
      //Serial.print("TRV to MIN, current position = ");
      //Serial.println(currentPosition);
      if(oBMSTRVSetMin()){
        //setMinBool = 0;
        return 1; 
      }
    }else{
      minBoolPrev = 0;
    }

    if(positionBool){
      
      int thisPosition = oBMSTRVPositionConvert(position);
      //Serial.printf("TRV setting position to %d, current position: %d, TRVstart:%d, positionboolPrev: %d\n", thisPosition,currentPosition, TRVStart, positionBoolPrev);
      if(thisPosition < currentPosition){
        //Serial.println("going backwards");
        if(TRVStart > 0){
          if(TRVStart % 4){
          //Serial.println("go backwards");
          oBMSTRVBackward();
          }
          TRVStart --;
        }else{
          oBMSTRVBackward();
        }
      }else if(thisPosition > currentPosition){
        //Serial.println("going forwards");
        //Serial.printf("TRVStart = %d\n",TRVStart);
        if(TRVStart > 0){
          if(TRVStart % 4){
            //Serial.println("go forwards");
            oBMSTRVForward();
          }
          //Serial.println("not mod 4");
          TRVStart --;
        }else{
         // Serial.println("go forwards, not slowly");
          oBMSTRVForward();
        }
      }else if(thisPosition == currentPosition){
        setPositionBool = 0;
        positionBoolPrev = 0;
      }
      return 1;
    }else{
      //Serial.println("position bool  = 0");
      positionBoolPrev = 0;
    }
    return 0;
  }
  return 0;
}


//  if (Serial.available() > 0) {
//     serialFloatStr = Serial.readString();
//     Serial.print("Received Float: ");
//     Serial.println(serialFloatStr);
//     setPosition = serialFloatStr.toFloat();
//     Serial.print("As a float: ");
//     Serial.println(setPosition);
//     setPositionBool =1;
//   }
