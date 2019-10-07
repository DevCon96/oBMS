# oBMS Project
Open-Source Building Management System (Messaging)

# Headstation section
## oBMSMessage Library
This library includes all the relevant code to impletement the headstation side of the of the code indepentantly of the outstation code.
This includes formatting and deformating functions.

## oBMSMessageHandle Library 
This library includes all the relevant code to implement the headstation message handling. This library will intercept all incomming messages and they handle them in the relevant manner.

# Oustation section
## oBMSMessage
This library includes all the JSON formatting and deformatting functions required for the communication channel of the project.
The outstation is where most of the formatting occurs and these are the primary message sending devices.

## oBMSMessageHandle 
This will intercept any messages from the headstation and execute relevant instructions to respond to the relevant messages. This will enable the devices to control the TRV's relevant to the error calculated by the headstation
