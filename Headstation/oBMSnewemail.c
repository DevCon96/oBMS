#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// download ssmtp
//set ssmtp.config file
// root=oBMSEmail@gmail.com
// mailhub=smtp.gmail.com:587
// AuthUser=oBMSEmail@gmail.com
// AuthPass=KaluKalu1
// UseSTARTTLS=YES
int oBMSEmailSend() {// if the recepient or text file need to be changed,change it from the string in cmd 

        char cmd[100]="ssmtp k_onuma@yahoo.com< oBMSmes.txt";  // to hold the command.
        system(cmd);     // execute it.
        printf("email sent");

        return 0;
}