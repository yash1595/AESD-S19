# APES Project 2  

Project Partners : Raj Lavingia  
	           Yash Gupte 
		   
**Project Description :**
To communicate between TIVA and Beagle Bone Green Board in a full duplex method. Tiva is considered as a remote node and BBG is considered as a control node. Ultrasonic Sensor, Temperature Sensor, Finger Print Sensor and Smoke Sensor were integrated on TIVA. All the code was written in FreeRTOS and by the use of its libraries. Different Tasks were made for each and every sensor. All the tasks were running at the same time and timing was set using vTaskDelay command. The data from all the tasks was transferred into a Queue which handled all the issues of bus contention. 

Startup Test was performed at the beginning when the system was made on. If any of the sensors fail then the system used to terminate gracefully. If the Startup test was passed then only the tasks were spawned.

Temperature Sensor : This Sensor was interfaced using I2C communication between the TIVA board and TMP102 Sensor.
Ultrasonic Sensor : Digital Interface. In order to run this sensor, first of all a pulse of 10us was given to the trigger pin of ultrasonic sensor in order to give it an initial stroke. After receiving the 10us on the trigger pin, the sensor sends a 5v from the echo pin to the controller but since TIVA board understands 3.3 v on the GPIO we used a voltage divider to protect our circuit from extensive load.
Fingerprint Sensor : UART Communication. First of all , the code was developed to recognise and store the data of the finger of a random person. It stores a 12 byte data size. After storing of the fingerprint, I checked the same byte with the person entering the finger, if it matches then fingerprint was correct.
Smoke Sensor : ADC Communication

Once all the Sensors were integrated with TIVA , the data which was sent to Queue , posted the data on UART port of TIVA board which was connected with BBG board. RX of TIVA was connected to TX of BBG and vice versa.

In order to check the data being received on the BBG, local terminal of BBG was used to check the incoming data. The data which was sent from BBG was a combination of Temperature, Ultrasonic, Fingerprint and Smoke Sensor. In order to seperate this data, I used "T24T", T means Temperature and data between two T indicated the data of the temperature from the temperature sensor. This is how data was being seperated and logged in a file.

After getting data from the TIVA, BBG used to process the data and in case of any alert it used to send some data back to TIVA and the actuators attached to TIVA used to run accordingly. 
For example, if Temperature went high then a certain threshold then LCD used to display "Alert temperature"
Smoke sensor value goes high then buzzer used to sound an alarm
Fingerprint value is Correct , it rotates a servo motor
Ultrasonic value is between a certain range then led used to remain ON.

Commands Used : GetCharData, GetPutData

Moreover, on BBG I had written a script for an automative running of the BBG code. Thus once the code was burned on Tiva and BBG whenever you plug them out and give them supply after sometime , still it used to work on its own without any human intervention.

Whenever there was an Alert detected by BBG or some sensor was not connected properly or when the communication link was broken between the TIVA and BBG , 7 segment display was connected on the BBG side which used to show a certain number which indicated that specific error

Problem Faced : While connecting so many sensors and accuators on a single TIVA board , sometimes the TIVA board was not able to provide appropriate current to the acctuators or sensors because the maximum output from the TIVA board was 40-60mA and the sensors like Fingerprint and acctuators like servo used to procure a current range of 100-200 mA and even more . So I connected an external supply for these and provided voltage and set some current range for them from an external suuply. 


Validation Plan : https://drive.google.com/open?id=12Sgx5I1wfiK8u7cKQgNSDgkUnxxTGB3S  
Report : https://drive.google.com/open?id=1EsaQF1W9tX8xjdOYZUIUGEy3_HKX9kEA  

References : https://github.com/rohitsay89/lcd_16x2/blob/master/lcd_16x2_code.c  
http://beaglebone.cameon.net/home/using-the-gpios  
https://energia.nu/pinmaps/img/EK-TM4C1294XL-FRONT.jpg  
https://docs.google.com/document/d/1QzakkATP8IdyN5t7XYYzXEXwnb7MoClkz2cKNlQPP7o/edit  
http://beaglebone.cameon.net/home/using-the-gpios  
https://github.com/YashBansod/ARM-TM4C-CCS  

Refer TIVA C Folder for TIVA C Code and BBG Code for BBG code

Remaining Documents are development stage modules

Useless Zip folders are deleted for better understanding 

 
