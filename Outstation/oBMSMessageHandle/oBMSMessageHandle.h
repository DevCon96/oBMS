
#ifndef OBMS_OBMSMESSAGEHANDLE_LIBRARY_H
#define OBMS_OBMSMESSAGEHANDLE_LIBRARY_H

#ifdef __cplusplus
extern "C"{
#endif

#include "oBMSMessage.h"

#define BOIL_CONTR 0
#define TRV_CONTR 1
#define STATUS 2

void oBMSMessageHandle(char *message);
void storeData(oBMSMessage message);


#endif // OBMS1_

#ifdef __cplusplus
}
#endif
