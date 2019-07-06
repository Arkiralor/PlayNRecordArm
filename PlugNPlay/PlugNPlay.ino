/*
  Main file to program a record and play robotic arm using an MPU6050 IMU.
  ** Coded and developed by arkiralor
  ** MPU_tockn.h library developed by to tockn, https://github.com/tockn .
  Please read the README.MD and CIRCUITDIAGRAM.PNG before using.
*/


#include <Servo.h> //header file for servoes.
#include <Wire.h> //header file for I2C communication protocol.
#include <MPU6050_tockn.h> //3rd party header for getting IMU readings.

Servo base, shoulder, elbow; //defining the servoes.
MPU6050 mpu6050(Wire); //defining an MPU which will communicate using the I2C i.e, Wire bus.
long timer=0; //will be used to store the elapsed time since we will only record a reading after every 500ms.

const int recorder=8, player=12; //defining the record and play pins.

int p=90, i=0, posrec; //posrec will be used to memorize the last known position of angle value stored in the arrays.

float arrayz[70], arrayy[70];


void setup() 
{
  Serial.begin(9600);
  
  base.attach(3); //attaching 'base' to digital pin 3 (PWM).
  shoulder.attach(5); //attaching 'shoulder' to digital pin 5 (PWM).
  elbow.attach(6); //attaching 'elbow' to digital pin 6 (PWM).

  base.write(90); //setting base to 90 deg.
  shoulder.write(90); //setting shoulder to 90 deg.
  elbow.write(90); //setting elbow to 90 deg.
  delay(100);
  
  pinMode(recorder, INPUT); //defining record button as input for electric potential threshold.
  pinMode(player, INPUT); //defining play button as input for electric potential threshold.

  Wire.begin(); //initiates I2C communication.
  mpu6050.begin(); //starting the MPU module.
  mpu6050.calcGyroOffsets(true); //callibrating the MPU module and calculating the offsets to be used.

}

void loop() 
{
   while(digitalRead(recorder)==HIGH && digitalRead(player)==LOW && i<70) //as long as the record button is closed.
   {
      base.detach(); //detaching 'base' from digital pin 3 (PWM).
      shoulder.detach(); //detaching 'shoulder' from digital pin 5 (PWM).
      elbow.detach(); //detaching 'elbow' from digital pin 6 (PWM).
      Serial.println("Recording ON!"); //serial monitor o/p to signal that the user can start recording.
    
      if(millis()-timer>500) //take a reading every 500ms.
      {
        recordarray(); //record the movements of the arms.
      
        posrec=i; //update the position variable.
        i++; //update array index.
        timer=millis(); //updates elasped time.
        Serial.println(millis());
        Serial.print(" ms elapsed.");
      }
    }
  
  while(digitalRead(recorder)==LOW && digitalRead(player)==HIGH) //as long as the play button is closed.
  {
    Serial.println("Playback ON!"); //serial monitor o/p to signal that the arm is now playing back recorded movements.
    
    base.attach(3); //attaching 'base' to digital pin 3 (PWM).
    shoulder.attach(5); //attaching 'shoulder' to digital pin 5 (PWM).
    elbow.attach(6); //attaching 'elbow' to digital pin 6 (PWM).
    
    base.write(90); //setting base to 90 deg.
    shoulder.write(90); //setting shoulder to 90 deg.
    elbow.write(90); //setting elbow to 90 deg.
    
    playarray(); //playback the recorded movements.
  }
}


void recordarray()
{
  mpu6050.update(); //function initiated to get gyro values with complimentary filter.
  
  arrayz[i]=mpu6050.getAngleZ(); //stores the z-axis value for the 'base' servo.
  
  
  arrayy[i]=mpu6050.getAngleY(); //stores the y-axis value for the 'shoulder' and 'elbow' servoes.
}

void playarray()
{
  for(i=0;i<=posrec;i+=1) //loop repeats for i=0 to i<=posrec.
  {
    base.write(90+arrayz[i]); //rotate base according to Z-value.
    Serial.println("Base record: ");
    Serial.print(arrayz[i]);
    Serial.println("");

    shoulder.write(90+arrayy[i]*0.66); //rotate shoulder by 66% of Y-value.
    Serial.println("Shoulder record: ");
    Serial.print(arrayy[i]*0.66);
    Serial.println("");
      
    elbow.write(90-arrayy[i]*0.33); //rotate elbow by 33% of -(Y-value) as the elbow is mirrored to the shoulder (keystone of Y-axis).
    Serial.println("Elbow record: ");
    Serial.print(arrayy[i]*0.33);
    Serial.println("");
    //Please note that we are taking 90 degrees as the 0-position of the arm.
   }
}
