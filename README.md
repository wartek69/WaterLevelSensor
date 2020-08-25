# WaterLevelSensor
A water level measurement sensor connected to thingsboard using an esp8266 that will publish measurement data each minute.

## Hardware
For this project an ultrasonic waterproof sensor was used as sensor, it can be bought pretty cheap on banggood or aliexpress. In the sources a link to this sensor can be found.
To connect the sensor to thingsboard over mqtt an d1 mini pro was used, this is an esp8266 based device.
When connecting the ultrasonic sensor to the esp8266 make sure to use either a logic level shifter (from 5v to 3.3v) or a voltage divider for the echo pin. An example schematic can be found here https://letscontrolit.com/wiki/index.php?title=File:HC-SR04_simple.jpg. This needs to be done since the esp8266 operates on 3.3V and the ultrasonic sensor is powered from 5V. It might work without the voltage divider, but it's not advised to do so.

## Thingsboard
Thingsboard is an opensource iot platform that can be self hosted. This is a perfect platform for visualisation and some preprocessing of your data. The setup is fairly easy and can be found on their official website https://thingsboard.io/.
The code uses MQTT to publish the sensor data to the thingsboard server. If you are hosting the thingsboard server behind a firewall make sure to forward the MQTT port (1883).

## Power saving
If you plan on running the esp8266 board on a battery it's advised to change the code to let the esp8266 go into deep sleep in between readings. This wasn't added in this repo since I wasn't using a battery power source.

## Sources 
https://www.banggood.com/nl/Waterproof-Ultrasonic-Sensor-Module-Integrated-Ranging-Sensor-Reversing-Radar-Measuring-Distance-p-1369915.html?gmcCountry=BE&currency=EUR&createTmp=1&utm_source=googleshopping&utm_medium=cpc_bgs&utm_content=xibei&utm_campaign=xibei-pla-be-pc-nl-rm-all-purchase-1024&gclid=Cj0KCQjw7ZL6BRCmARIsAH6XFDKmp3t68WWDq_ikyfzh1azs-hjXFEXfP7R2_GFCuLQIAaQWsr8n1jMaAvE9EALw_wcB&cur_warehouse=CN
