#include<stdio.h>
#define led 11
#define echoPin 6
#define trigPin 7
#define buzzer 10
int duration;
int distance;
int FirstDistance=0;
int SecondDistance=0;

//int Time = 2.0;
//int delayedtime = 1000*Time;

//Left Motor Pins  
int Enable1 = 2;
int Motor1_Pin1 = 4;  
int Motor1_Pin2 = 5;  

//Right Motor Pins      
int Motor2_Pin1 = 9; 
int Motor2_Pin2 = 8;
int Enable2 = 12; 

char direction; //variable to store the data
int speed; //Variable to control the speed of motor
int velo;

int getdistance()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2
    Serial.println(distance);
    return distance;
}

int carspeed()
{
    FirstDistance = getdistance(); //get the first distance
    delay(1000);
    SecondDistance = getdistance(); //gets the second distance
    speed = (FirstDistance - SecondDistance)/1;
    return speed;
}

void check(){
    //conditions based on distance of the car from obstacle
    if(getdistance() < 150)
        digitalWrite(led, HIGH);
    else if(getdistance() < 100){
      tone(buzzer, 300);
        delay(1000);
        noTone(buzzer);
        delay(1000);
    }
    //decreasing speed as a function of distance
    else if(getdistance() < 50){
        map(velo, 0, getdistance(), 0, carspeed());
        analogWrite(Enable1, velo);
        analogWrite(Enable2, velo);
    }
}
    
void setup() 
{       
  //Set the baud rate of serial communication and bluetooth module at same rate.
    Serial.begin(9600);  
//  bluetooth.begin(9600);

  //Setting the L298N, LED, Buzzer, UDS pins as output and input pins.
    pinMode(Motor1_Pin1, OUTPUT);  
    pinMode(Motor1_Pin2, OUTPUT);
    pinMode(Enable1, OUTPUT);
    pinMode(Motor2_Pin1, OUTPUT);  
    pinMode(Motor2_Pin2, OUTPUT);
    pinMode(Enable2, OUTPUT); 
    pinMode(buzzer, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

  //Setting the enable pins as HIGH.
    digitalWrite(Enable1, HIGH);
    digitalWrite(Enable2, HIGH);
}

void loop(){
//  if(bluetooth.available() > 0){  //Checking if there is some data available or not
    direction = Serial.read();   //Storing the data in the 'command' variable
   // Serial.println(direction);      //Printing it on the serial monitor
    
    //Change pin mode only if new command is different from previous.   
    switch(direction){
    case 'w':  //Moving the Car Forward
        digitalWrite(Motor2_Pin1, HIGH);
        digitalWrite(Motor2_Pin2, LOW);
        digitalWrite(Motor1_Pin1, HIGH);
        digitalWrite(Motor1_Pin2, LOW);
        check();
        break;
    case 's':  //Moving the Car Backward
        digitalWrite(Motor2_Pin1, LOW);
        digitalWrite(Motor2_Pin2, HIGH);
        digitalWrite(Motor1_Pin1, LOW);
        digitalWrite(Motor1_Pin2, HIGH);
        check();
        break;
    case 'a':  //Moving the Car Left
        digitalWrite(Motor1_Pin1, LOW);
        digitalWrite(Motor1_Pin2, LOW);
        digitalWrite(Motor2_Pin1, HIGH);
        digitalWrite(Motor2_Pin2, LOW);
        check();
        break;
    case 'd':   //Moving the Car Right
        digitalWrite(Motor1_Pin2, LOW);
        digitalWrite(Motor1_Pin1, HIGH);  
        digitalWrite(Motor2_Pin1, LOW);
        digitalWrite(Motor2_Pin2, LOW);
        check();
        break;
    case 'q':   //Stop
        digitalWrite(Motor2_Pin2, LOW);
        digitalWrite(Motor2_Pin1, LOW);
        digitalWrite(Motor1_Pin2, LOW);
        digitalWrite(Motor1_Pin1, LOW);
        break; 
    }
  } 
