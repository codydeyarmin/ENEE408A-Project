# ESP32 Communication
There will be a communication channel established between the STM32L4A6 running the drone control loop
and the Oculus connected to a PC. The STM32 on the drone will use UART to drive an ESP32 to connect 
to a PC running a TCP server on port 40810.

The ESP32 can communicate on any UART baud rate. I am currently finding that `115200` is too high of a 
baud rate and tends to miss some bits. The current test setup is using `57600`.

## LAN 
We will be using a router as an access point for this communication channel as well as for the Camera on
the drone. There isn't a need to access the internet, so no ethernet will be necessary.

**SSID:** ENEE408A Drone Team

**Password:** dr0n3sAr3R3@llyC00l!

## firmware.ino
This file is flashed onto the ESP32, which basically acts as a buffer between UART and TCP 40810.
When powered on, the firmware will:

1. (setup) Establish a connection to SSID "ENEE408A Drone Team" using a password

2. (loop) Connect to `192.168.1.x:40810` and hold connection

	- loop will restart on timeout 
	
## test_arduino.ino
This Arduino sketch is an example of how the STM32L4A6 will communicate to the ESP32. This is basically 
a practice setup used for troubleshooting. **TX**-->11, **RX**-->10. Establishes a serial connection using 
baud rate `57600` and uses baud rate `9600` to print debug messages to the screen.

## tcp_server.py
A python script used to start a tcp server on port 40810 and handle incoming clients, which should only be
the ESP32. As of now, there is only one message being received and sent, but the goal is to use asynchronous 
sending of commands and receiving of information using [Msgpack](https://msgpack.org/).

## NOTES
#### ESP32 Flashing
In order to flash the firmware onto the ESP32, you need to go to `File->Preferences` and under 
*Additional Board Manager URLs:* put `https://dl.espressif.com/dl/package_esp32_index.json`. Now, go to 
`Tools->Board->Board Manager` and search **ESP32** and install the libraries for the result that pops up. 

Now, load up the `firmware.ino` into Arduino, plug in the ESP32 using the micro-USB, select the appropriate 
board and port, and click the `Upload` button. When you start to see `Connecting ......_______.......____`, 
you want to **HOLD DOWN** the `BOOT` button on the ESP32. You should start to see the firmware being uploaded.
You can stop holding the button when it says it's done.

#### ESP32 Wiring
The ESP32 has a voltage regulator, taking the voltage down to 3.3V automatically, but it is suggested to directly
apply 3.3V. I use the `3.3V` and `GND` pins for power and ground as well as `RX0` and `TX0` for receive and transmit pins. 

[INSERT IMAGE HERE]
