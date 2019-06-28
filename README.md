# PlayNRecordArm
ABSTRACT

Robots are mechanized automatons and may come in multiple shapes and sizes.
The focus of our study would be a robotic arm such as one usually found in assembly-line deployments in factories around the globe. 
Most of these arms are programmed to perform a fixed set of movements. Usually, these movesets are defined using a separate hardware or software interface. This presents a certain level of difficulty in configuring the machine.
This project aims to construct a robot that can however, be configured by the user by physically moving the arm through the required movements without the need of any intermediate hardware or software layer. 
If successful, this would be preferable to methods currently in use at large as this would provide some haptic feedback to the operator, a feature that is either absent in other methods or is only simulated.
Although this approach may reduce precision down to a human level, for general purposes if done carefully enough, the advantages gained may outweigh the disadvantages.
 

CHAPTER 1: INTRODUCTION

	A manually-trainable robotic arm is a mechanical appendage resembling a human forelimb that does not require any middleware for interfacing between the user and arm.
For this kind of an arm, we physically move the arm to whatever position(s) we need it to be at, by pushing and pulling the arm and not a controller device while the arm is recording and when prompted, the arm plays back the movements as closely as possible, repeatedly.
For the purpose of this project, we will be using an IMU named ‘MPU6050’ to record the rotational positions of the armatures that comprise the arm to a data-structure that we will then use to replay the stored movements. We will use a mathematical/logical function to convert the rotational position vectors stored in the data-structure to simple angular values between 0 degrees and 180 degrees that can be accepted by the servos which will act as the joints of the arm.
	PROBLEM STATEMENTS
To create a robotic mechanism that can be easily configured to repeat a set of movements without the need for user to understand complex mathematical and electrical principles.
The task is: Record all movements of the arm, caused by the human operator moving the arm manually and then playback the recorded movements within an acceptable range of accuracy.
	OBJECTIVES
The objective of this project is to construct a basic, low-cost robotic arm that can record its movements and then play back the recorded move sets.
The mechanism uses an MPU6050 IMU with 3-axis gyroscope and accelerometer to record the orientation of the arm and a series of SG90 servos to power the arm and actually move the joints during move-set playback.
An Arduino Uno R3 with will be used as the brains of the mechanism to manage and operate all the mentioned modules.




CHAPTER 2: METHODOLOGY

Our system will consist of three modules:
	The Arduino itself
	An MPU6050 IMU with a 3-Axis Gyroscope
	A set of SG90 servomotors, every axis of rotation will have at least one servo attached to provide rotational movement along that particular axis.
The system will operate in two mode:
	Recording Mode:
Here, the system will record the orientation of the arm, sampling every 500milliseconds and store the data in an array. There will be a distinct array for every single
	Playback Mode:
Here, the system will start at an index value of 0 and pass the values stored into the servo, after passing the stored values through a processing filter.
To select whether the system will record or playback movements, we will use a selector switch attached to Arduino.

  2.1 PROPOSED METHOD
	Pressing the record button will tell the Arduino to go into Recording Mode. The Arduino will then detach the servos from their respective GPIO pins. The user will physically move the arm, carrying out the movements and vector paths the arm will need to follow. The sampling rate will be 2Hz i.e, once every 500 miliseconds. The readings from the IMU will be stored in two separate arrays: one for Z-axis values and one for Y-axis values. Opening the record button and then closing the playback button will tell the Arduino to go into Playback Mode. All the servos will be sent to the calibration position (90 degrees in this case).
The system will start to read the array values sequentially: First Z[i], then Y[i] for all values of i, such that:
		
		i ∈ [0, posrec] | posrec = highest known value of i while recording movements.
		
The reading from Z[i] will be sent to the base servo, such that:
		
			set base to (90+Z[i]) degrees.
		

The reading from Y[i] will be split between the shoulder and elbow servos in a 2:3 ratio and mirrored between them and they are vertically mirrored to each other, such that:

			set shoulder to (90+Y[i]*0.6666) degrees.
			set elbow to (90-Y[i]*0.3333) degrees.
		
This will repeat until the record switch is closed again, after I = posrec, the loop will start again from I = 0.
	2.1.1 Extracting Data from the MPU6050:
	The MPU6050 stores the data in an 8-bit sequence that can be extracted via:
	
		 rawGyroB = wire->read() << 8 | wire->read(); 
		 
Where B can be X, Y or Z (corresponding to the required axis. 
	This extracts the raw data from the IMU, which can be converted to the gyrorate using:
	
	    gyroB = (((rawGyroB) / 65.5) + B_offset; **
	    
** We need to divide by 65.5 due to our sensitivity of 500°/sec according to the MPU6050 datasheet[1]. 
After we find the gyrorate for a given axis, we can find the angular displacement along that axis using:
	
	angleGyroX += gyroX * interval;
Where interval is the time elasped since the last reading from the IMU. 
	2.1.2 Signalling SG90 to rotate to an angular value:
      
The sg90, is signalled using PWM through a data pin with the function:

		servo.write(angle) 
		
During playback we can signal a particular servo to move to an angular value using the filter: 
The Y-read is presented in that way due to the fact that the shoulder and elbow servo are vertically mirrored in orientation and the shoulder/elbow being responsible for 66% & 33% of the rotation along the Y-axis as can be determined from the construction of the arm-frame. 
    2.2 SYSTEM ARCHITECTURE
The principal architecture of the robotic arm is as follows:
	One Arduino Uno R3 as the brain of the mechanism.
	One mode-selector module composed of a 3-state switch with each of its outputs connected to two different GPIO pins of the Arduino.
	One IMU (MPU6050 in our case) attached to one extremity of the arm so as to experience the full range of movement induced by the user and communicating with the brain via the I2C communication bus.
	Three SG90 microservomotors attached to each joint of the arm and each of their signal pins connected to the brain at a PWN GPIO pin (D3, D5 and D6 in our case).
	
The circuit layout of the mechanism is as follows:
The Vcc and Gnd pins of the MPU6050 are connected to the Arduino’s 5V and Gnd pins, respectively. The SDA pin is connected to the Arduino’s A4 pin; the SCL pin is connected to the Arduino’s A5 pin. An external 9V/2A power supply is connected to the Arduino’s Vin pin. One of the Arduino’s Gnd pins is connected to a 5V/2.8A external power supply to provide a common drain for the current. The common pin of the 2-way switch is connected to the positive terminal of the 5V/2.8A power supply. 

One output pin is connected to the Arduino’s D8 pin, this will be the record state selector. One output pin is connected to the Arduino’s D12 pin, this will be the play state selector.

Each of the servos’ positive pin is connected to the positive terminal of the 5V/2.9A power bar and their negative pins are connected to the bar’s negative terminal. The signal pin of the base servo is connected to D3 (PWN) on the Arduino. The signal pin of the shoulder servo is connected to D5 (PWN) on the Arduino. The signal pin of the elbow servo is connected to D6 (PWN) on the Arduino.


The MPU6050 which we will be using, is a combination 3-axis accelerometer and gyroscopic sensor or IMU.
The MPU6050 detects rotational velocity along an axis by the current induced along that axis by the piezzo-electric crystal and the measuring the current to determine how fast the object is rotating along that asix. The working principle of this phenomenon is beyond the scope of this project. All we need to know is that the gyroscope outputs the rotational velocity at a given point in time. We can set the sampling rate used to find the instantaneous rotational velocity by changing the sensitivity of the Gyro sensor of the IMU module. 

The sensitivities provided by the MPU6050 Gyroscope are:

	+/- 250, +/- 500, +/- 1000 and +/- 2000 degrees of rotation/sec
	
For our purposes we will be setting the sensitivity of the Gyro to +/- 500 degrees per second.
The IMU stores the reading from each axis in two bits. By extracting the data stored in the bits, we can obtain the RAW data for the corresponding axis.
To calculate the angle from the gyrodata, we first calculate the gyro-rate (speed of rotation) from the raw value. To calculate the gyrorate for X axis, we divide the raw value for x by 65.5.

			GyroXrate= gx/65.

We take gyro sensitivity as ±500 degrees/sec and due to this we divided gx by 65.5 to calculate the gyro-rate, as explained in the datasheet for the MPU6050. Similarly we can calculate gyro-rate for Y & Z.
Now we need to integrate the gyrorate over time to get value of the angular position from the data sent by gyroscope:

			GyroXangle= GyroXrate*dt + GyroXangle

In the same manner the angular positions along the other axes can be determined.
Before all this however, we will first need to find the offsets of the gyro sensor. The offset of the gyro is the difference between the actual orientation of the IMU and the theoretical orientation of the IMU in the calibration software.

			Xoff = (Xreal-Xtheory)/(number of readings)
			
To use the offset, we simply perform:

			GyroXangle = GyroXangle + Xoff
	
 However, the values obtained by this method will start to decay over time due to the inexact nature of the integration function used to initially find the angular rotation. This is called drift and occurs due to minute errors that occur in the sensor readings that add up over time and are compounded by the integration. Every gyro module will produce some amount of drift over time. 




To combat this drift, we can use two types of filter functions known as:
	Complimentary Filter
	Kalman Filter



For our case, we will apply the complimentary filter, which takes adjusts the value obtained from the sensor as follows:

		GyroXangle = 0.98*(GyroXangle + GyroXrate * dt) + 0.02 * (accelX)
		GyroYangle = 0.98*(GyroYangle + GyroYrate * dt) + 0.02 * (accelY)
		
Unfortunately, the complimentary filter function cannot be used on the Z-axis as the accelerometer cannot provide a reading for the same.
We can include all of these statements in a function named update() with sub-functions getXangle(), getYangle and getZangle() to get the instantaneous angular position along an axis. We will call this function at the sampling rate (2Hz) as long as the record button is pressed.

When the play button would be pressed, for every iteration of ‘i’ :
first the Z-axis rotation stored in Z[i] is passed to

		base as write(90+Z[i])
  
**This is due to the fact that the base servo is set at the keystone for the Z-axis movement.
then the Y-axis rotation stored is Y[i]  is passed to:
	
  		shoulder as write(90+Y[i]*0.6666)
  
**This is due to the fact that the shoulder servo is set at the keystone for the Y-axis movement and only 66% of the movement is due to the shoulder joint.

		elbow as write(90-Y[i]*0.3333)
  
**This is due to the fact that the elbow is vertically mirrored with the shoulder and only 66% of the movement is due to the shoulder joint.

This loop will run for all values of ‘i’ in the closed set [0, posrec], where ‘posrec’ was the highest recorded value of ‘i’ during the record phase of the main loop. 
This is done to ensure that any null values that may be left over in the array during the recording phase are not passed down to the servos during the playback phase of the loop, as:

		<90.000 + ‘o/’> is an undefined value.

This approach will hopefully be able to emulate any and all movements induced by the user to within a respectable degree of accuracy.




CHAPTER 3: IMPLEMENTATION

The mechanism will be implemented in Arduino, the coding for the brain will be done in the proprietary Arduino language which happens to be a subset of C/C++.
The main algorithm will be as follows:

			//Check button status
			if(record button is pressed)
				while the button is pressed
		    			if(500ms has passed since the last reading)
						store Z-axis rotation to Z[i]
						store Y-axis rotation to Y[i]
			else if(play button is pressed)
				for(i between 0 and posrec)
					send Z[i] to base servo.
					send Y[i]*0.6666 to shoulder servo.
					send -Y[i]*0.3333 to elbow servo. //elbow and shoulder are vertically mirrored.

3.1 PROBLEMS ENCOUNTERED DURING IMPLEMENTATION
  
1. Servomotors getting stuck
       
This as initially thought to be cause by the less than ideal power delivery, it was later found out that it was caused by the shoddy construction of frame. This was rectified by disassembling the frame and polishing the surfaces with a sandpaper sheet and enlarging the joint holes with a 2mm drill bit.
    
 2. Servomotors moving in jerked sequences
      
This is due to the fact that the sampling rate is not equal to the playback rate. The sampling rate has to be low as the 8-bit microprocessor used by the Arduino simply doesn’t have enough memory available to have store a high resolution sampling of the movements. 
In future implementations, an EEPROM module to increase the available memory would be preferable.



 3. Gyro readings experiencing drift
    
Gyro drift is a universal problems in all IMU modules that contain a gyroscopic sensor. As discussed above, this is due to slight imperfections in the RAW data sent by the sensor that are compounded over time by the integration function. This can be eliminated or at least mitigated by using either a Complimentary Filter or a Kalman Filter. In our case, we have deployed a complimentary filter which reduces to gyro drift to a almost insignificant amount.
  
  4. Power delivery
    
When the servos were directly powered from the Arduino, it was observed that the board wasn’t able to deliver enough current to both the IMU and the set of servos. This was rectified by leaving the IMU powered by the Arduino and powering the servos by a dedicated 5V/2.8Amp line. 

  5. Apparent Freezing of the Serial Monitor
    
The serial monitor is used to check the status of this deployment i.e, whether the mechanism is in recording mode or playback mode. During initial implementation, the Arduino apparently froze with the serial monitor becoming unresponsive after the calibration stage.
During this stage, the baud rate was set to 9600baud/sec which, with 1 reading only containing 1 bit was equal to 9600bps of 9.6kbps. Which was apparently not high enough of a bandwidth to accommodate this deployment.
This was rectified by increasing the baud rate to 2’000’000 baud/sec which would be equal to:

		(((2000000*1)/8)/1024) bytes/sec.

This value is equal to 244.14kBps.

REFERENCES

[1] Peng Cheng, Student Member, IEEE, and Bengt Oelmann, Joint-Angle Measurement Using Accelerometers and Gyroscopes—A Survey, IEEE TRANSACTIONS ON INSTRUMENTATION AND MEASUREMENT, VOL. 59, NO. 2, FEBRUARY 2010

[2] Ariful Islam Bhuyan, Tuton Chandra Mallick Department of Electrical and  Electronic Engineering, Premier University, Chittagong, Bangladesh, Gyro-Accelerometer based control of a robotic Arm using AVR Microcontroller, The 9th International Forum on Strategic Technology (IFOST), October 21-23, 2014, Cox’s Bazar, Bangladesh

[3] J. Y. S. Luh, Senior Member IEEE, An Anatomy of Industrial Robots and Their Controls, Transactions on Automatic Control, Vol. AC-28, No. 2, February 1983

[4] Arduino Playground, https://playground.arduino.cc/ 

[5] TinkerCAD, https://www.tinkercad.com

[6] EEEnthusiast, https://www.youtube.com/user/EEEnthusiast

[7] Tockn, MPU6050_tockn.h, https://github.com/tockn/

[8] Arduino Forum, https://forum.arduino.cc/

[9] Maker Pro, https://maker.pro/

[10] How to Mechatronics, https://howtomechatronics.com/

