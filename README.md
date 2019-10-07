# oBMS Project
Open-Source Building Management System (Messaging)
This project was completed as part of a masters group project at The University of Sheffield and plays a role in a larger research topic affiliated with ARUP. The overall aim of the project was to design, build and test a system which would have the capability of performing basic HVAC controls within a larger building concept. 

Currently such systems are closed to a small group of companies and require the user to contract out work on the systems to these companies in order to make changes. The solution aims to solve this byt allowing the system to be open source such that the facilities manager could modify the system without contracting an external engineer to work on it.

All licenses used in the project allow for the client to choose whether they would like to continue having their system open source or commercialising their implementation.

# Communication Channel Description
For the communciation of the system, JSON payload formatting was used with the MQTT protocol to allow for messages to be sent between clients without instant responses. Buildings response slowly to instantaneous responses aren't required

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

# Copyright 
This project is owned by The University of Sheffield 2019.
Project section executed by Connor Jones

All rights reserved

