//
// Created by Connor Jones on 2019-03-09.
//

#ifndef OBMS_OBMSMESSAGEHANDLE_LIBRARY_H
#define OBMS_OBMSMESSAGEHANDLE_LIBRARY_H

#ifdef __cplusplus
extern "C"{
#endif

#include "oBMSMessage.h"

#define DATA_MES 1
// #define COMMAND_MES 2
#define STATUS_MES 2

void oBMSMessageHandle(char *message);
void storeData(oBMSMessage message);


#endif // OBMS1_

#ifdef __cplusplus
}
#endif
