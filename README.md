# Smart-Dustbin
This repository consists of all the work done to make a smart dustbin for the Social Entrepreneurship Competition of Inter IIT Tech Meet 10.0
So, our product consists of a smart dustbin which uses different sensors 
and servo motor to segregate organic waste and tell the status of the 
dustbin to our server.
Our dustbin consists of following sensors-
➔ Ultrasonic range sensor
➔ Capacitive soil moisture sensor
➔ NPN Inductive proximity sensor
➔ Photoelectric proximity sensor
➔ Weighing Load cell sensor
➔ Gear Servo Motor
Basic structure of a dustbin consists of two bins with a rotatable 
platform on top, the platform is equipped with the inductive and photoelectric proximity sensor and 
weight sensor on the bottom. Each bin has an ultrasonic sensor to check for bin capacity and a moisture 
sensor which will tell the dryness of the waste. All these sensors will be controlled through an Arduino 
Mega and all the data will be sent through ESP8266. All these components will be powered through an 
external battery of 12V.

The inductive and photoelectric proximity sensors are used to detect if there is any content of plastic, 
wood, metal etc. If any of these is detected in the poured waste, the platform is rotated by two servo 
motors and the waste is poured in the right bin with its weight added to the weight already present in 
the right bin. If nothing non-organic is detected, then all the waste is poured into the left bin. In this way 
we have separated pure organic waste and waste with organic and non-organic contents.

IoT Network Implementation for the Dustbins
Our first task was to create a local network of the dustbins to 
know the relevant information about each dustbin. As shown in 
the figure, there is one central server which will receive data 
from all the other ESP8266 nodes (Dustbins).
We have used the ESP-Now Protocol instead of Wi-Fi.
Working of the system:
● Here each dustbin (ESP8266) is programmed to operate 
in station mode and the central server is programmed 
to operate in access point mode.
● Each node (Dustbin) is connected to multiple sensors 
like GPS, temperature, Ultrasonic sensor and collecting 
data in real time.
● We are sending this data from each node to the central server using the mac address of the 
server.
● For identifying each dustbin, we are sending unique Id from each dustbin along the sensor 
readings to the central server.

