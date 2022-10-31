# ESP32 Communication
There will be a communication channel established between the STM32L4A6 running the drone control loop
and the Oculus connected to a PC. The STM32 on the drone will use UART to drive an ESP32 to connect 
to a PC running a TCP server on port 40810.

## LAN 
We will be using a router as an access point for this communication channel as well as for the Camera on
the drone. There isn't a need to access the internet, so no ethernet will be necessary.

**SSID:** ENEE408A Drone Team

**Password:** dr0n3sAr3R3@llyC00l!

## firmware.ino
This file is flashed onto the ESP32, which basically acts as a buffer between UART and TCP 40810.
When powered on, the firmware will:

1. Establish a connection to SSID "ENEE408A Drone Team" using a password

2. 

